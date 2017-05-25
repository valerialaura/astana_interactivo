//
//  AstanaSoundManager.h
//  gestionSonido
//
//  Created by Roy Macdonald on 21-05-17.
//
//

#pragma once
#include "ofMain.h"
#include "AstanaSoundGroup.h"
#include "AstanaSoundTextura.h"
#include "AstanaSoundSecuencia.h"
#include "AstanaSoundIntervencion.h"
#include "ofxSoundMixer.h"
class AstanaSoundManager {
public:
    AstanaSoundManager();
    virtual ~AstanaSoundManager();
    
    void setup(string folderPath);
	void close();
    void drawGui();
    shared_ptr<AstanaSoundTextura> getTexturas(){return texturas;}
    void playNextInGroup(string groupName);
    void playNextTrack();
    void stopAll();
    
    void setActiveGroup(string groupName);
    
    ofParameter<float>fadeEscenaDuration;
    ofxPanel gui;
	
protected:
    virtual void startActiveGroup();
    void setupGui();
    void load(string folderPath);
    string activeGroup;
    map<string, shared_ptr<AstanaSoundGroup> > groups;
    template<typename T>
    shared_ptr<T> addGroup(string groupFolder);
    shared_ptr<AstanaSoundTextura> texturas;
    ofSoundStream soundStream;
    ofxSoundMixer mixer;
    string folderPath;


};