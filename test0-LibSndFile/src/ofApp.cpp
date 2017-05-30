#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofFileDialogResult r = ofSystemLoadDialog();
	if (r.bSuccess) {
		if (player.load(r.getPath())) {
			ofSoundStreamSettings settings;
			settings.sampleRate = 48000;
			settings.numInputChannels = 0;
			settings.numOutputChannels = 2;
			settings.numBuffers = 1;
			settings.bufferSize = 128;
			ss.setup(settings);
			ss.setOutput(player);
			player.play();
		}
	}

}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

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
