#pragma once

#include "ofMain.h"
#include "ofxKinect.h"
#include "ofxOpenCv.h"
//#include "ofxGui.h"
#include "ofxBox2d.h"
/*William Jattin CIM 795 SPRING 2017 UNIVERSITY OF MIAMI */
// ------------------------------------------------- a simple extended box2d circle
class CustomParticle : public ofxBox2dCircle {
    
public:
    CustomParticle() {
    }
    ofColor color;
    void draw() {
        float radius = getRadius();
        
        glPushMatrix();
        glTranslatef(getPosition().x, getPosition().y, 0);
        
        ofSetColor(color.r, color.g, color.b);
        ofFill();
        ofCircle(0, 0, radius);
        
        glPopMatrix();
        
    }
};


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
    void draw();
    void createSnow();
    void destroySnow();

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
    ofImage ball;
    // kinect
    ofxKinect kinect;
    ofParameter <int> nearClip;
    ofParameter <int> farClip;
    ofParameter <bool> bFlipX, bFlipY;
    
    ofVideoPlayer videoPlayer;
    
    bool bUseLiveKinect;
    bool bbool;
    bool bloaded;
    bool sloaded;
    // cv //
    ofParameter<int> numDilatePasses;
    ofParameter<int> blurAmount;
    ofParameter<int> threshold;
    ofParameter<float> minSize, maxSize;
    
    ofPixels grayPixels;
    ofxCvColorImage colorCv;
    ofxCvGrayscaleImage grayCv;
    ofxCvGrayscaleImage processedCv;
    ofxCvGrayscaleImage prevFrame;
    ofxCvGrayscaleImage historyCv;
    ofxCvContourFinder finder;
    ofImage bg;
    
    ofParameter<int> contourPolySpacing;
    ofParameter<int> contourSmoothing;
    
    ofParameter<int> minPixToActivateBox;
    
    vector< ofPolyline > contours;
    
    int incBlobId = 1;
    
    float                                   px, py;
    bool                                    bDrawLines;
    bool                                    bMouseForce;
    
    int                                  mode;
    
    ofxBox2d                                box2d;			  //	the box2d world
    ofPolyline                              drawing,drawing1, drawing2, drawing3, drawing4, drawing5 ;		  //	we draw with this first
    ofxBox2dEdge                            edgeLine,edgeLine1, edgeLine2, edgeLine3, edgeLine4, edgeLine5;		  //	the box2d edge/line shape (min 2 points)

    CustomParticle                          beachball;
    ofxBox2dRect                            beachbounds;
    vector		<shared_ptr<ofxBox2dCircle> >	circles;		  //	default box2d circles
    vector		<shared_ptr<ofxBox2dCircle> >	bubbles;		  //	default box2d circles to add to contour
    vector		<shared_ptr<ofxBox2dRect> >		boxes;			  //	defalut box2d rects
    vector		<shared_ptr<CustomParticle> >    customParticles;  //	this is a custom particle the extends a cirlce
    
};
