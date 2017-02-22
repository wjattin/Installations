#include "ofApp.h"
#include "ofMain.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetBackgroundColor(0, 0, 0);
    sounds[0].load("a3.wav");
    sounds[1].load("b3.wav");
    sounds[2].load("c3.wav");
    sounds[3].load("d3.wav");
    sounds[4].load("e3.wav");
    sounds[5].load("f3.wav");
    sounds[6].load("g3.wav");
    
    /*sounds[0].setMultiPlay(true);
    sounds[1].setMultiPlay(true);
    sounds[2].setMultiPlay(true);
    sounds[3].setMultiPlay(true);
    sounds[4].setMultiPlay(true);
    sounds[5].setMultiPlay(true);
    sounds[6].setMultiPlay(true);
     */
    currentSound  = 0;

}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    if(sounds[0].isPlaying()){
    ofSetColor(255, 0, 0);
    ofDrawCircle(ofRandom(0,ofGetWidth()), ofRandom(0,ofGetHeight()), (sounds[0].getPosition()*100));
    currentSound++;
    }
    if(sounds[1].isPlaying()){
        ofSetColor(0, 255, 0);
        ofDrawCircle(ofRandom(0,ofGetWidth()), ofRandom(0,ofGetHeight()), (sounds[1].getPosition()*100));
        ofDrawCircle(ofRandom(0,ofGetWidth()), ofRandom(0,ofGetHeight()), (sounds[1].getPosition()*200));
       currentSound++;

    }
    if(sounds[2].isPlaying()){
        ofSetColor(0, 0, 255);
        ofDrawCircle(ofRandom(0,ofGetWidth()), ofRandom(0,ofGetHeight()), (sounds[2].getPosition()*200));
        ofDrawCircle(ofRandom(0,ofGetWidth()), ofRandom(0,ofGetHeight()), (sounds[2].getPosition()*100));
        currentSound++;
    }
    if(sounds[3].isPlaying()){
        ofSetColor(255, 0, 255);
        ofDrawCircle(ofRandom(0,ofGetWidth()), ofRandom(0,ofGetHeight()), (sounds[3].getPosition()*100));
        ofDrawCircle(ofRandom(0,ofGetWidth()), ofRandom(0,ofGetHeight()), (sounds[3].getPosition()*200));
        currentSound++;
    }
    if(sounds[4].isPlaying()){
        ofSetColor(0, 255, 255);
        ofDrawCircle(ofRandom(0,ofGetWidth()), ofRandom(0,ofGetHeight()), (sounds[4].getPosition()*200));
        ofDrawCircle(ofRandom(0,ofGetWidth()), ofRandom(0,ofGetHeight()), (sounds[4].getPosition()*100));
        currentSound++;
    }
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if(key == 'a') {
        sounds[0].play();
    }
    if(key == 's') {
        sounds[1].play();
    }
    if(key == 'd') {
        sounds[2].play();
    }
    if(key == 'f') {
        sounds[3].play();
    }
    if(key == 'g') {
        sounds[4].play();
    }
    if(key == 'h') {
        sounds[5].play();
    }
    if(key == 'j') {
        sounds[6].play();
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
