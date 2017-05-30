#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	sender.setup("localhost", 12345);
	
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	ofSetColor(ofColor::white);
	line.draw();
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
	line.addVertex(x, y, 0);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	line.clear();
	line.addVertex(x, y,0);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	auto& p = line.getVertices();
	if (p.size() > 0) {
		ofxOscMessage m;
		m.setAddress("/");
		m.addIntArg(x);
		m.addIntArg(y);
		puntos.resize(10);
		for (int i = 0; i < 10; i++) {
			puntos[i].set(ofRandomWidth(), ofRandomHeight());
			m.addDoubleArg(puntos[i].x);
			m.addDoubleArg(puntos[i].y);
		}

		ofBuffer buffer;
		buffer.allocate(p.size() * sizeof(glm::vec3));
		memcpy(buffer.getData(), &p[0], buffer.size());
		m.addBlobArg(buffer);

		sender.sendMessage(m);
	}
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
