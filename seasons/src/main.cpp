#include "ofMain.h"
#include "ofApp.h"
/*William Jattin CIM 795 SPRING 2017 UNIVERSITY OF MIAMI */
//========================================================================
int main( ){
	ofSetupOpenGL(1024,768,OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}
