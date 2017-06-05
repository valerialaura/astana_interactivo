//
//  AstanaSoundPlayer.cpp
//  gestionSonido
//
//  Created by Roy Macdonald on 22-05-17.
//
//

#include "AstanaSoundPlayer.h"

//ofEvent<void> AstanaSoundPlayer::guiNeedUpdateEvent = ofEvent<void>();

AstanaSoundPlayer::AstanaSoundPlayer():ASTANA_SOUND_PLAYER(){
    //setMultiPlay(false);
}
//---------------------------------------------------
AstanaSoundPlayer::~AstanaSoundPlayer(){}
//---------------------------------------------------
bool AstanaSoundPlayer::load(std::filesystem::path filePath, bool stream){
    bIsLoaded = ASTANA_SOUND_PLAYER::load(filePath, stream);
    if (bIsLoaded) {
        name = ofFilePath::getBaseName(filePath);
        loopeable.set(name, false);
    }
    return bIsLoaded;
}
//---------------------------------------------------
void AstanaSoundPlayer::fadeIn(size_t index,float duration){
    if(!isPlaying(index)){
        fade(FADE_IN, index, duration);
    }
}
//---------------------------------------------------
void AstanaSoundPlayer::fadeOut(size_t index, float duration){
    if(isPlaying(index)){
		fade(FADE_OUT, index, duration);
    }
}
//---------------------------------------------------
void AstanaSoundPlayer::fade(_fadeStatus newStatus, size_t index, float duration){
	if(fadeStatus.count(index)){
		if (fadeStatus[index] == newStatus) {
			return;
		}
	    if(newStatus == FADE_NONE || fadeStatus[index] != FADE_NONE){
		    stopFade(index);
		}
	}
    fadeStatus[index] = newStatus;
    
    if(fadeStatus[index] != FADE_NONE){
        bool bIsFadeIn = (fadeStatus[index] == FADE_IN);
		fadeTween[index].setParameters(index, ofxeasing::linear::easeNone, (bIsFadeIn?0:getVolume(index)), (bIsFadeIn?getVolume(index):0), duration*1000, 0);
        fadeTween[index].addEndListener(this, &AstanaSoundPlayer::fadeEnd);
        ofAddListener(ofEvents().update, this, &AstanaSoundPlayer::updateFade);
    }
}
//---------------------------------------------------
void AstanaSoundPlayer::updateFade(ofEventArgs& a){
	for(auto& f: fadeTween){
		setVolume(f.second.update(), f.first );
		if(!isPlaying(f.first)){
			stopFade(f.first);
		}
	}
}
//---------------------------------------------------
void AstanaSoundPlayer::stopFade(size_t index){
    fadeEnd(index);
}
//---------------------------------------------------
void AstanaSoundPlayer::fadeEnd(size_t & i){
	if(fadeTween.count(i)){
	    fadeTween[i].removeEndListener(this, &AstanaSoundPlayer::fadeEnd);
		setPaused(true,i);
	    ofNotifyEvent(fadeEndEvent,this);
		fadeStatus[i] = FADE_NONE;
		fadeTween.erase(i);
	}
	if(fadeTween.size() == 0){
		ofRemoveListener(ofEvents().update, this, &AstanaSoundPlayer::updateFade);
	}
}
////---------------------------------------------------
//void AstanaSoundPlayer::volumeChanged(float& f){
//    setVolume(volume);
//}
////---------------------------------------------------
//void AstanaSoundPlayer::panChanged(float& f){
//    setPan(pan);
//}
////---------------------------------------------------
//void AstanaSoundPlayer::speedChanged(float&){
//	setSpeed(speed);
//}
