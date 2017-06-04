#include "ofApp.h"
#define PATH_AUDIOS "../../../SHARED_DATA/AstanaInstalacionInteractivaAudio"
//#define PATH_AUDIOS "AstanaInstalacionInteractivaAudio"
//#include "ofBitmapFont.h"
//--------------------------------------------------------------
void ofApp::setup(){
	ofSetLogLevel(OF_LOG_VERBOSE);
	blobsMngr = make_shared<AstanaBlobsManager>();
	blobsMngr->setup();
    sndMngr.setup( blobsMngr);
	sndMngr.enableGui();
	
}

//--------------------------------------------------------------
void ofApp::update(){
    
}

//--------------------------------------------------------------
void ofApp::draw(){
	if (!sndMngr.isGuiEnabled()) {
		blobsMngr->draw();
		blobsMngr->drawGui();
	}
        
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
 
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	if (key == ' ') {
		sndMngr.toggleGui();
	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
    
}
