//
//  AstanaSoundLayer.cpp
//  gestionSonido
//
//  Created by Roy Macdonald on 21-05-17.
//
//

#include "AstanaSoundGroup.h"
#include "AstanaSoundManager.h"
//---------------------------------------------------
AstanaSoundGroup::AstanaSoundGroup(){
    bIsTextura = false;
}
//---------------------------------------------------
void AstanaSoundGroup::setup(string folderPath){
    this->loadFolder(folderPath);
    this->setupParameters();
//    setListeners();
    gui.getGroup("Pistas Loopeables").minimize();
    if(!bIsTextura){
        gui.getGroup("Texturas Validas").minimize();
    }
}
//---------------------------------------------------
void AstanaSoundGroup::loadFolder(string folderPath){
    ofDirectory dir;
    dir.allowExt("wav");
    dir.allowExt("WAV");
    dir.allowExt("AIFF");
    dir.allowExt("aiff");
    dir.allowExt("mp3");
    dir.allowExt("MP3");
    
    dir.listDir(folderPath);
    dir.sort();

    players.clear();
    for (auto& f: dir.getFiles()) {
        players.push_back(AstanaSoundPlayer());
		players.back().setMultiPlay(bIsTextura);
		players.back().load(f.getAbsolutePath());
    }
    bFolderLoaded = true;
}
//---------------------------------------------------
void AstanaSoundGroup::setupParameters(){
//    cout << __PRETTY_FUNCTION__ << endl;
    if(bFolderLoaded){
        addParameterGroup(loopeablesGroup,"Pistas Loopeables");
//        addParameterGroup(volumeGroup,"Volumen Pistas");
//        addParameterGroup(panGroup,"Pan Pistas");
        //parameters.add(delayFx.parameters);

		speedGroup.setName("Speed");

		for(auto& p: players){
            if(p.isLoaded()){
                loopeablesGroup.add(p.loopeable);
                //volumeGroup.add(p.volume);
                //panGroup.add(p.pan);
				//speedGroup.add(p.speed);
            }
        }
        if(!isTextura() && manager != nullptr){
            texturasValidasGroup.clear();
            texturasValidasGroup.setName("Texturas Validas");
            
            for(auto&t : manager->getTexturas()->players){
                texturasValidasGroup.add(ofParameter<bool>().set(t.getName(), false));
            }
            parameters.add(texturasValidasGroup);
        }
        if (parameters.getName() == "") {
            parameters.setName(getName());
        }
        gui.setup(parameters, getName()+"_settings.xml");
        gui.setName(getName());
        gui.loadFromFile(getName()+"_settings.xml");
    }
}
//---------------------------------------------------
void AstanaSoundGroup::updateGui(size_t&){
    for(auto& p: players){
        auto gr = gui.getGroup("Volumen Pistas");
        auto vc = gr.getControl(p.getName());
        if (vc) {
            vc->setFillColor(p.isPlaying()?ofColor::red:ofColor(128));
        }
    }
}
//---------------------------------------------------
void AstanaSoundGroup::stopAll(){
	for(auto& p: players){
		p.stop(p.getNumInstances());
	}
}
//---------------------------------------------------
void AstanaSoundGroup::addParameterGroup(ofParameterGroup& pg, string name){
    pg.clear();
    pg.setName(name);
    if (parameters.contains(name)) {
        parameters.remove(pg);
    }
    parameters.add(pg);
}
//---------------------------------------------------
void AstanaSoundGroup::setManager(AstanaSoundManager* mngr){
    manager = mngr;
}
//---------------------------------------------------
AstanaSoundPlayer* AstanaSoundGroup::getPlayer(const string& playerName){
    for(auto&p: players){
        if(playerName == p.getName()){
            return &p;
        }
    }
    return nullptr;
}
//---------------------------------------------------
bool AstanaSoundGroup::isNextAllowed(){
//    cout << __PRETTY_FUNCTION__ << endl;
    return true;
}
//---------------------------------------------------
bool AstanaSoundGroup::playNext(){
	if (players.size() > 0) {
		if (current < (int)players.size()) {
			if (this->isNextAllowed()) {
				if (current >= 0) {
					players[current].fadeOut(0);
				}
				current++;
				current %= players.size();
				players[current].play();
				return true;
			}else{
				cout << "playNext next ! allowed" << endl;
			}
		}else{
		cout << "playNext current >= players.size  current: " << current << "  playerssize: " << players.size()<< endl;
		}
	}
	else {
		cout << "playNext players.size<0" << endl;
	}
    return false;
}
//---------------------------------------------------
bool AstanaSoundGroup::play(){
    pause();
    if(players.size() > 0){
		if (current < 0) {
			current = 0;
		}
        if(current < players.size()){
            players[current].play();
            return true;
        }
    }
    return false;
}
//---------------------------------------------------
void AstanaSoundGroup::pause(){
    for (auto& p: players) {
        p.setPaused(true);
    }
}
//---------------------------------------------------
void AstanaSoundGroup::reset(){
    pause();
    current = -1;
    if(players.size() > 0){
        for (auto& p: players) {
              p.setPosition(0);
        }
    }
}
//---------------------------------------------------
bool AstanaSoundGroup::isPlaying(){
    for(auto&p: players){
        if(p.isPlaying())return true;
    }
    return false;
}
//---------------------------------------------------
void AstanaSoundGroup::fadeOutAllEnd(){
      fadeOutAllCallback();
    fadeListeners.clear();
}
//---------------------------------------------------
void AstanaSoundGroup::fadeOutAll(std::function< void() > callback){
    if(isPlaying()){
        fadeOutAllCallback = callback;
        fadeListeners.clear();
        for (auto& p: players) {
            if(p.isPlaying()){
                fadeListeners.push_back(p.fadeEndEvent.newListener(this, &AstanaSoundGroup::fadeOutAllEnd));
                float fadeDuration = 1;
                if (manager) {
                    fadeDuration = manager->getFadeEscenaDuration();
                }
                p.fadeOut(fadeDuration);
            }
        }
    }
}
//---------------------------------------------------
void AstanaSoundGroup::highlightGui(bool hl){
    if(hl){
        gui.setHeaderBackgroundColor(ofColor::yellow);
        gui.setTextColor(0);
    }else{
        gui.setHeaderBackgroundColor(64);
        gui.setTextColor(255);
    }
}