//
//  AstanaSoundPlayer.h
//  gestionSonido
//
//  Created by Roy Macdonald on 22-05-17.
//
//

#pragma once
//#define ASTANA_USAR_DEFAULT_SOUND_PLAYER

#include "ofMain.h"
#ifdef ASTANA_USAR_DEFAULT_SOUND_PLAYER
#define ASTANA_SOUND_PLAYER ofSoundPlayer
#else
#include "ofxSoundObjects.h"
#define ASTANA_SOUND_PLAYER ofxBasicSoundPlayer
#endif
#include "ofxTween2.h"
class AstanaSoundPlayer: public ASTANA_SOUND_PLAYER{
public:
    AstanaSoundPlayer();
    virtual ~AstanaSoundPlayer();
    bool load(std::filesystem::path filePath, bool stream = false);
    
    ofParameter<bool> loopeable;
    //ofParameter<float> volume, pan, speed;
    
 //   void play();
	//void stop();
	//void setPaused(bool bP);
    
    string getName(){return name;}
    bool isLoaded(){return bIsLoaded;}
    void fadeOut(size_t index , float duration = 1);
    void fadeIn(size_t index , float duration = 1);
    void updateFade(ofEventArgs& a);
    //void enableGuiListeners(bool e =true);
    //ofEvent<void> guiNeedUpdateEvent;
    ofEvent<void> fadeEndEvent;
protected:
    bool bIsLoaded = false;
    bool bHasListeners = false;
    string name;
private:
	enum _fadeStatus {
		FADE_NONE,
		FADE_IN,
		FADE_OUT
	};
	map<size_t, _fadeStatus> fadeStatus;

    void fade(_fadeStatus newStatus, size_t index, float duration);

    map<size_t ,ofxTween2> fadeTween;
	// player instance id, tween    
	void stopFade(size_t index);
    void fadeEnd(size_t & i);
};