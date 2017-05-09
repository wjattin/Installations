#include "ofApp.h"
/*William Jattin CIM 795 SPRING 2017 UNIVERSITY OF MIAMI */
//--------------------------------------------------------------
void ofApp::setup(){
    mode = 0; //0 seasons or 1 beach ball "game"
    sloaded = true;
    bloaded = false;
    bg.load("summer.png");
    ball.load("emoji.png");
    
    //Particles 2d
    ofSetVerticalSync(true);
    ofBackgroundHex(0xffffff);
    ofSetLogLevel(OF_LOG_NOTICE);
    //World Settings
    bMouseForce = false;
    bbool = true;
    box2d.init();
    box2d.setGravity(0.0, 2);//orig 10
    box2d.createGround();
    box2d.setFPS(30.0);
    box2d.registerGrabbing();
    //Kinect Settings
    nearClip.set(150); //old 500
    farClip.set(3000); //3487
    bFlipX.set(true);//to make it look like a mirror
    bFlipY.set(false );
    
    // cv //
    numDilatePasses.set(2); //ORIG 3
    blurAmount.set(2); //ORIG 1
    threshold.set(25); //ORIG 20
    minSize.set(4.725); //orig 4.725
    maxSize.set(599 );
    
    contourPolySpacing.set(3);
    contourSmoothing.set(1);
    // Left over from example code
    minPixToActivateBox.set(23 );
   
    bUseLiveKinect = true;
    // init the kinect
    // bool ofxKinect::init(bool infrared, bool video, bool texture)
    kinect.init(false,false,true);
    // try to open the default kinect location //
    if( !kinect.open() ) {
        string tVideoPath = "No video";
        cout << "could not open kinect! " << endl;
    } else {
        cout << "opened kinect serial: " << kinect.getSerial() << endl;
    }
    
    ofSetFrameRate(30);

}

