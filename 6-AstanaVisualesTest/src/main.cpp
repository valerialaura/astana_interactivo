#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
	//ofSetupOpenGL(2560, 800,OF_WINDOW);			// <-------- setup the GL context
	
	

	ofAppGLFWWindow window;
	window.setMultiDisplayFullscreen(true);
	ofSetupOpenGL(&window, 2560, 800, OF_WINDOW);
	ofSetWindowPosition(1280, 20);
	//ofSetWindowPosition(1920, 0);
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}
