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
void AstanaSoundManager::drawGui(){
    for(auto& g: groups){
        g.second->gui.draw();
    }
    gui.draw();
}
//---------------------------------------------------
void AstanaSoundManager::setup(string folderPath){
    load(folderPath);
    setupGui();
}
//---------------------------------------------------
void AstanaSoundManager::setupGui(){
    string xmlName = "astana_sound_manager_settings.xml";
    gui.setup("OPCIONES GLOBALES", xmlName);
    gui.setHeaderBackgroundColor(ofColor::blue);
    gui.add(fadeEscenaDuration.set("duracion fade escenas. segs.", 1, 0, 10));
    gui.add(new ofxButton(ofParameter<void>("STOP ALL")));
    gui.getButton("STOP ALL").addListener(this, &AstanaSoundManager::stopAll);
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
    settings.bufferSize = 512;
    settings.sampleRate = 48000;
    settings.setOutListener(&mixer);
    soundStream.setup(settings);
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
        return p;
    }else{
        cout << "ya existe el grupo " << groupName << endl;
    }
    return nullptr;
}