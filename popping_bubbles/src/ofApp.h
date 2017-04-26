#pragma once

#include "ofMain.h"
#include "ofxOpenCV.h"
#include "particle.hpp"
#include "character.hpp"
#include "bullet.hpp"
class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    void audioIn(float * input, int bufferSize, int nChannels);
    
    float loudness;
    
    //adding for beat detection
    float threshold;
    float minimumThreshold;
    float decayRate;
    
    ofSoundStream soundStream;
    int camWidth;
    int camHeight;
    int audiothreshold;
    int score;
    bool shoot;
    bool play;
    ofVideoGrabber cam;
    
    ofxCvColorImage			colorImg;
    
    ofxCvGrayscaleImage 	grayImage;
    ofxCvGrayscaleImage 	grayBg;
    ofxCvGrayscaleImage 	grayDiff;
    
    ofxCvContourFinder 	contourFinder;
    
    float px;
    float py;
    float diameter;
    
    vector<Particle> particles;
    Character character;
    Bullet bullet;
    
};
