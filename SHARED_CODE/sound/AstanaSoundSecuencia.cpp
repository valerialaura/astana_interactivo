//
//  AstanaSoundSecuencia.cpp
//  gestionSonido
//
//  Created by Roy Macdonald on 21-05-17.
//
//

#include "AstanaSoundSecuencia.h"

AstanaSoundSecuencia::AstanaSoundSecuencia():AstanaSoundGroup(){}
//---------------------------------------------------
bool AstanaSoundSecuencia::play(){
    if (AstanaSoundGroup::play()) {
        currentListener = players[current].endEvent.newListener(this, &AstanaSoundSecuencia::onCurrentEnd);
		return true;
    }
	return false;
}
//---------------------------------------------------
bool AstanaSoundSecuencia::isNextAllowed(){
//    cout << __PRETTY_FUNCTION__ << endl;
    if(bFirstPlay){
        bFirstPlay = false;
        return true;
    }
    if(players[current].isPlaying())return false;
    return (ofGetElapsedTimef() - lastEndTime) >= gapFraseMin;
}
//---------------------------------------------------
void AstanaSoundSecuencia::setupParameters(){
    parameters.setName("Secuencia " + getName());
    parameters.add(gapFraseMin.set("tiempo min entre frases (seg)", 0,0, 10));
    AstanaSoundGroup::setupParameters();    
}
//---------------------------------------------------
void AstanaSoundSecuencia::loadFolder(string folderPath){
    AstanaSoundGroup::loadFolder(folderPath);
    if (players.size()) {
        current = 0;
    }
}
//---------------------------------------------------
void AstanaSoundSecuencia::onCurrentEnd(){
    lastEndTime = ofGetElapsedTimef();
    players[current].setPositionMS(0);
    if (players[current].loopeable) {
        players[current].play();
    }else{
        players[current].setPaused(true);
        currentListener.unsubscribe();
    }
}