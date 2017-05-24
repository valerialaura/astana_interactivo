//
//  AstanaSoundPlayer.cpp
//  gestionSonido
//
//  Created by Roy Macdonald on 22-05-17.
//
//

#include "AstanaSoundPlayer.h"

//ofEvent<void> AstanaSoundPlayer::guiNeedUpdateEvent = ofEvent<void>();

AstanaSoundPlayer::AstanaSoundPlayer():ofxBasicSoundPlayer(){
    setMultiPlay(false);
}
//---------------------------------------------------
AstanaSoundPlayer::~AstanaSoundPlayer(){
    enableGuiListeners(false);
}
//---------------------------------------------------
bool AstanaSoundPlayer::load(std::filesystem::path filePath, bool stream){
    bIsLoaded = ofxBasicSoundPlayer::load(filePath, stream);
    if (bIsLoaded) {
        name = ofFilePath::getBaseName(filePath);
        loopeable.set(name, false);
        volume.set(name, 1, 0, 1);
        pan.set(name, 0, -1, 1);
    }
    return bIsLoaded;
}
//---------------------------------------------------
void AstanaSoundPlayer::enableGuiListeners(bool e){
    if(e != bHasListeners){
        if(e){
            volume.addListener(this, &AstanaSoundPlayer::volumeChanged);
            pan.addListener(this, &AstanaSoundPlayer::panChanged);
        }else{
            volume.removeListener(this, &AstanaSoundPlayer::volumeChanged);
            pan.removeListener(this, &AstanaSoundPlayer::panChanged);
        }
        bHasListeners = e;
    }
}
//---------------------------------------------------
void AstanaSoundPlayer::play(){
    if(!isPlaying()){
        ofxBasicSoundPlayer::play();
        ofNotifyEvent(guiNeedUpdateEvent, this);
    }
}
//---------------------------------------------------
void AstanaSoundPlayer::stop(){
    if(isPlaying()){
        ofxBasicSoundPlayer::stop();
        ofNotifyEvent(guiNeedUpdateEvent, this);
    }
}
//---------------------------------------------------
void AstanaSoundPlayer::setPaused(bool bP){
    ofxBasicSoundPlayer::setPaused(bP);
    ofNotifyEvent(guiNeedUpdateEvent, this);
}
//---------------------------------------------------
void AstanaSoundPlayer::fadeIn(float duration){
    if(!isPlaying()){
        fade(FADE_IN, duration);
    }
}
//---------------------------------------------------
void AstanaSoundPlayer::fade(_fadeStatus newStatus, float duration){
    if (fadeStatus == newStatus) {
        return;
    }
    if(newStatus == FADE_NONE || fadeStatus != FADE_NONE){
        stopFade();
    }
    fadeStatus = newStatus;
    
    if(fadeStatus != FADE_NONE){
        bool bIsFadeIn = (fadeStatus == FADE_IN);
        fadeOutTween.setParameters( ofxeasing::linear::easeNone, (bIsFadeIn?0:getVolume()), (bIsFadeIn?getVolume():0), duration*1000, 0);
        fadeOutTween.addEndListener(this, &AstanaSoundPlayer::fadeEnd);
        ofAddListener(ofEvents().update, this, &AstanaSoundPlayer::updateFade);
    }
}
//---------------------------------------------------
void AstanaSoundPlayer::fadeOut(float duration){
    if(isPlaying()){
        fade(FADE_OUT, duration);
        //        fadeOutTween.setParameters( ofxeasing::linear::easeNone, getVolume(), 0, duration*1000, 0);
        //        fadeOutTween.addEndListener(this, &AstanaSoundPlayer::fadeEnd);
        //        ofAddListener(ofEvents().update, this, &AstanaSoundPlayer::updateFade);
    }
}
//---------------------------------------------------
void AstanaSoundPlayer::updateFade(ofEventArgs& a){
    setVolume(fadeOutTween.update());
    if(!isPlaying()){
        stopFade();
    }
}
//---------------------------------------------------
void AstanaSoundPlayer::stopFade(){
    int i;
    fadeEnd(i);
}
//---------------------------------------------------
void AstanaSoundPlayer::fadeEnd(int & i){
    fadeOutTween.removeEndListener(this, &AstanaSoundPlayer::fadeEnd);
    ofRemoveListener(ofEvents().update, this, &AstanaSoundPlayer::updateFade);
    setPaused(true);
    ofNotifyEvent(fadeEndEvent,this);
    fadeStatus = FADE_NONE;
    
}
//---------------------------------------------------
void AstanaSoundPlayer::volumeChanged(float& f){
    setVolume(volume);
}
//---------------------------------------------------
void AstanaSoundPlayer::panChanged(float& f){
    setPan(pan);
}