//--------------------------------------------------------------
void ofApp::update(){
    // Destroy all bubbles attached to the contour before recreating
    for(int i=0; i< bubbles.size(); i++) {
        bubbles[i].get()->destroy();
    }
    bubbles.clear();
    contours.clear();
    // Setting stage for mode
    if( (mode == 0) || (mode == 3)) {
        createSnow();

       if(!sloaded){
           if(mode == 0){
           bg.load("summer.png");
           } else if(mode == 3) {
               bg.load("winter.png");
           }
        beachball.destroy();
        edgeLine.destroy();
        bloaded = false;
        sloaded = true;
        }

    } else {

        if(!bloaded){
        destroySnow();
            // Load empji image and setup circle property
            bg.load("beach.png");
            beachball.setPhysics(0.2, 1.75, 0.31);
            beachball.setup(box2d.getWorld(), (ofGetWidth()/2), (ofGetHeight()/2), 80);
            // In this mode, edgeline is used as screen boundaries
            drawing.addVertex(4, 4);
            drawing.addVertex((ofGetWidth()-8), 4);
            drawing.addVertex((ofGetWidth()-8), (ofGetHeight()-8));
            drawing.addVertex(4, (ofGetHeight()-8));
            drawing.addVertex(4, 4);
            // Create an edge from drawing's vertices
            edgeLine.addVertexes(drawing);
            edgeLine.setPhysics(0.0, 0.1, 0.1);
            edgeLine.create(box2d.getWorld());
            
            bloaded = true;
            sloaded = false;
            cout << "ball loaded " << endl;
        }
    
    }
    
    
    //Kinnect input
    bool bReceivedNewFrame = false;
    
    if( bUseLiveKinect ) {
        kinect.update();
        // only perform cpu intense cv operations when new data has been received //
        if( kinect.isFrameNew() ) {
            if( nearClip != kinect.getNearClipping() || farClip != kinect.getFarClipping() ){
                kinect.setDepthClipping(nearClip, farClip);
            }
            bReceivedNewFrame = true;
            grayPixels = kinect.getDepthPixels();
            grayCv.setFromPixels( grayPixels );
        }
    } else {
        videoPlayer.update();
        if( videoPlayer.isFrameNew() ) {
            bReceivedNewFrame = true;
            ofPixels& videoPixels = videoPlayer.getPixels();
            // allocate the cv images to the size of the video pixels //
            if( colorCv.getWidth() != videoPixels.getWidth() ){
                // we aren't going to draw these cv images, so do not use a texture
                // to help reduce overhead //
                colorCv.setUseTexture(false);
                colorCv.allocate(videoPixels.getWidth(), videoPixels.getHeight());
                grayCv.setUseTexture(false);
                grayCv.allocate(videoPixels.getWidth(), videoPixels.getHeight());
            }
            // the video comes in as RGB
            colorCv.setFromPixels(videoPixels);
            // ofxCvGrayscaleImage can be set from an ofxOpenCvColorImage
            // converts from color to gray
            grayCv      = colorCv;
            //grayPixels  = grayCv.getPixels();
        }
    }
    
    if( bReceivedNewFrame ) {
        if( bFlipX || bFlipY ){
            grayCv.mirror(bFlipY, bFlipX);
        }
        prevFrame = processedCv;
        // set the cv image from the pixels we have extracted //
        //        processedCv.setFromPixels( grayPixels );
        if( processedCv.getWidth() == 0 ) {
            processedCv.allocate( grayCv.getWidth()/2, grayCv.getHeight()/2 );
        }
        // perform operations at a smaller size //
        processedCv.scaleIntoMe( grayCv, CV_INTER_LINEAR );
        if( blurAmount > 0 ) {
            //processedCv.blurGaussian( preBlurAmount*2+1 );
        }
        processedCv.threshold( threshold );
        
        for( int i = 0; i < numDilatePasses; i++ ) {
            processedCv.dilate();
            if(blurAmount>0)processedCv.blurGaussian(blurAmount*2+1);
            processedCv.erode_3x3();
        }
        
        if( prevFrame.getWidth() && processedCv.getWidth() ) {
            historyCv.absDiff( prevFrame, processedCv );
        }
        
        //        findContours( ofxCvGrayscaleImage&  input,
        //                     int minArea,
        //                     int maxArea,
        //                     int nConsidered,
        //                     bool bFindHoles,
        //                     bool bUseApproximation)
        if( maxSize < minSize ) {
            maxSize = minSize;
        }
        finder.findContours( processedCv, minSize*minSize, maxSize*maxSize, 20, true, false);
        
        // copy the contours and apply some smoothing //
        vector< ofxCvBlob >& blobs = finder.blobs;
        
        for( int d = 0; d < blobs.size(); d++ ) {
            ofPolyline tempPoly;
            tempPoly.addVertices( blobs[d].pts );
            if( tempPoly.getPerimeter() > contourPolySpacing ) {
                tempPoly = tempPoly.getResampledBySpacing( contourPolySpacing );
            }
            if( contourSmoothing > 0 ) {
                tempPoly = tempPoly.getSmoothed( contourSmoothing );
                //tempPoly = tempPoly.getSmoothed( 20,0.5 );
            }
 
            contours.push_back( tempPoly );

        }
        // all of the contours are relative to the width and height of the kinect depth image
        // convert them to screen space //
            float xscale = (float)ofGetWidth() / processedCv.getWidth();
            float yscale = (float)ofGetHeight() / processedCv.getHeight();


        cout << "CONTOURS "<< contours.size() <<  endl;

        for( int i = 0; i < contours.size(); i++ ) {
                // Clear blob drawing before each blob iteration
                // loop through all of the vertices and scale them //
                for( int j = 0; j < contours[i].getVertices().size(); j++ ) {
                    contours[i].getVertices()[j].x *= xscale;
                    contours[i].getVertices()[j].y *= yscale;
                    // The vector approach kept on crashing so I used a limited number of drawings to trace the contours
                    // In this version only drawing1 and drawing2 are used
                    //box2d poly
                    if(i == 0){
                        drawing1.addVertex(contours[i].getVertices()[j].x, contours[i].getVertices()[j].y);
                    }
                    else if(i == 1){
                        drawing2.addVertex(contours[i].getVertices()[j].x, contours[i].getVertices()[j].y);
                    }
                    else if(i == 2){
                        drawing3.addVertex(contours[i].getVertices()[j].x, contours[i].getVertices()[j].y);
                    }
                    else if(i == 3){
                        drawing4.addVertex(contours[i].getVertices()[j].x, contours[i].getVertices()[j].y);
                    }
                    else if(i == 4){
                    drawing5.addVertex(contours[i].getVertices()[j].x, contours[i].getVertices()[j].y);
                    }
                    drawing1.setClosed(false);
                    drawing1.simplify();

                }
                
            }
            // scale the blobs to screen size
            float rxscale = processedCv.getWidth() / (float)ofGetWidth();
            float ryscale = processedCv.getHeight() / (float)ofGetHeight();

    }
    
    // We attach and adgeline and particles to all contours to get better collisions
    drawing1.simplify();
    drawing2.simplify();
    
    edgeLine1.destroy();
    edgeLine2.destroy();
    
    
    edgeLine1.addVertexes(drawing1);
    edgeLine2.addVertexes(drawing2);
    
    edgeLine1.setPhysics(1, 0, 0); //density, bounce, and friction
    edgeLine2.setPhysics(1, 0, 0); //density, bounce, and friction
    
    // Bring edgeline into the world and save it in the vector destruction.
    edgeLine1.create(box2d.getWorld());
    edgeLine2.create(box2d.getWorld());
    
    // Add the bubbles to the drawing
    for(int i= 0; i < drawing1.size();i++){
        bubbles.push_back(shared_ptr<ofxBox2dCircle>(new ofxBox2dCircle));
        ofxBox2dCircle *b = bubbles.back().get();
        b->setPhysics(3.3, 0.1, 3.3);
        b->setup(box2d.getWorld(), drawing1.getVertices()[i].x, drawing1.getVertices()[i].y, 10);
        b->create();
    }
    for(int i= 0; i < drawing2.size();i++){
        bubbles.push_back(shared_ptr<ofxBox2dCircle>(new ofxBox2dCircle));
        ofxBox2dCircle *b = bubbles.back().get();
        b->setPhysics(3.3, 0.1, 3.3);
        b->setup(box2d.getWorld(), drawing2.getVertices()[i].x, drawing2.getVertices()[i].y, 5);
        b->create();
    }
    // NOTE: better performance when we update box2d at the end
    box2d.update();

    drawing1.clear();
    drawing2.clear();

    // remove shapes offscreen
    ofRemove(boxes, ofxBox2dBaseShape::shouldRemoveOffScreen);
    ofRemove(circles, ofxBox2dBaseShape::shouldRemoveOffScreen);
    ofRemove(customParticles, ofxBox2dBaseShape::shouldRemoveOffScreen);
    //end update
   
    }

