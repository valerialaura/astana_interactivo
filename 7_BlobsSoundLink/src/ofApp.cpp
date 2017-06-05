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


	// aqui se registran los callbacks
	ofAddListener(blobsMngr->newBlobEvent, this, &ofApp::onNewBlobs);
	ofAddListener(blobsMngr->killedBlobEvent, this, &ofApp::onKillBlobs);
	ofAddListener(blobsMngr->onMoveBlobEvent, this, &ofApp::onMovedBlobs);
	ofAddListener(blobsMngr->onScaleBlobEvent, this, &ofApp::onScaledBlobs);
	ofAddListener(blobsMngr->onMergeBlobEvent, this, &ofApp::onMergedBlobs);



}
//--------------------------------------------------------------
void ofApp::onNewBlobs(){
	//sndMngr.playNextTrack();// esta llamada va a darle play al siguiente track en la en grupo activo.

	// usar las siguientes llamadas para activar cada grupo.
	//sndMngr.activarCumparsita();
	//sndMngr.activarElDiaQueMeQuieras();
	//sndMngr.activarNonino();
	//sndMngr.activarSueltos();

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

		sndMngr.playNextTrack();
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
