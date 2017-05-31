#include "ofApp.h"

#define DEPTH_WIDTH 512
#define DEPTH_HEIGHT 424


//--------------------------------------------------------------
void ofApp::setup() {

	blobFinder = make_shared<AstanaKinectBlobFinder>();
	blobFinder->setup();
	sender.setup(blobFinder);

}
//--------------------------------------------------------------
void ofApp::update() {
	blobFinder->update();
}
//--------------------------------------------------------------
void ofApp::draw() {
	blobFinder->draw();
	stringstream ss;
	ss << "fps : " << ofGetFrameRate() << endl;	
	ofDrawBitmapStringHighlight(ss.str(), DEPTH_WIDTH + 40, 20);

	sender.drawGui();
}
