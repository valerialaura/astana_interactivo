//
//  AstanaSoundManager.cpp
//  gestionSonido
//
//  Created by Roy Macdonald on 21-05-17.
//
//

#include "AstanaSoundManager.h"
#include <random>

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
AstanaSoundManager::AstanaSoundManager() :activeGroup("") {
	startThread();
}
//---------------------------------------------------
AstanaSoundManager::~AstanaSoundManager() {
#ifdef ASTANA_PRINT_DEBUG
	ofBufferToFile("fileBuffer.txt", fileBuffer);
#endif
	close();
	toBlobsUpdate.close();
	fromBlobsUpdate.close();
	waitForThread(true);
}
//---------------------------------------------------
void AstanaSoundManager::drawGui(ofEventArgs&) {
	for (auto& g : groups) {
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
	s << "Estas teclas solo haran esto cuando se vea esta GUI. Son para debuguear principalmente\n";
	//s << "Todos los parametros que se ven el la GUI pueden ser\nanclados dinamicamente a OSC.";

	ofBitmapFont f;
	auto r = f.getBoundingBox(s.str(), 0, 0);
	ofDrawBitmapStringHighlight(s.str(), 20, gui.getShape().getY() - r.height - 20, ofColor::fromHex(0xec008c), ofColor::black);

	stringstream ss;
	for (auto& sb : soundBlobLinks) {
		ss << sb.first << endl;
		for (auto& l : sb.second) {
			ss << "    " << l.first << " I: " << l.second;
			auto p = texturas->getPlayer(l.first);
			if(p){
				ss << " V: " << p->getVolume(l.second);
				ss << " P: " << p->getPan(l.second);
				ss << " S: " << p->getSpeed(l.second);
			}
			ss << endl;
		}
	}
	//ss << "Available Params: " << endl;
	//
	//for (auto& a : availableParams) {
	//	ss << a.first << " " << (a.second ? "TRUE" : "FALSE") << endl;
	//}
	r = f.getBoundingBox(ss.str(), 0, 0);
	ofDrawBitmapStringHighlight(ss.str(), 0.5 * (ofGetWidth() - r.width), ofGetHeight() - r.height - 20);
	//ofDrawBitmapStringHighlight(ss.str(), ofGetWidth() - r.width - 20, ofGetHeight() - r.height - 20);
	if (blobManager) {
		blobManager->drawDebug();

	}

}
//---------------------------------------------------
void AstanaSoundManager::setup(shared_ptr<AstanaBlobsManager>mng) {
	string folderPath;
	ofXml xml;
	string xmlPath = "path_carpeta_audio.xml";
	if (!xml.load(xmlPath)) {
		ofFileDialogResult r = ofSystemLoadDialog("Seleccionar carpeta audios", true);
		if (r.bSuccess) {
			folderPath = r.getPath();
			xml.appendChild("Astana").appendChild("path_audio").set(r.getPath());
			xml.save(xmlPath);
		}
	}
	else {
		auto path = xml.getChild("Astana");
		if (path) {
			folderPath = path.getChild("path_audio").getValue();
		}
	}
	load(folderPath);
	setupGui();

	setBlobManager(mng);
//	setAvailableSoundParams();
}
//---------------------------------------------------
void AstanaSoundManager::setupGui() {
	listeners.clear();
	string xmlName = "astana_sound_manager_settings.xml";
	gui.setup("OPCIONES GLOBALES", xmlName);
	gui.setHeaderBackgroundColor(ofColor::blue);
	gui.add(mixer.masterVol);
	gui.add(fadeEscenaDuration.set("duracion fade escenas. segs.", 1, 0, 10));
	gui.add(smoothingParams.set("Smooth cambio sonidos", 0.8, 0, 1));


	ofParameter<void> stopAllP("STOP ALL");
	gui.add(stopAllP);
	listeners.push_back(stopAllP.newListener(this, &AstanaSoundManager::stopAll));
	gui.loadFromFile(xmlName);
	string bmgXml = "blob_sound_mapping_settings.xml";
	blobMappingGui.setup("Mapping Blob a Sonido", bmgXml);

	auto makeMinMaxParamGroup = [&](string name, AstanaBlobParam p, float _min, float _max) -> ofParameterGroup {
		ofParameterGroup g;
		g.setName(name);
		if (mapMinVals.count(p) == 0) {
			mapMinVals[p].set("min", _min, _min, _max);
			g.add(mapMinVals[p]);
		}
		if (mapMaxVals.count(p) == 0) {
			mapMaxVals[p].set("max", _max, _min, _max);
			g.add(mapMaxVals[p]);
		}
		return g;
	};
	blobMappingGui.add(makeMinMaxParamGroup("Blob Center X", ASTANA_BLOB_CENTER_X, 0, 2000));
	blobMappingGui.add(makeMinMaxParamGroup("Blob Center Y", ASTANA_BLOB_CENTER_Y, 0, 2000));
	blobMappingGui.add(makeMinMaxParamGroup("Blob Area", ASTANA_BLOB_AREA, 0, 2000));
//	blobMappingGui.add(makeMinMaxParamGroup("Blob Vel X", ASTANA_BLOB_VEL_X, 0, 200));
	//blobMappingGui.add(makeMinMaxParamGroup("Blob Vel Y", ASTANA_BLOB_VEL_Y, 0, 200));

	ofParameterGroup usarGroup;
	usarGroup.setName("Propiedades blobs a mapear");
	usarGroup.add(bUsarCentroX.set("Usar Centro X", true));
	usarGroup.add(bUsarCentroY.set("Usar Centro Y", true));
	usarGroup.add(bUsarArea.set(   "Usar Area", true));
	//usarGroup.add(bUsarVelX.set(   "Usar Vel X", true));
	//usarGroup.add(bUsarVelY.set(   "Usar Vel Y", true));
	//usarGroup.add(vResetearLinks.set("Resetear Enlace Sonido a Blob"));
	//listeners.push_back(vResetearLinks.newListener(this, &AstanaSoundManager::clearSoundBlobLinks));

	blobMappingGui.add(usarGroup);
	blobMappingGui.loadFromFile(bmgXml);


	float x = 20;
	for (auto&g : groups) {
		g.second->gui.setPosition(x, 40);
		x += g.second->gui.getWidth() + 20;
	}
	blobMappingGui.setPosition(x, 40);
	x += blobMappingGui.getWidth() + 20;

	gui.setPosition(20, ofGetHeight() - (20 + gui.getShape().height));
	if (x > ofGetWidth()) {
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
void AstanaSoundManager::stopAll() {
	std::function<void()> cb;
	for (auto& g : groups) {
		g.second->fadeOutAll(cb);
	}
}
//---------------------------------------------------
void AstanaSoundManager::playNextTrack() {
	if (groups.count(activeGroup)) {
		if(!groups[activeGroup]->playNext()){
			cout << "No se puede dar play a next!" << endl;
		}
	}
}
//---------------------------------------------------
void AstanaSoundManager::setActiveGroup(string groupName) {
	if (texturas) {
		if (groupName == texturas->getName()) {
			cout << " set active group no puede ser texturas" << endl;
			return;
		}
	}
	if (activeGroup != groupName) {
		string tempGroup = activeGroup;
		activeGroup = groupName;
		if (groups.count(tempGroup)) {
			groups[tempGroup]->highlightGui(false);
			if (groups[tempGroup]->isPlaying()) {
				groups[tempGroup]->stopAll();
//				groups[tempGroup]->fadeOutAll([this]() {startActiveGroup();});
			}
			else {
				startActiveGroup();
			}
		}
		else {
			startActiveGroup();
		}
	}
}
//---------------------------------------------------
void AstanaSoundManager::startActiveGroup() {
	if (texturas && groups.count(activeGroup)) {
		clearSoundBlobLinks();
		groups[activeGroup]->highlightGui(true);
		if (typeid(*(groups[activeGroup].get())).name() == typeid(AstanaSoundIntervencion).name()) {
			groups[activeGroup]->play();
		}
		//else {
		//	ofParameterGroup& texGrp = groups[activeGroup]->texturasValidasGroup;
		//	for (int i = 0; i < texGrp.size();i++) {
		//		auto p = texturas->getPlayer(texGrp.getName(i));
		//		if (p) {
		//			if (texGrp.getBool(i)) {
		//		//		p->play();
		//			}
		//			else {
		//				p->fadeOut();
		//			}
		//		}
		//	}
		//}
	}
}

//---------------------------------------------------
void AstanaSoundManager::load(string folderPath) {
	this->folderPath = folderPath;
	groups.clear();
	texturas = addGroup<AstanaSoundTextura>("Texturas");
	addGroup<AstanaSoundIntervencion>("CumparsitaCOmbo");
	addGroup<AstanaSoundSecuencia>("ElDiaQueMeQuieras");
	addGroup<AstanaSoundSecuencia>("Nonino");
	addGroup<AstanaSoundGroup>("Sueltos");


	ofSoundStreamSettings settings;
	settings.numInputChannels = 0;
	settings.numOutputChannels = 2;
	settings.numBuffers = 1;
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
		blobListeners.clear();
		blobListeners.push_back(blobManager->anyBlobEvent.newListener(this, &AstanaSoundManager::onAnyBlobs));
		blobListeners.push_back(ofEvents().update.newListener(this, &AstanaSoundManager::update));

	}
	else {
		cout << "BLOB MANGER ES NULL!!!" << endl;
	}
}
//---------------------------------------------------
void AstanaSoundManager::onAnyBlobs() {
	AstanaBlobCollection c;
	toBlobsUpdate.send(c);//blobManager->getBlobsCollection());
}
//---------------------------------------------------
//AstanaBlobParam AstanaSoundManager::getNextAvailableBlobParam() {
//
//	std::random_device random_device;
//	std::mt19937 engine{ random_device() };
//	std::uniform_int_distribution<int> dist(0, availableBlobParams.size() - 1);
//
//	return availableBlobParams[dist(engine)];
//}
//---------------------------------------------------
void AstanaSoundManager::updateBlobs() {

	if (blobsBack.size()) {
		if (blobsBack.count(ASTANA_ALL_BLOBS)) {
			for (auto& b : blobsBack[ASTANA_ALL_BLOBS]) {//blobManager->getNewBlobs()) {
				if (!soundBlobLinks.count(b->label)) {
					addLink(b->label);//, getNextAvailableBlobParam());
//					addLink(b->label, getNextAvailableBlobParam());
				}
			}
		}
		if (blobsBack.count(ASTANA_KILLED_BLOBS)) {
			for (auto& b : blobsBack[ASTANA_KILLED_BLOBS]) {
				if (!removeLink(b->label)) {
					cout << "No se elimino el blob " << b->label << endl;
				}
			}
		}

		if (blobsBack.count(ASTANA_GHOST_BLOBS)) {
			for (auto& b : blobsBack[ASTANA_GHOST_BLOBS]) {
				if (soundBlobLinks.count(b->label)) {
					if (!removeLink(b->label)) {
						cout << "No se elimino el blob " << b->label << endl;
					}
				}
			}
		}

		updateSoundBlobLinks(ASTANA_ALL_BLOBS);
		//updateSoundBlobLinks(ASTANA_MOVED_BLOBS);
		//updateSoundBlobLinks(ASTANA_SCALED_BLOBS);
		//updateSoundBlobLinks(ASTANA_MERGED_BLOBS);
	}
}
//---------------------------------------------------
void AstanaSoundManager::update(ofEventArgs&) {
	while (fromBlobsUpdate.tryReceive(blobsMiddle)) {
		if(blobsMiddle.size()){
		mutex.lock();
		//blobsFront.swap(blobsMiddle);
		blobsFront = blobsMiddle;
		mutex.unlock();
		}
	}
}
//---------------------------------------------------
void AstanaSoundManager::threadedFunction() {
	// also receive doesn't allocate or make any copies
	while (toBlobsUpdate.receive(blobsBack)) {
		mutex.lock();
		blobsBack = blobManager->getBlobsCollection();
		mutex.unlock();
		updateBlobs();
#ifdef ASTANA_PRINT_DEBUG
		stringstream ss;
		ss << "=================================================" << endl;
		for (auto&g : blobsBack) {
			ss << "------------" << AstanaToString(g.first) << endl;
			for (auto& a : g.second) {
				ss << a->label << endl;
			}
		}
		for (auto& sb : soundBlobLinks) {
			for (auto& l : sb.second) {
				ss << sb.first << "    " << l.first << "  " << AstanaToString(l.second.blobParam) << endl;
			}
		}
		fileBuffer.append(ss.str());
#endif
		//		blobsMiddle.clear();
		blobsMiddle = blobsBack;
		fromBlobsUpdate.send(std::move(blobsMiddle));
	}
}
//---------------------------------------------------
string AstanaSoundManager::getNextTextura(){
	if(texturas){
	//for (int i = 0; i < texturas->players.size(); i++) {
		for(auto& p: texturas->players){
		if(currentTextura != p.getName()){
			if(isTexturaValida(p.getName())){
				currentTextura = p.getName();
				return currentTextura;
				break;
			}
		}
	}
	}
	return 	"";	

}
//---------------------------------------------------
bool AstanaSoundManager::addLink(unsigned int label){//, AstanaBlobParam blobParam) {
	if(texturas){
		if (soundBlobLinks.count(label) == 0) {
				auto indexTextura = getNextTextura();
				auto player = texturas->getPlayer(indexTextura);
				if(player){
					if(player->canPlayInstance()){
						cout << "link added " << endl;
						auto index =player->play();
							soundBlobLinks[label][indexTextura] = index;
	//						player->fadeIn(index);
					//soundBlobLinks[label][indexTextura].soundParam.makeReferenceTo(*p);
		//			setAvailableParamUse(name, false);
					return true;
					}
				}
			}
		}
	return false;
}
//---------------------------------------------------
bool AstanaSoundManager::removeLink(unsigned int label) {// AstanaBlobParam blobParam) {
	if(texturas){
	if (soundBlobLinks.count(label)) {
		//for (auto& b : soundBlobLinks[label]) {
		//	setAvailableParamUse(b.first, true);
		//}
		for (auto& l: soundBlobLinks[label]) {
			auto p = texturas->getPlayer(l.first);
			if(p){
				p->stop(l.second);
			}
		}
		soundBlobLinks.erase(label);

		return true;
	}
	}
	return false;
}
//---------------------------------------------------
bool AstanaSoundManager::isTexturaValida(string tex){
	if (groups.count(activeGroup)) {
		ofParameterGroup& texGrp = groups[activeGroup]->texturasValidasGroup;
		if (texGrp.contains(tex)) {
			return texGrp.getBool(tex).get();
		}
	}
	return false;
}
//---------------------------------------------------


	//if(bUsarCentroX) availableBlobParams.push_back(ASTANA_BLOB_CENTER_X);
	//if(bUsarCentroY) availableBlobParams.push_back(ASTANA_BLOB_CENTER_Y);
	//if(bUsarArea) availableBlobParams.push_back(ASTANA_BLOB_AREA);
	//if(bUsarVelX) availableBlobParams.push_back(ASTANA_BLOB_VEL_X);
	//if(bUsarVelY) availableBlobParams.push_back(ASTANA_BLOB_VEL_Y);

//}
//---------------------------------------------------
void AstanaSoundManager::clearSoundBlobLinks(){
	soundBlobLinks.clear();
//	setAvailableSoundParams();
}
//---------------------------------------------------
void AstanaSoundManager::updateSoundBlobLinks(AstanaBlobType t) {
	auto mapParam = [&](float val, const AstanaBlobParam& p, float mn, float mx)->float {
		if (mapMinVals.count(p) > 0 && mapMaxVals.count(p)) {
			return ofMap(val, mapMinVals[p], mapMaxVals[p], mn, mx, true);
		}
		return val;
	};
	auto smooth = [&](const float& prev, const float& current)->float {
		return prev*(1 - smoothingParams) + current*smoothingParams;
	};
	if(texturas){
	if (blobsBack.count(t)) {
		for (auto& b : blobsBack[t]) {
			if (soundBlobLinks.count(b->label)) {
				for (auto& l : soundBlobLinks[b->label]) {
					auto p = texturas->getPlayer(l.first);
					if(p){
						if(bUsarCentroX) p->setVolume(smooth(p->getVolume(l.second), mapParam(b->center.x, ASTANA_BLOB_CENTER_X,  0, 1)), l.second);
						if(bUsarCentroY) p->setPan(   smooth(p->getPan(   l.second), mapParam(b->center.y, ASTANA_BLOB_CENTER_Y, -1, 1)), l.second);
						if(bUsarArea) p->setSpeed( smooth(p->getSpeed( l.second), mapParam(b->area,     ASTANA_BLOB_AREA,      0.6, 1.5)), l.second);
					//texturas->getPlayer(l.)
					//l.second
					//	= smooth(l.second.soundParam.get() ,mapParam(b->getParam(l.second.blobParam), l.second.blobParam), smoothingParams);
					}
				}
			}
		}
	}
	}
}
//---------------------------------------------------
template<typename T>
shared_ptr<T> AstanaSoundManager::addGroup(string groupFolder) {
	ofFile f(folderPath + ofFilePath::addLeadingSlash(groupFolder));
	string groupName = f.getBaseName();
	if (groups.count(groupName) == 0) {
		groups[groupName] = make_shared<T>();
		auto p = static_pointer_cast<T>(groups[groupName]);
		p->setName(groupName);
		p->setManager(this);
		p->setup(f.getAbsolutePath());
		p->getMixer().connectTo(mixer);
		return p;
	}
	else {
		cout << "ya existe el grupo " << groupName << endl;
	}
	return nullptr;
}
//---------------------------------------------------
void AstanaSoundManager::keyReleased(ofKeyEventArgs& k) {
	if (k.key == OF_KEY_RIGHT) {
		playNextTrack();
	}
	else if (k.key == OF_KEY_RETURN) {
		if(texturas){
			texturas->playAll();
		}
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
//---------------------------------------------------
void AstanaSoundManager::activarCumparsita() {
	setActiveGroup("CumparsitaCOmbo");
}
//---------------------------------------------------
void AstanaSoundManager::activarElDiaQueMeQuieras() {
	setActiveGroup("ElDiaQueMeQuieras");
}
//---------------------------------------------------
void AstanaSoundManager::activarNonino() {
	setActiveGroup("Nonino");
}
//---------------------------------------------------
void AstanaSoundManager::activarSueltos() {
	setActiveGroup("Sueltos");
}
