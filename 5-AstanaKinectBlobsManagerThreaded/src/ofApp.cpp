#include "ofApp.h"
#include "AstanaDraw.h"



//--------------------------------------------------------------
void ofApp::setup() {

	blobManager.setup();

	//newBlobSound.load("Glass.aiff");
	//killBlobSound.load("Morse.aiff");
	//movedBlobSound.load("Frog.aiff");

	ofAddListener(blobManager.newBlobEvent, this, &ofApp::onNewBlobs);
	ofAddListener(blobManager.killedBlobEvent, this, &ofApp::onKillBlobs);
	ofAddListener(blobManager.onMoveBlobEvent, this, &ofApp::onMovedBlobs);
	ofAddListener(blobManager.onScaleBlobEvent, this, &ofApp::onScaledBlobs);
	ofAddListener(blobManager.onMergeBlobEvent, this, &ofApp::onMergedBlobs);

}
//--------------------------------------------------------------
void ofApp::update() {
}
//--------------------------------------------------------------
void ofApp::onScaledBlobs() {}
//--------------------------------------------------------------
void ofApp::onMergedBlobs() {}
//--------------------------------------------------------------
void ofApp::onNewBlobs() {
	//newBlobSound.play();
}
//--------------------------------------------------------------
void ofApp::onKillBlobs() {
	//killBlobSound.play();
}
//--------------------------------------------------------------
void ofApp::onMovedBlobs() {
	//movedBlobSound.play();
}
//--------------------------------------------------------------
void ofApp::draw() {
	blobManager.draw();
	blobManager.drawGui();
	stringstream ss;
	ss << "fps : " << ofGetFrameRate() << endl;	
	ofDrawBitmapStringHighlight(ss.str(), 20, 20);

}
