//
//  AstanaSoundIntervencion.cpp
//  gestionSonido
//
//  Created by Roy Macdonald on 21-05-17.
//
//

#include "AstanaSoundIntervencion.h"
AstanaSoundIntervencion::AstanaSoundIntervencion():AstanaSoundGroup(){}
//---------------------------------------------------
bool AstanaSoundIntervencion::isLoopPlaying(){
    if(loopIndex < players.size()){
        return players[loopIndex].isPlaying();
    }
    return false;
}
//---------------------------------------------------
bool AstanaSoundIntervencion::isIntervencionPlaying(){
    for (auto i: intervencionesIndices) {
        if (i < players.size()) {
            if (players[i].isPlaying()) {
                cout << "intervencion " << players[i].getName() << " playing!" << endl;
                return true;
            }
        }
    }
    return false;
}
//---------------------------------------------------
void AstanaSoundIntervencion::onLoopEnd(){
    bFirstLoop = false;
    play();
}
//---------------------------------------------------
void AstanaSoundIntervencion::onIntervencionEnd(){
    if (current < intervencionesIndices.size()) {
        if (intervencionesIndices[current] < players.size()) {
            if (players[intervencionesIndices[current]].loopeable) {
                if(ofRandomuf() < probabilidadLoopIntervencion ){
                    players[intervencionesIndices[current]].setPosition(0);
                    players[intervencionesIndices[current]].play();
                }else{
                    players[intervencionesIndices[current]].stop();
                }
            }else{
                players[intervencionesIndices[current]].stop();
            }
        }
    }
}
//---------------------------------------------------
void AstanaSoundIntervencion::loadFolder(string folderPath){
    AstanaSoundGroup::loadFolder(folderPath);
}
//---------------------------------------------------
void AstanaSoundIntervencion::setListeners(bool e){
    if(bListenersEnabled != e){
        AstanaSoundGroup::setListeners(e);
        if (bListenersEnabled) {
            for(int i = 0; i < players.size(); i++){
                if(ofStringTimesInString(ofToLower(players[i].getName()), "loop")){
                    loopIndex = i;
                    listeners.push_back(players[i].endEvent.newListener(this, &AstanaSoundIntervencion::onLoopEnd));
                    cout << "AstanaSoundIntervencion loop: " << players[i].getName() << " index: " << loopIndex << endl;
                }else{
                    listeners.push_back(players[i].endEvent.newListener(this, &AstanaSoundIntervencion::onIntervencionEnd));
                    intervencionesIndices.push_back(i);
                    cout << "AstanaSoundIntervencion intervencion: " << players[i].getName() << " index: " << i << endl;
                }
            }
        }
    }
}
//---------------------------------------------------
bool AstanaSoundIntervencion::isNextAllowed(){
    bool bIntPlay = isIntervencionPlaying();
    cout << __PRETTY_FUNCTION__ << " intervencion playing: " << (bIntPlay?"TRUE":"FALSE") << " first loop: " << (bFirstLoop?"TRUE":"FALSE") << endl;
    if (bIntPlay) {
        return false;
    }
    if(!bFirstLoop){
        return true;
    }
    return false;
}
//---------------------------------------------------
bool AstanaSoundIntervencion::playNext(){
    if(this->isNextAllowed()){
            cout << "current " << current <<" intervencionesIndices: " << intervencionesIndices.size()<<endl;
        if (current < intervencionesIndices.size()) {

            if (intervencionesIndices[current] < players.size()) {
            cout << "intervencionesIndices[current] < players.size()"<<endl;
                players[intervencionesIndices[current]].fadeOut();
                current++;
                current %= intervencionesIndices.size();
                players[intervencionesIndices[current]].setPosition(0);
                players[intervencionesIndices[current]].play();
                return true;
            }
        }
    }else{
        cout << __PRETTY_FUNCTION__ << " not ALLOWED"<< endl;
    }
    return false;
}
//---------------------------------------------------
bool AstanaSoundIntervencion::play(){
    //    pause();
    //    if(players.size() > 0){
    //        if(current < intervencionesIndices.size()){
    //            if (intervencionesIndices[current] < players.size()) {
    //                players[intervencionesIndices[current]].setPosition(0);
    //                players[intervencionesIndices[current]].play();
    //                return true;
    //            }
    //        }
    //    }
    //    return false;
    if(loopIndex < players.size()){
        players[loopIndex].loopeable = true;
        players[loopIndex].setMultiPlay(false);
        if (!players[loopIndex].isPlaying()) {
            players[loopIndex].setPosition(0);
        }
        players[loopIndex].play();
        return true;
    }
    return false;
    
}
//---------------------------------------------------
void AstanaSoundIntervencion::setupParameters(){
    parameters.add(probabilidadLoopIntervencion.set("Probabilidad Loop Intervencion", 0.5,0,1));
    AstanaSoundGroup::setupParameters();
}
