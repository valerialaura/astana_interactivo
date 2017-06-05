//
//  AstanaSoundPlayer.h
//  gestionSonido
//
//  Created by Roy Macdonald on 22-05-17.
//
//

#pragma once

#include "ofMain.h"
#include "ofxTween2.h"
class AstanaSoundPlayer{
public:
    AstanaSoundPlayer();
    virtual ~AstanaSoundPlayer();
    bool load(std::filesystem::path filePath, bool stream = false);
    
    ofParameter<bool> loopeable;
    
    string getName(){return name;}
    bool isLoaded(){return bIsLoaded;}
    void fadeOut(size_t index , float duration = 1);
    void fadeIn(size_t index , float duration = 1);
    void updateFade(ofEventArgs& a);
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
public:
	void unload();
	size_t play();// when play is called and multiplay enabled a new playing instance is setup and it's index returned;
	void stop(int index = -1);
	
	void setVolume(float vol, size_t index =0 );
	void setPan(float vol, size_t index =0 ); // -1 = left, 1 = right
	void setSpeed(float spd, size_t index =0 );
	void setPaused(bool bP, int index = -1);
	void setLoop(bool bLp);
	void setMultiPlay(bool bMp);
	void setPosition(float pct, size_t index =0 ); // 0 = start, 1 = end;

	float getPosition(size_t index =0) const;
	bool isPlaying() const;
	bool isPlaying(size_t index) const;
	float getSpeed(size_t index =0) const;
	float getPan(size_t index =0) const;
	bool isLoaded() const;
	float getVolume(size_t index =0) const;
	bool getIsLooping() const;

  
	bool canPlayInstance();
	size_t getNumInstances() { return instances.size(); }
private:
	
	static int maxSoundsTotal;
	static int maxSoundsPerPlayer;
	int maxSounds;
	bool bIsLooping = false;
	bool bStreaming = false;
	bool bMultiplay = false;
	vector<ofSoundPlayer> instances;
	




};

