#include "ofApp.h"
#define PATH_AUDIOS "../../../SHARED_DATA/AstanaInstalacionInteractivaAudio"
//#define PATH_AUDIOS "AstanaInstalacionInteractivaAudio"
//#include "ofBitmapFont.h"
//--------------------------------------------------------------
void ofApp::setup(){
	ofSetLogLevel(OF_LOG_VERBOSE);
	//ofFileDialogResult r = ofSystemLoadDialog("Cargar carpeta audios");
	//if (r.bSuccess) {
	//	player.load(r.getPath());
	//	//player.connectTo(mixer);
	//	ofSoundStreamSettings settings;
	//	settings.numInputChannels = 0;
	//	settings.numOutputChannels = 2;
	//	settings.numBuffers = 1;
	//	settings.bufferSize = 512;
	//	settings.sampleRate = 48000;
	//	settings.setOutListener(&mixer);
	//	stream.setup(settings);
	//	stream.setOutput(&player);
	//	player.play();
	//}
	ofFileDialogResult r = ofSystemLoadDialog("Cargar carpeta audios", true);
    if (r.bSuccess) {
        cout << "path audios relative: " << ofFilePath::makeRelative(ofToDataPath("",true), r.getPath()) << endl;
      sndMngr.setup(r.getPath());
    }
//    sndMngr.setup(PATH_AUDIOS);
	sndMngr.enableGui();
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
