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
#include "AstanaBlobsManager.h"
class AstanaSoundManager {
public:
    AstanaSoundManager();
    virtual ~AstanaSoundManager();
    
    void setup(string folderPath, shared_ptr<AstanaBlobsManager>mng = nullptr);
	void close();

	void enableGui(bool e = true);
	void disableGui();
	void toggleGui();
	bool isGuiEnabled() { return bGuiEnabled; }
	shared_ptr<AstanaSoundTextura> getTexturas(){return texturas;}
    void playNextInGroup(string groupName);
    void playNextTrack();
    void stopAll();
    void setActiveGroup(string groupName);
	
	float getFadeEscenaDuration() { return fadeEscenaDuration; }

	void setBlobManager(shared_ptr<AstanaBlobsManager>mng);
protected:
	void onNewBlobs();
	void onKillBlobs();
	void onMovedBlobs();
	void onScaledBlobs();
	void onMergedBlobs();
	virtual void startActiveGroup();
    void setupGui();
    void load(string folderPath);

	void keyReleased(ofKeyEventArgs& k);
	void drawGui(ofEventArgs&);

	ofxPanel gui;
	ofParameter<float>fadeEscenaDuration;
    string activeGroup;
    map<string, shared_ptr<AstanaSoundGroup> > groups;
    template<typename T>
    shared_ptr<T> addGroup(string groupFolder);

	shared_ptr<AstanaSoundTextura> texturas;
    ofSoundStream soundStream;
    ofxSoundMixer mixer;
    string folderPath;
	bool bGuiEnabled = false;

	vector<ofEventListener> listeners;
	vector<ofEventListener> guiListeners;
	vector<ofEventListener> blobListeners;

	shared_ptr<AstanaBlobsManager>blobManager;
	
	struct AstanaSoundBlobLink {
		string soundParam;
		//unsigned int blobLabel;
		AstanaBlobParam blobParam;
	};

	//map<string, shared_ptr<AstanaSoundBlobLink> >linkSoundParamMap;
	//map<unsigned int, shared_ptr<AstanaSoundBlobLink> >linkBlobLabelMap;
	map<unsigned int, AstanaSoundBlobLink> soundBlobLinks;
	//int findSoundBlobLinkByLabel(unsigned int label);

};