//--------------------------------------------------------------
void ofApp::draw(){

    cout << "x " << beachball.getPosition().x << "y  " << beachball.getPosition().y  <<  endl;

    ofSetHexColor(0xFFFFFF);
    ofNoFill();
    
    bg.draw(0,0,ofGetWidth(),ofGetHeight());
    // Draw the emoji image on top of circle
    if(mode == 1){
    ball.draw((beachball.getPosition().x - 80),(beachball.getPosition().y -80),160,160);
    }
    ofFill();
    
    for(int i=0; i< bubbles.size(); i++) {
    ofSetColor(ofRandom(0,255), ofRandom(0,255), ofRandom(0,255));
      
    bubbles[i].get()->draw();
    }
    ofSetHexColor(0xffcc00);

    ofFill();
    ofSetHexColor(0xFFCC00);
    
    if(mode == 1) {
        // Draw edge to show boundaries
        edgeLine.draw();
    }
    
    ofSetHexColor(0x000000);
    ofNoFill();
    // These were user to debug
    bool lines = false;
    bool bodylines = true;

    //Draw particles
    if(mode == 0 || mode == 3){
        cout << "MODE is 0 test draw" <<  endl;
    for(int i=0; i<customParticles.size(); i++) {
        customParticles[i].get()->draw();
    }
    } else {
        cout << "MODE is 1 test draw" <<  endl;
    }
    
    if(lines) {
        ofSetHexColor(0x91A522);
        for( int i = 0; i < contours.size(); i++ ) {
            contours[i].draw();
        }
    }
//

//end draw
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 'c') {
        float r = ofRandom(4, 40);		// a random radius 4px - 20px
        circles.push_back(shared_ptr<ofxBox2dCircle>(new ofxBox2dCircle));
        circles.back().get()->setPhysics(3.0, 0.53, 0.1);
        circles.back().get()->setup(box2d.getWorld(), mouseX, mouseY, r);
        
    }
    
    if(key == 'b') {
        float w = ofRandom(4, 20);
        float h = ofRandom(4, 20);
        boxes.push_back(shared_ptr<ofxBox2dRect>(new ofxBox2dRect));
        boxes.back().get()->setPhysics(3.0, 0.53, 0.1);
        boxes.back().get()->setup(box2d.getWorld(), mouseX, mouseY, w, h);
    }
    
    if(key == 'z') {
        
        customParticles.push_back(shared_ptr<CustomParticle>(new CustomParticle));
        CustomParticle * p = customParticles.back().get();
        float r = ofRandom(3, 10);		// a random radius 4px - 20px
        p->setPhysics(0.4, 0.53, 0.31);
        p->setup(box2d.getWorld(), mouseX, mouseY, r);
        p->color.r = ofRandom(20, 100);
        p->color.g = 0;
        p->color.b = ofRandom(150, 255);
    }
    
    if(key == 'f') bMouseForce = !bMouseForce;
    if(key == 't') ofToggleFullscreen();
    if(key == 'b') {
        mode = 1;
    }
    if(key == 's') {
        mode = 0;
        bg.load("summer.png");
    }
    if(key == 'w') {
        mode = 3;
        bg.load("winter.png");


    }
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
    

}
//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

    box2d.createGround();
    
    if(bloaded) {
        drawing.clear();
        edgeLine.destroy();
        
        drawing.addVertex(4, 4);
        drawing.addVertex((ofGetWidth()-8), 4);
        drawing.addVertex((ofGetWidth()-8), (ofGetHeight()-8));
        drawing.addVertex(4, (ofGetHeight()-8));
        drawing.addVertex(4, 4);
        //drawing.addVertex((ofGetHeight()-8), 4);
        //drawing.addVertex(4, 4);
        edgeLine.addVertexes(drawing);
        edgeLine.updateShape();
        //polyLine.simplifyToMaxVerts(); // this is based on the max box2d verts
        edgeLine.setPhysics(0.0, 0.1, 0.1);
        edgeLine.create(box2d.getWorld());
    }
    


}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

void ofApp::createSnow(){
   
    for(int i = 0; i < 1; i++ ) {
    
    customParticles.push_back(shared_ptr<CustomParticle>(new CustomParticle));
    CustomParticle * p = customParticles.back().get();
    float r = ofRandom(3, 10);		// a random radius 4px - 20px
    p->setPhysics(0.4, 0.35, 0.31);//density, bounce, and friction
    p->setup(box2d.getWorld(), ofRandom(0, ofGetWidth()), 0, r);
        if(mode == 0){
        p->color.r = ofRandom(100, 145);
        p->color.g = ofRandom(145, 165);
        p->color.b = ofRandom(0, 34);
        } else
        {
            p->color.r = ofRandom(240, 255);
            p->color.g = ofRandom(255, 255);
            p->color.b = ofRandom(125, 255);
            
        }
    }

}
void ofApp::destroySnow(){
    
    for(int i = 0; i < 1; i++ ) {
        
        customParticles[i].get()->destroy();
        
    }
    customParticles.clear();
}
