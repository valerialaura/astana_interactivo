//
//  AstanaSoundManager.cpp
//  gestionSonido
//
//  Created by Roy Macdonald on 21-05-17.
//
//

#include "AstanaSoundManager.h"
//---------------------------------------------------
AstanaSoundManager::AstanaSoundManager():activeGroup(""){}
//---------------------------------------------------
AstanaSoundManager::~AstanaSoundManager(){
    soundStream.close();
}
//---------------------------------------------------
void AstanaSoundManager::drawGui(ofEventArgs&){
    for(auto& g: groups){
        g.second->gui.draw();
    }
    gui.draw();

	stringstream s;
	s << "Tecla 1 : activa CumparsitaCOmbo\n";
	s << "Tecla 2 : activa ElDiaQueMeQuieras\n";
	s << "Tecla 3 : activa Nonino\n";
	s << "Tecla 4 : activa Sueltos\n";
	s << "[ flecha derecha -> ]  trigger siguiente del grupo actual.\n";
	s << endl;
	s << "Al activar un grupo se apagara el que este andando y \nse iniciaran las texturas correspondientes.\n";
	s << "Todos los parametros que se ven el la GUI pueden ser\nanclados dinamicamente a OSC.";

	ofBitmapFont f;
	auto r = f.getBoundingBox(s.str(), 0, 0);
	ofDrawBitmapStringHighlight(s.str(), 0.5 * (ofGetWidth() - r.width), ofGetHeight() - r.height - 20);
}
//---------------------------------------------------
void AstanaSoundManager::setup(string folderPath, shared_ptr<AstanaBlobsManager>mng){
	
	load(folderPath);
    setupGui();

	setBlobManager(mng);
	mixer.print();
}
//---------------------------------------------------
void AstanaSoundManager::setupGui(){
	listeners.clear();
	string xmlName = "astana_sound_manager_settings.xml";
    gui.setup("OPCIONES GLOBALES", xmlName);
    gui.setHeaderBackgroundColor(ofColor::blue);
	gui.add(mixer.masterVol);
	gui.add(fadeEscenaDuration.set("duracion fade escenas. segs.", 1, 0, 10));
	ofParameter<void> stopAllP("STOP ALL");
	gui.add(stopAllP);
	listeners.push_back(stopAllP.newListener(this, &AstanaSoundManager::stopAll));
    gui.loadFromFile(xmlName);

    
    float x = 20;
    for (auto&g: groups) {
        g.second->gui.setPosition(x, 40);
        x += g.second->gui.getWidth() + 20;
    }
    
    gui.setPosition(20, ofGetHeight() - (20+ gui.getShape().height));
    if(x > ofGetWidth()){
        ofSetWindowShape(x, ofGetHeight());
    }
    
}
//---------------------------------------------------
void AstanaSoundManager::enableGui(bool e) {
	if (bGuiEnabled != e) {
		bGuiEnabled = e;
		if (e) {
			guiListeners.push_back(ofEvents().draw.newListener(this, &AstanaSoundManager::drawGui));
			guiListeners.push_back(ofEvents().keyReleased.newListener(this, &AstanaSoundManager::keyReleased));
		}
		else {
			guiListeners.clear();
		}
	}
}
//---------------------------------------------------
void AstanaSoundManager::disableGui() {
	enableGui(false);
}
//---------------------------------------------------
void AstanaSoundManager::toggleGui() {
	enableGui(!bGuiEnabled);
}
//---------------------------------------------------
void AstanaSoundManager::stopAll(){
    std::function<void()> cb;
    for(auto& g: groups){
        g.second->fadeOutAll(cb);
    }
}
//---------------------------------------------------
void AstanaSoundManager::playNextTrack(){
    if (groups.count(activeGroup)) {
        groups[activeGroup]->playNext();
    }
}
//---------------------------------------------------
void AstanaSoundManager::setActiveGroup(string groupName){
    if(texturas){
        if (groupName == texturas->getName()) {
            cout << " set active group no puede ser texturas" << endl;
            return;
        }
    }
    if(activeGroup != groupName){
        string tempGroup = activeGroup;
        activeGroup = groupName;
        if (groups.count(tempGroup)) {
            groups[tempGroup]->highlightGui(false);
            if(groups[tempGroup]->isPlaying()){
                groups[tempGroup]->fadeOutAll([this](){startActiveGroup();});
            }else{
                startActiveGroup();
            }
        }else{
            startActiveGroup();
        }
    }
}
//---------------------------------------------------
void AstanaSoundManager::startActiveGroup(){
    if (texturas && groups.count(activeGroup)) {
        groups[activeGroup]->highlightGui(true);
        if (typeid(*(groups[activeGroup].get())).name() == typeid(AstanaSoundIntervencion).name()){
            groups[activeGroup]->play();
        }else{
            ofParameterGroup& texGrp = groups[activeGroup]->texturasValidasGroup;
            for (int i = 0; i< texGrp.size();i++) {
                auto p = texturas->getPlayer(texGrp.getName(i));
                if (p) {
                    if (texGrp.getBool(i)) {
                        p->play();
                    }else{
                        p->fadeOut();
                    }
                }
            }
        }
    }
}
//---------------------------------------------------
void AstanaSoundManager::playNextInGroup(string groupName){
    if(groups.count(groupName)){
        groups[groupName]->playNext();
    }
}
//---------------------------------------------------
void AstanaSoundManager::load(string folderPath){
    this->folderPath = folderPath;
    groups.clear();
    texturas = addGroup<AstanaSoundTextura>("Texturas");
    addGroup<AstanaSoundIntervencion>("CumparsitaCOmbo");
    addGroup<AstanaSoundSecuencia>("ElDiaQueMeQuieras");
    addGroup<AstanaSoundSecuencia>("Nonino");
    addGroup<AstanaSoundGroup>("Sueltos");
    
    ofSoundStreamSettings settings;
    settings.numInputChannels =0;
    settings.numOutputChannels =2;
    settings.numBuffers =1;
    settings.bufferSize = 256;
    settings.sampleRate = 48000;
	soundStream.setup(settings);
	soundStream.setOutput(&mixer);
}
//---------------------------------------------------
void AstanaSoundManager::close() {
	soundStream.close();
}
//---------------------------------------------------
void AstanaSoundManager::setBlobManager(shared_ptr<AstanaBlobsManager>mng) {
	if (mng != nullptr) {
		blobManager = mng;
		blobListeners.clear();
		blobListeners.push_back(blobManager->newBlobEvent.newListener(this, &AstanaSoundManager::onNewBlobs));
		blobListeners.push_back(blobManager->killedBlobEvent.newListener(this, &AstanaSoundManager::onKillBlobs));
		blobListeners.push_back(blobManager->onMoveBlobEvent.newListener(this, &AstanaSoundManager::onMovedBlobs));
		blobListeners.push_back(blobManager->onScaleBlobEvent.newListener(this, &AstanaSoundManager::onScaledBlobs));
		blobListeners.push_back(blobManager->onMergeBlobEvent.newListener(this, &AstanaSoundManager::onMergedBlobs));
	}
}
//---------------------------------------------------
//int AstanaSoundManager::findSoundBlobLinkByLabel(unsigned int label) {
//	for (int i = 0; i < soundBlobLinks.size(); i++) {
//		if (soundBlobLinks[i].blobLabel == label) {
//			return i;
//		}
//	}
//	return -1;
//}
//---------------------------------------------------
void AstanaSoundManager::onNewBlobs() {
	for (auto& b : blobManager->getNewBlobs()) {
		if (soundBlobLinks.count(b->label) == 0) {
			
		}
	}
}
//---------------------------------------------------
void AstanaSoundManager::onKillBlobs() {}
//---------------------------------------------------
void AstanaSoundManager::onMovedBlobs() {}
//---------------------------------------------------
void AstanaSoundManager::onScaledBlobs() {}
//---------------------------------------------------
void AstanaSoundManager::onMergedBlobs() {}
//---------------------------------------------------
template<typename T>
shared_ptr<T> AstanaSoundManager::addGroup(string groupFolder){
    ofFile f(folderPath+ofFilePath::addLeadingSlash(groupFolder));
    string groupName = f.getBaseName();
    if(groups.count(groupName) == 0){
        groups[groupName] = make_shared<T>();
        auto p = static_pointer_cast<T>(groups[groupName]);
        p->setName(groupName);
        p->setManager(this);
        p->setup(f.getAbsolutePath());
		p->getMixer().connectTo(mixer);
		/*if (p->getOutput()) {
			p->getOutput()->connectTo(mixer);
		}else {
			cout << groupName << "no se puede conectar a mixer. su output es null"<<endl;
		}*/
		return p;
    }else{
        cout << "ya existe el grupo " << groupName << endl;
    }
    return nullptr;
}

void AstanaSoundManager::keyReleased(ofKeyEventArgs& k) {
	if (k.key == OF_KEY_RIGHT) {
		playNextTrack();
	}
	else if (k.key == '1') {
		setActiveGroup("CumparsitaCOmbo");
	}
	else if (k.key == '2') {
		setActiveGroup("ElDiaQueMeQuieras");
	}
	else if (k.key == '3') {
		setActiveGroup("Nonino");
	}
	else if (k.key == '4') {
		setActiveGroup("Sueltos");
	}
}