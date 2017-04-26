#include "ofApp.h"
//#include "character.hpp"
//--------------------------------------------------------------
void ofApp::setup(){
   
    soundStream.printDeviceList();
    int bufferSize = 256;
    
    //set initial values for beat detection
    decayRate = 0.05;
    minimumThreshold = 0.1;
    audiothreshold = minimumThreshold;
    soundStream.setup(this, 0, 2, 44100, bufferSize, 4);
    
    
    ofBackground(0,255,0);
    character.setup();
    bullet.setup();
    camWidth = 320;
    camHeight = 240;
    cam.setup(camWidth, camHeight);
    colorImg.allocate(320,240);
    grayImage.allocate(320,240);
    grayBg.allocate(320,240);
    grayDiff.allocate(320,240);
    threshold = 80;
    shoot = false;
    score = 0;

    
    for(int i = 0; i < ofRandom(1,300); i++) {
        // Draw a particle from the blob's center
        Particle p;
        p.setInitialCondition(ofRandom(1,ofGetWidth()),ofRandom(1, ofGetHeight()), ofRandom(-1,1), ofRandom(-1,1));
        particles.push_back(p);
        
    

    }
}
void ofApp::audioIn(float * input, int bufferSize, int nChannels){
    //RMS Algorithm. Get loudness of input.
    
    float rms = 0.0;
    int numCounted = 0;
    for (int i = 0; i < bufferSize; i++) {
        float leftSample = input[i * 2] * .5;
        float rightSample = input[i * 2 + 1] * .5;
        rms += leftSample * leftSample;
        rms += rightSample * rightSample;
        numCounted += 2;
    }
    
    rms /= (float)numCounted;
    rms = sqrt(rms);
    loudness = rms;
    
    
    audiothreshold = ofLerp(audiothreshold, minimumThreshold, decayRate);
    if(loudness > audiothreshold) {
        float vx = ofRandom(-2,2);
        //the downward trajectory is going to depend on how loud our input is
        //float vy = ofRandom(0,loudness*100);
        float vy = loudness*10000;

    }
    
}


//--------------------------------------------------------------
void ofApp::update(){
    float dt = 0.006f;
    //character.update(dt);
    bullet.update(shoot);
    if(bullet.y < 201) {
        shoot = false;
    }
    
    bool bNewFrame = false;
    
    cam.update();
    bNewFrame = cam.isFrameNew();
    if (bNewFrame){
        
        colorImg.setFromPixels(cam.getPixels());
        
        grayImage = colorImg;
        
        // take the abs value of the difference between background and incoming and then threshold:
        grayDiff.absDiff(grayBg, grayImage);
        grayDiff.threshold(threshold);
        
        // find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
        // also, find holes is set to false so we won't get interior contours
        contourFinder.findContours(grayDiff, 20, (340*240)/3, 10, false);	// find holes
    }
    
    
   
}

//--------------------------------------------------------------
void ofApp::draw(){
    //ofDrawBitmapString("RMS: " + ofToString(loudness),100,100 );
    if(loudness > 0.2){
        shoot=true;
    }

    ofSetColor(0);

    ofDrawBitmapString("Score: ", 20, 60);
    ofDrawBitmapString(score, 70, 60);
    character.draw();
    bullet.draw();
    
    for(int i = 0; i < contourFinder.nBlobs; i++) {
        if(ofMap(contourFinder.blobs[i].centroid.x, 0, 340, 0, ofGetWidth()) > ofGetWidth()/2){
            if(character.x < ofGetWidth()){
                character.x++;
                bullet.x++;
            }
        } else {
            if(character.x > 0){
                character.x--;
                bullet.x--;
            }
        }
        //character.x = ofMap(contourFinder.blobs[i].centroid.x, 0, 340, 0, ofGetWidth());
                            //contourFinder.blobs[i].centroid.x;
    }
    for (int i = 0; i < particles.size(); i++){
        particles[i].draw();
         if((character.x < particles[i].pos.x) && ((character.x + 100) > particles[i].pos.x) && (particles[i].pos.y > (ofGetHeight()-100)) )
         {
             particles.erase(particles.begin() + i);
         }
        if( ((bullet.x-30) < particles[i].pos.x) && ((bullet.x + 30) > particles[i].pos.x) && (particles[i].pos.y > bullet.y) )
        {
            particles.erase(particles.begin() + i);
            score++;
        }

    }
    
    ofSetColor(255);
    for(int i = 0; i < particles.size(); i++) {
        particles[i].resetForce();
        particles[i].bounceOffWalls();
        particles[i].addDampingForce();
        particles[i].update();
        
        
    }


}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    play = true;
}
//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
//character.x = x;
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
