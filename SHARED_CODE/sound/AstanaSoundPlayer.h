//
//  AstanaSoundPlayer.h
//  gestionSonido
//
//  Created by Roy Macdonald on 22-05-17.
//
//

#pragma once
#include "ofMain.h"
#include "ofxSoundObjects.h"
#include "ofxTween2.h"
class AstanaSoundPlayer: public ofxBasicSoundPlayer{
public:
    AstanaSoundPlayer();
    virtual ~AstanaSoundPlayer();
    bool load(std::filesystem::path filePath, bool stream = false);
    
    ofParameter<bool> loopeable;
    ofParameter<float> volume, pan;
    
    void play();
	void stop();
	void setPaused(bool bP);
    
    string getName(){return name;}
    bool isLoaded(){return bIsLoaded;}
    void fadeOut(float duration = 1);
    void fadeIn(float duration = 1);
    void updateFade(ofEventArgs& a);
    void enableGuiListeners(bool e =true);
    ofEvent<void> guiNeedUpdateEvent;
    ofEvent<void> fadeEndEvent;
protected:
    bool bIsLoaded = false;
    bool bHasListeners = false;
    string name;
private:
    enum _fadeStatus{
        FADE_NONE,
        FADE_IN,
        FADE_OUT
    }fadeStatus = FADE_NONE;
    void fade(_fadeStatus newStatus, float duration);
//    bool bIsFading = false, bIsFadeIn = false;
    ofxTween2 fadeOutTween;
    void stopFade();
    void fadeEnd(int & i);
    void volumeChanged(float& f);
    void panChanged(float& f);
};