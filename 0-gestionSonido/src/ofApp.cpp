#include "ofApp.h"
#define PATH_AUDIOS "../../../SHARED_DATA/AstanaInstalacionInteractivaAudio"
//#define PATH_AUDIOS "AstanaInstalacionInteractivaAudio"
#include "ofBitmapFont.h"
//--------------------------------------------------------------
void ofApp::setup(){
    //*
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofFileDialogResult r = ofSystemLoadDialog("Cargar carpeta audios", true);
    if (r.bSuccess) {
        cout << "path audios relative: " << ofFilePath::makeRelative(ofToDataPath("",true), r.getPath()) << endl;
        sndMngr.setup(r.getPath());
    }
    //*/
//    sndMngr.setup(PATH_AUDIOS);
}

//--------------------------------------------------------------
void ofApp::update(){
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    sndMngr.drawGui();
    
    stringstream s;
    s << "Tecla 1 : activa CumparsitaCOmbo\n";
    s << "Tecla 2 : activa ElDiaQueMeQuieras\n";
    s << "Tecla 3 : activa Nonino\n";
    s << "Tecla 4 : activa Sueltos\n";
    s << "[ barra espacio ]  trigger siguiente del grupo actual.\n";
    s << endl;
    s << "Al activar un grupo se apagara el que este andando y \nse iniciaran las texturas correspondientes.\n";
    s << "Todos los parametros que se ven el la GUI pueden ser\nanclados dinamicamente a OSC.";
    
    ofBitmapFont f;
    auto r = f.getBoundingBox(s.str(), 0, 0);
    ofDrawBitmapStringHighlight(s.str(), 300, ofGetHeight() - r.height - 20);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if(key == ' '){
    sndMngr.playNextTrack();
//        if(sndMngr.getTexturas()->isPlaying()){
//            sndMngr.getTexturas()->pause();
//        }else{
//            sndMngr.getTexturas()->playAll();
//        }
//    }else if(key == OF_KEY_RETURN){
//        sndMngr.getTexturas()->printVols();
//    }else if(key == '1'){
//        sndMngr.playNextInGroup("Texturas");
//    }else if(key == '2'){
//        sndMngr.playNextInGroup("CumparsitaCOmbo");
//    }else if(key == '3'){
//        sndMngr.playNextInGroup("ElDiaQueMeQuieras");
//    }else if(key == '4'){
//        sndMngr.playNextInGroup("Nonino");
//    }else if(key == '5'){
//        sndMngr.playNextInGroup("Sueltos");
    }else if(key == '1'){
        sndMngr.setActiveGroup("CumparsitaCOmbo");
    }else if(key == '2'){
        sndMngr.setActiveGroup("ElDiaQueMeQuieras");
    }else if(key == '3'){
        sndMngr.setActiveGroup("Nonino");
    }else if(key == '4'){
        sndMngr.setActiveGroup("Sueltos");
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
