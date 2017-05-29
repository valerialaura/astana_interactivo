#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	receiver.setup();


}


//--------------------------------------------------------------
void ofApp::draw() {
	receiver.draw();
	receiver.drawGui();
}
