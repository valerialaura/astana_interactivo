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
class AstanaSoundManager :public AstanaBaseHasBlobs {
public:
    AstanaSoundManager();
    virtual ~AstanaSoundManager();
    
    void setup(string folderPath, shared_ptr<AstanaBlobsManager>mng = nullptr);
	void close();

	void enableGui(bool e = true);
	void disableGui();
	void toggleGui();
	bool isGuiEnabled() { return bGuiEnabled; }

    void playNextInGroup(string groupName);
    void playNextTrack();
    void stopAll();
    void setActiveGroup(string groupName);
	void playSample(const string& folderName, const string& sampleName);


	float getFadeEscenaDuration() { return fadeEscenaDuration; }
	shared_ptr<AstanaSoundTextura> getTexturas(){return texturas;}
	virtual AstanaBlobCollection& getBlobsCollection() { return blobs; }
protected:
	void setBlobManager(shared_ptr<AstanaBlobsManager>mng);
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

	ofxPanel gui, blobMappingGui;
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
		ofParameter<float> soundParam;
		AstanaBlobParam blobParam;
	};

	map<unsigned int, map<string, AstanaSoundBlobLink> > soundBlobLinks;
	// label blob, map<soundParamName, link>

	void updateSoundBlobLinks(AstanaBlobType t);

	void setAvailableSoundParams();
	void setAvailableParamUse(const string& paramName, bool bUse);
	string getNextAvailableParamName();
	bool hasNextAvailableParamName();

	bool addLink(unsigned int label, AstanaBlobParam blobParam);
	bool removeLink(unsigned int label);//, AstanaBlobParam blobParam);
	//int findSoundBlobLinkByLabel(unsigned int label);
	map<AstanaBlobParam, ofParameter<float> > mapMinVals, mapMaxVals;
	map<string, bool> availableParams;

	ofMutex mutex;

	ofParameter<float>dummyFloatParam;

	AstanaBlobCollection blobs;
};