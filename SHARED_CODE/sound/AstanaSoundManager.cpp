//
//  AstanaSoundManager.cpp
//  gestionSonido
//
//  Created by Roy Macdonald on 21-05-17.
//
//

#include "AstanaSoundManager.h"


bool isGroup(ofAbstractParameter& p) {
	return (p.type() == typeid(ofParameterGroup).name());
};
string getParamRoute(ofAbstractParameter& p) {
	auto v = p.getGroupHierarchyNames();
	string r;
	for (auto&n : v) {
		r += "/" + n;
	}
	return r;
}
string printGroup(ofParameterGroup& g) {
	stringstream ss;
	for (int i = 0; i < g.size(); i++) {
		if (isGroup(g.get(i))) {
			ss << printGroup(g.getGroup(i));//, nam
		}
		else {
			ss << getParamRoute(g.get(i)) << endl;
		}
	}
	return ss.str();
}
vector<string> getGroupRoutes(ofParameterGroup& g) {
	vector<string> v;
	for (int i = 0; i < g.size(); i++) {
		if (isGroup(g.get(i))) {
			auto gr = getGroupRoutes(g.getGroup(i));
			for (auto& r : gr) {
				v.push_back(r);
			}
		}
		else {
			v.push_back(getParamRoute(g.get(i)));
		}
	}
	return v;
}
//--------------------------------------------------------------
ofParameter<float>* getParamFromRoute(ofParameterGroup& g, string route) {
	auto n = ofSplitString(route, "/", true, true);

	ofParameterGroup* prevGroup = &(g);
	for (int i = 0; i < n.size(); i++) {
		if (prevGroup->contains(n[i])) {
			if (isGroup(prevGroup->get(n[i]))) {
				if (i < n.size() - 1) {
					prevGroup = &(prevGroup->getGroup(n[i]));
				}
			}
			else {
				if (i == n.size() - 1) {
					return &(prevGroup->getFloat(n[i]));
				}
			}
		}
	}
	return nullptr;
}
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
	blobMappingGui.draw();
	
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

	stringstream ss;
	for (auto& sb : soundBlobLinks) {
		ss << sb.first << endl;
		for (auto& l : sb.second) {
			ss << "    " << l.first << "  " << AstanaToString(l.second.blobParam) << endl;
		}
	}
	ss << "Available Params: " << endl;
	for (auto& a : availableParams) {
		ss << a.first << " " << (a.second ? "TRUE" : "FALSE") << endl;
	}
	r = f.getBoundingBox(ss.str(), 0, 0);
	ofDrawBitmapStringHighlight(ss.str(), ofGetWidth() - r.width - 20, ofGetHeight() - r.height - 20);

}
//---------------------------------------------------
void AstanaSoundManager::setup(string folderPath, shared_ptr<AstanaBlobsManager>mng){
	
	load(folderPath);
    setupGui();

	setBlobManager(mng);
	setAvailableSoundParams();
	//if (texturas) {
	//	cout << endl;
	//	cout << printGroup(texturas->gui.getParameter().castGroup()) << endl;
	//	cout << "----------" << endl;
	//	auto gr = getGroupRoutes(texturas->gui.getParameter().castGroup());
	//	for (auto& r : gr) {
	//		cout << r << endl;
	//	}
	//}
//	mixer.print();
}
//---------------------------------------------------
void AstanaSoundManager::setAvailableSoundParams() {
	
	auto addParamGroup = [&](ofParameterGroup& g) {
		auto gr = getGroupRoutes(g);
		for (auto&r : gr) {
			if (availableParams.count(r) == 0) {
				availableParams[r] = true;
			}
		}
	};
	if (texturas) {
		addParamGroup(texturas->volumeGroup);
		addParamGroup(texturas->panGroup);
	}
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
	string bmgXml = "blob_sound_mapping_settings.xml";
	blobMappingGui.setup("Mapping Blob a Sonido", bmgXml);

	auto makeMinMaxParamGroup = [&](string name, AstanaBlobParam p,  float _min, float _max) -> ofParameterGroup {
		ofParameterGroup g;
		g.setName(name);
		if (mapMinVals.count(p)==0) {
			mapMinVals[p].set("min", _min, _min, _max);
			g.add(mapMinVals[p]);
		}
		if (mapMaxVals.count(p)==0) {
			mapMaxVals[p].set("max", _max, _min, _max);
			g.add(mapMaxVals[p]);
		}
		return g;
	};
	blobMappingGui.add(makeMinMaxParamGroup("Blob Center X",ASTANA_BLOB_CENTER_X, 0, 2000));
	blobMappingGui.add(makeMinMaxParamGroup("Blob Center Y",ASTANA_BLOB_CENTER_Y, 0, 2000));
	blobMappingGui.add(makeMinMaxParamGroup("Blob Area",    ASTANA_BLOB_AREA, 0, 2000));
	blobMappingGui.add(makeMinMaxParamGroup("Blob Vel X",   ASTANA_BLOB_VEL_X, 0, 200));
	blobMappingGui.add(makeMinMaxParamGroup("Blob Vel Y",   ASTANA_BLOB_VEL_Y, 0, 200));

	blobMappingGui.loadFromFile(bmgXml);


    float x = 20;
    for (auto&g: groups) {
        g.second->gui.setPosition(x, 40);
        x += g.second->gui.getWidth() + 20;
    }
	blobMappingGui.setPosition(x, 40);
	x += blobMappingGui.getWidth() + 20;

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
void AstanaSoundManager::playSample(const string& folderName, const string& sampleName) {
	if (groups.count(folderName)) {
		auto p = groups[folderName]->getPlayer(sampleName);
		if (p) {
			p->play();
		}
	}
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
	if (mng) {
		blobManager = mng;
		//blobListeners.clear();
		//ofAddListener(blobManager->newBlobEvent    , this, &AstanaSoundManager::onNewBlobs);
		//ofAddListener(blobManager->killedBlobEvent , this, &AstanaSoundManager::onKillBlobs);
		//ofAddListener(blobManager->onMoveBlobEvent , this, &AstanaSoundManager::onMovedBlobs);
		//ofAddListener(blobManager->onScaleBlobEvent, this, &AstanaSoundManager::onScaledBlobs);
		//ofAddListener(blobManager->onMergeBlobEvent, this, &AstanaSoundManager::onMergedBlobs);

		blobListeners.push_back(blobManager->newBlobEvent.newListener(this, &AstanaSoundManager::onNewBlobs));
		blobListeners.push_back(blobManager->killedBlobEvent.newListener(this, &AstanaSoundManager::onKillBlobs));
		blobListeners.push_back(blobManager->onMoveBlobEvent.newListener(this, &AstanaSoundManager::onMovedBlobs));
		blobListeners.push_back(blobManager->onScaleBlobEvent.newListener(this, &AstanaSoundManager::onScaledBlobs));
		blobListeners.push_back(blobManager->onMergeBlobEvent.newListener(this, &AstanaSoundManager::onMergedBlobs));
	}
	else {
		cout << "BLOB MANGER ES NULL!!!" << endl;
	}
}
//---------------------------------------------------
void AstanaSoundManager::onNewBlobs() {
	//playNextTrack();
//	cout << "new blobs" << endl;
	mutex.lock();
	getBlobsCollection()[ASTANA_NEW_BLOBS] = blobManager->getNewBlobs();
	mutex.unlock();

	for (auto& b : getNewBlobs()){//blobManager->getNewBlobs()) {
		addLink(b->label, ASTANA_BLOB_CENTER_X);
		addLink(b->label, ASTANA_BLOB_CENTER_Y);
	}
}
//---------------------------------------------------
void AstanaSoundManager::onKillBlobs() {
	//cout << "killed blobs" << endl;
//
	mutex.lock();
	getBlobsCollection()[ASTANA_KILLED_BLOBS] = blobManager->getKilledBlobs();
	mutex.unlock();
	for (auto& b : getKilledBlobs()) {
		removeLink(b->label);
		removeLink(b->label);
	}
}
//---------------------------------------------------
void AstanaSoundManager::onMovedBlobs() {
	//cout << "moved blobs" << endl;
	mutex.lock();
	getBlobsCollection()[ASTANA_MOVED_BLOBS] = blobManager->getMovedBlobs();
	mutex.unlock();
	updateSoundBlobLinks(ASTANA_MOVED_BLOBS);
}
//---------------------------------------------------
void AstanaSoundManager::onScaledBlobs() {
	mutex.lock();
	getBlobsCollection()[ASTANA_SCALED_BLOBS] = blobManager->getScaledBlobs();
	mutex.unlock();
	updateSoundBlobLinks(ASTANA_SCALED_BLOBS);
}
//---------------------------------------------------
void AstanaSoundManager::onMergedBlobs() {
	mutex.lock();
	getBlobsCollection()[ASTANA_MERGED_BLOBS] = blobManager->getMergedBlobs();
	mutex.unlock();
	updateSoundBlobLinks(ASTANA_MERGED_BLOBS);
}
//---------------------------------------------------
void AstanaSoundManager::setAvailableParamUse(const string& paramName, bool bUse) {
	if (availableParams.count(paramName)) {
		availableParams[paramName] = bUse;
	}
}
//---------------------------------------------------
string AstanaSoundManager::getNextAvailableParamName() {
	for (auto& p : availableParams) {
		if (p.second) {
			return p.first;
		}
	}
	return "";
}
//---------------------------------------------------
bool AstanaSoundManager::hasNextAvailableParamName() {
	return getNextAvailableParamName() != "";
}
//---------------------------------------------------
bool AstanaSoundManager::addLink(unsigned int label, AstanaBlobParam blobParam) {
		if (hasNextAvailableParamName()) {
			bool bFound = false;
			if (soundBlobLinks.count(label)) {
				for (auto& p : soundBlobLinks[label]) {
					if (p.second.blobParam == blobParam) {
						bFound = true;
						break;
					}
				}
			}
			if (!bFound) {
				string n = getNextAvailableParamName();
				auto p = getParamFromRoute(texturas->gui.getParameter().castGroup(), n);
				if (p) {
					soundBlobLinks[label][n].blobParam = blobParam;
					soundBlobLinks[label][n].soundParam.makeReferenceTo( *p);
					setAvailableParamUse(n, false);
					return true;
				}
			}
		}
	return false;
}
//---------------------------------------------------
bool AstanaSoundManager::removeLink(unsigned int label){// AstanaBlobParam blobParam) {
	if (soundBlobLinks.count(label)) {
		for (auto& b : soundBlobLinks[label]) {
			setAvailableParamUse(b.first, true);
		}
		soundBlobLinks.erase(label);
		return true;
	}
	return false;
}
//---------------------------------------------------
void AstanaSoundManager::updateSoundBlobLinks(AstanaBlobType t) {
	auto mapParam = [&](float val, AstanaBlobParam& p)->float {
		if (mapMinVals.count(p) > 0 && mapMaxVals.count(p)) {
			return ofMap(val, mapMinVals[p], mapMaxVals[p], 0,1);
		}
		return val;
	};

	for (auto& b : getBlobs(t)) {
		if (soundBlobLinks.count(b->label)) {
			for (auto& l : soundBlobLinks[b->label]) {
				l.second.soundParam = mapParam(b->getParam(l.second.blobParam), l.second.blobParam);
			}
		}
	}
}
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