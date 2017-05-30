#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	receiver.setup(12345);
}

//--------------------------------------------------------------
void ofApp::update(){
	while (receiver.hasWaitingMessages()) {
		ofxOscMessage m;
		receiver.getNextMessage(m);
		mouse.x = m.getArgAsInt(0);
		mouse.y = m.getArgAsInt(1);

		puntos.resize(10);
		if (m.getNumArgs() > 21) {
			for (int i = 0; i < 10; i++) {
				puntos[i].x = m.getArgAsDouble(i * 2);
				puntos[i].y = m.getArgAsDouble(i * 2 + 1);
			}
		}

		ofBuffer buffer = m.getArgAsBlob(m.getNumArgs() -1);
		if (buffer.size() > 0) {
			if (buffer.size() % sizeof(glm::vec3) == 0) {
				size_t n = buffer.size() / sizeof(glm::vec3);
				vector<glm::vec3> p;
				p.resize(n);
				memcpy(&p[0], buffer.getData(), buffer.size());
				poly.clear();
				poly.addVertices(p);
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofSetColor(255);
	poly.draw();
	ofSetColor(255, 0, 0);
	ofDrawCircle(mouse, 10);
	ofSetColor(ofColor::yellow);
	for (auto&p : puntos) {
		ofDrawCircle(p, 10);
	}
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
