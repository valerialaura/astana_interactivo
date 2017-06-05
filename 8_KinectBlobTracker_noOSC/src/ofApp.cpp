#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	blobManager.setup();
	bDrawGui = true;

	// aqui se registran los callbacks
	ofAddListener(blobManager.newBlobEvent, this, &ofApp::onNewBlobs);
	ofAddListener(blobManager.killedBlobEvent, this, &ofApp::onKillBlobs);
	ofAddListener(blobManager.onMoveBlobEvent, this, &ofApp::onMovedBlobs);
	ofAddListener(blobManager.onScaleBlobEvent, this, &ofApp::onScaledBlobs);
	ofAddListener(blobManager.onMergeBlobEvent, this, &ofApp::onMergedBlobs);

}
//--------------------------------------------------------------
void ofApp::onNewBlobs(){
}
//--------------------------------------------------------------
void ofApp::onKillBlobs(){
}
//--------------------------------------------------------------
void ofApp::onMovedBlobs(){
}
//--------------------------------------------------------------
void ofApp::onScaledBlobs(){
}
//--------------------------------------------------------------
void ofApp::onMergedBlobs(){// este callback es llamado cuando dos blobs se unen.
}
//--------------------------------------------------------------
void ofApp::update() {
	
}
//--------------------------------------------------------------
void ofApp::draw() {
	if (bDrawGui) {
		blobManager.draw();
		blobManager.drawGui();
	}

	//acceso a blobs
	//blobManager.getAllBlobs();
	//blobManager.getNewBlobs();
	//blobManager.getMovedBlobs();
	//blobManager.getScaledBlobs();
	//blobManager.getMergedBlobs();
	//blobManager.getKilledBlobs();
	//blobManager.getGhostBlobs();


}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){}
//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	if(key == 'g'){
		bDrawGui ^= true;
	}
}
//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){}
//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){}
//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){}
//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){}
//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){}
//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){}
//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){}
//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){}
//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){}