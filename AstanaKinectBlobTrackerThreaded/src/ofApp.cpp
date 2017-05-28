#include "ofApp.h"

#define DEPTH_WIDTH 512
#define DEPTH_HEIGHT 424
#define DEPTH_SIZE DEPTH_WIDTH * DEPTH_HEIGHT


//--------------------------------------------------------------
void ofApp::setup() {

	blobFinder.setup(DEPTH_WIDTH, DEPTH_HEIGHT);
	string bfXml = "blobFinder_settings.xml";
	gui.setup("BlobFinder", bfXml);
	gui.add(bDrawBlobFinder.set("Draw Blob Finder", true));
	gui.add(blobFinder.parameters);
	gui.setSize(300, 400);
	gui.setWidthElements(300);
	gui.loadFromFile(bfXml);
	

	newBlobSound.load("Glass.aiff");
	killBlobSound.load("Morse.aiff");
	movedBlobSound.load("Frog.aiff");

	ofAddListener(blobFinder.newBlobEvent, this, &ofApp::onNewBlobs);
	ofAddListener(blobFinder.killedBlobEvent, this, &ofApp::onKillBlobs);
	ofAddListener(blobFinder.onMoveBlobEvent, this, &ofApp::onMovedBlobs);
	ofAddListener(blobFinder.onScaleBlobEvent, this, &ofApp::onScaledBlobs);
	ofAddListener(blobFinder.onMergeBlobEvent, this, &ofApp::onMergedBlobs);

}
//--------------------------------------------------------------
void ofApp::update() {
	blobFinder.update();
}
//--------------------------------------------------------------
void ofApp::onScaledBlobs() {}
//--------------------------------------------------------------
void ofApp::onMergedBlobs() {}
//--------------------------------------------------------------
void ofApp::onNewBlobs() {
	newBlobSound.play();
}
//--------------------------------------------------------------
void ofApp::onKillBlobs() {
	killBlobSound.play();
}
//--------------------------------------------------------------
void ofApp::onMovedBlobs() {
	movedBlobSound.play();
}
//--------------------------------------------------------------
void ofApp::draw() {
	if (bDrawBlobFinder) {
		blobFinder.draw();
	}
	stringstream ss;
	ss << "fps : " << ofGetFrameRate() << endl;	
	ofDrawBitmapStringHighlight(ss.str(), 20, 20);
	gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
	
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
	
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
	
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
	
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
	
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {
	
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {
	
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {
	
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {
	
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {
	
}
