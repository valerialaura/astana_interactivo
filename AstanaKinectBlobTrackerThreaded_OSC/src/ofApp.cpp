#include "ofApp.h"

#define DEPTH_WIDTH 512
#define DEPTH_HEIGHT 424
#define DEPTH_SIZE DEPTH_WIDTH * DEPTH_HEIGHT


//--------------------------------------------------------------
void ofApp::setup() {

	blobFinder = make_shared<AstanaKinectBlobFinder>();
	blobFinder->setup(DEPTH_WIDTH, DEPTH_HEIGHT);
	string bfXml = "blobFinder_settings.xml";
	gui.setup("BlobFinder", bfXml);
	gui.add(bDrawBlobFinder.set("Draw Blob Finder", true));
	gui.add(blobFinder->parameters);
	gui.setSize(300, 400);
	gui.setPosition(DEPTH_WIDTH + 40, 40);
	gui.setWidthElements(300);
	gui.loadFromFile(bfXml);

	sender.setup(blobFinder);

	
}
//--------------------------------------------------------------
void ofApp::update() {
	blobFinder->update();
}
//--------------------------------------------------------------
void ofApp::draw() {
	if (bDrawBlobFinder) {
		blobFinder->draw();
	}
	stringstream ss;
	ss << "fps : " << ofGetFrameRate() << endl;	
	ofDrawBitmapStringHighlight(ss.str(), DEPTH_WIDTH + 40, 20);

	gui.draw();
	sender.drawGui();
}
