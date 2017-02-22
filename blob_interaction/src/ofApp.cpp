#include "ofApp.h"

//--------------------------------------------------------------
//--------------------------------------------------------------
void ofApp::setup(){
    camWidth = 320;
    camHeight = 240;
    
    cam.setup(camWidth, camHeight);
    colorImg.allocate(320,240);
    grayImage.allocate(320,240);
    grayBg.allocate(320,240);
    grayDiff.allocate(320,240);
    
    threshold = 20;
    ofBackground(0);
}

//--------------------------------------------------------------
//--------------------------------------------------------------
void ofApp::update(){
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
    for(int i = 0; i < particles.size(); i++) {
        particles[i].resetForce();
        particles[i].bounceOffWalls();
        particles[i].addDampingForce();
        particles[i].update();
    }
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    for (int i = 0; i < particles.size(); i++){
        particles[i].draw();
    }
    ofSetColor(255);
    for(int i = 0; i < contourFinder.nBlobs; i++) {
       // Draw a particle from the blob's center
       Particle p;
        p.setInitialCondition(ofMap(contourFinder.blobs[i].centroid.x, 0, 340, 0, ofGetWidth()), ofMap(contourFinder.blobs[i].centroid.y, 0, 240, 0, ofGetHeight()), ofRandom(-1,1), ofRandom(-1,1));
        particles.push_back(p);
        
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    grayBg = grayImage;		// the = sign copys the pixels from grayImage into grayBg (operator overloading)
    
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
