//
//  AstanaSoundPlayer.cpp
//  gestionSonido
//
//  Created by Roy Macdonald on 22-05-17.
//
//

#include "AstanaSoundPlayer.h"
//---------------------------------------------------
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

/*
 * AstanaSoundPlayer.cpp
 *
 *  Created on: 25/07/2012
 *      Author: arturo
 */

int AstanaSoundPlayer::maxSoundsTotal=128;
int AstanaSoundPlayer::maxSoundsPerPlayer=16;

//--------------------------------------------------------------
AstanaSoundPlayer::AstanaSoundPlayer() {
	bStreaming = false;
	bMultiplay = false;
	bIsLoaded = false;
	bIsLooping = false;
	instances.resize(1);
	maxSounds = maxSoundsPerPlayer;
}
//--------------------------------------------------------------
bool AstanaSoundPlayer::canPlayInstance(){
	for(auto& i: instances){
		if(!i.getIsPlaying()){
			return true;
		}
	}
	return false;
}
//--------------------------------------------------------------
AstanaSoundPlayer::~AstanaSoundPlayer() {
	if (isLoaded()) {
		unload();
	}
}
//--------------------------------------------------------------
bool AstanaSoundPlayer::load(std::filesystem::path filePath, bool _stream){
	bStreaming = _stream;
	instances.resize((bMultiplay?maxSounds:1));
	
	bIsLoaded = true;
	for(auto& i: instances){
		if (!i.load(filePath, _stream)) {
			cout << "No se cargo el sonido " << filePath << endl;
			bIsLoaded = false;
		}
	}
	if(bIsLoaded){
		name = ofFilePath::getBaseName(filePath);
		loopeable.set(name, false);
	}
	return bIsLoaded;
}

//--------------------------------------------------------------
void AstanaSoundPlayer::unload(){
	bIsLoaded = false;
	for (auto& i : instances) {
		i.unload();
	}
	instances.clear();
}
//--------------------------------------------------------------
size_t AstanaSoundPlayer::play() {
	size_t index = 0;
	if (bIsLoaded) {
		for (int i = 0; i < instances.size(); i++) {
			if (!instances[i].isPlaying()) {
				instances[i].play();
				index = i;
				break;
			}
		}
	}
	return index;
}
//--------------------------------------------------------------
void AstanaSoundPlayer::stop(int index){
	if (index < 0) {
		for(auto& i: instances){
			i.stop();
		}
	}else if(index < instances.size()){
		instances[index].stop();
	}
}
//========================SETTERS===============================
void AstanaSoundPlayer::setVolume(float vol, size_t index){ 
	if(index < instances.size()){
		instances[index].setVolume(vol);
	}else{
		cout << "index out of range" << endl;
	}
}
//--------------------------------------------------------------
void AstanaSoundPlayer::setPan(float _pan, size_t index){ 
	if(index < instances.size()){
		instances[index].setPan(_pan);
	}else{
		cout << "index out of range" << endl;
	}
}
//--------------------------------------------------------------
void AstanaSoundPlayer::setSpeed(float spd, size_t index){
	if(index < instances.size()){
		instances[index].setSpeed(spd);
	}else{
		cout << "index out of range" << endl;
	}
}
//--------------------------------------------------------------
void AstanaSoundPlayer::setPaused(bool bP, int index){ 
	if (index < 0) {
		for (auto& i : instances) {
			i.setPaused(bP);
		}
	}else if(index < instances.size()){
		instances[index].setPaused(bP);
	}
}
//--------------------------------------------------------------
void AstanaSoundPlayer::setLoop(bool bLp){ 
	bIsLooping = bLp;
	for(auto&i: instances){
		i.setLoop(bLp);
	}
}
//--------------------------------------------------------------
void AstanaSoundPlayer::setMultiPlay(bool bMp){
	bMultiplay = bMp;
	if(!bMultiplay && instances.size() != 1){
		instances.resize(1);
	}
}
//--------------------------------------------------------------
void AstanaSoundPlayer::setPosition(float pct, size_t index){
	pct = ofClamp(pct, 0, 1);
	if(index < instances.size()){
		instances[index].setPosition(pct);
	}
}
//--------------------------------------------------------------
//--------------------------------------------------------------
//========================GETTERS===============================
float AstanaSoundPlayer::getPosition(size_t index) const{ 
	if(index < instances.size()){
		return instances[index].getPosition();
	}
	return 0;
}
//--------------------------------------------------------------
bool AstanaSoundPlayer::isPlaying(size_t index) const{ 
	if(index < instances.size()){
		return instances[index].isPlaying(); 
	}
	return false;
}
//--------------------------------------------------------------
bool AstanaSoundPlayer::isPlaying() const{ 
	for (auto& i : instances) {
		if (i.isPlaying()) return true;
	}
	return false;
}
//--------------------------------------------------------------
bool AstanaSoundPlayer::getIsLooping() const{ 
	return bIsLooping;
}
//--------------------------------------------------------------
float AstanaSoundPlayer::getSpeed(size_t index) const{
	if(index < instances.size()){
		return instances[index].getSpeed();
	}
	return 0;
}
//--------------------------------------------------------------
float AstanaSoundPlayer::getPan(size_t index) const{
	if(index < instances.size()){
		return instances[index].getPan();
	}
	return 0;
}
//--------------------------------------------------------------
bool AstanaSoundPlayer::isLoaded() const{ return bIsLoaded; }
//--------------------------------------------------------------
float AstanaSoundPlayer::getVolume(size_t index) const{ 
	if(index < instances.size()){
		return instances[index].getVolume();
	}
	return 0;	
}
//--------------------------------------------------------------
