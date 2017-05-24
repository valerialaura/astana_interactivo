//
//  AstanaSoundGroup.h
//  gestionSonido
//
//  Created by Roy Macdonald on 21-05-17.
//
//

#pragma once
#include "ofMain.h"
#include "AstanaSoundPlayer.h"
#include "ofxGui.h"
#include "ofxSoundMixer.h"
#include "AstanaDigitalDelay.h"
class AstanaSoundManager;
class AstanaSoundGroup{//:public ofxSoundObject{
public:
    AstanaSoundGroup();
    void setManager(AstanaSoundManager* mngr);
    virtual ~AstanaSoundGroup(){}
    virtual bool play();
    virtual bool playNext();
    virtual void pause();
    virtual void reset();
    virtual bool isNextAllowed();
    
    virtual void setup(string folderPath);

    void fadeOutAll(std::function< void() > callback);
    AstanaSoundPlayer* getPlayer(const string& playerName);
    vector<AstanaSoundPlayer> players;
    string getName(){return name;}
    void setName(string name){this->name = name;}
    
    ofParameterGroup parameters;
    ofParameterGroup loopeablesGroup, volumeGroup, panGroup, texturasValidasGroup;
    ofxPanel gui;
    bool isTextura(){return bIsTextura;}
    ofxSoundMixer& getMixer(){return mixer;}
    virtual void setListeners(bool e = true);
    void highlightGui(bool hl);
    bool isPlaying();
    
    AstanaDigitalDelay delayFx;
    
protected:
    void updateGui();
    virtual void setupParameters();
    virtual void loadFolder(string folderPath);
    
    AstanaSoundManager* manager = nullptr;
    size_t current =0;
    ofEventListener currentListener;
    bool bIsTextura = false;
    bool bFolderLoaded = false;
    ofxSoundMixer mixer;
    vector <ofEventListener>listeners;
    bool bListenersEnabled = false;
        vector<ofEventListener> fadeListeners;
private:
    string name;
    void addParameterGroup(ofParameterGroup& pg, string name);
    std::function< void() > fadeOutAllCallback;
    void fadeOutAllEnd();
};