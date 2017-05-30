#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
	ofSetupOpenGL(1024, 768,OF_WINDOW);			// <-------- setup the GL context

	TIME_SAMPLE_ADD_SETUP_HOOKS();
	TIME_SAMPLE_SET_FRAMERATE(60);	//if you want setup() to get automaticall measured,
									//you need to create the instance once before setup() is called;
									//just call any TIME_SAMPLE_* method to do so
	ofRunApp(new ofApp());

}
