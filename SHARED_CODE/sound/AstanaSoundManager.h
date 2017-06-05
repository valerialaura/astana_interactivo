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
//#include "ofxSoundMixer.h"
#include "AstanaBlobsManager.h"

//#define ASTANA_PRINT_DEBUG
class AstanaSoundManager :protected AstanaBaseHasBlobs, public ofThread {
public:
    AstanaSoundManager();
    virtual ~AstanaSoundManager();
    
    void setup( shared_ptr<AstanaBlobsManager>mng = nullptr);
	void close();

	void enableGui(bool e = true);
	void disableGui();
	void toggleGui();
	bool isGuiEnabled() { return bGuiEnabled; }

    void playNextTrack();
    void stopAll();
    void setActiveGroup(string groupName);
	void playSample(const string& folderName, const string& sampleName);


	float getFadeEscenaDuration() { return fadeEscenaDuration; }
	shared_ptr<AstanaSoundTextura> getTexturas(){return texturas;}
	virtual AstanaBlobCollection& getBlobsCollection() { return blobsFront; }
	void threadedFunction();

	void activarCumparsita();
	void activarElDiaQueMeQuieras();
	void activarNonino();
	void activarSueltos();

protected:
	void update(ofEventArgs&);
	void updateBlobs();
	ofThreadChannel<AstanaBlobCollection> toBlobsUpdate, fromBlobsUpdate;

	void setBlobManager(shared_ptr<AstanaBlobsManager>mng);
	void onAnyBlobs();

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
    //ofxSoundMixer mixer;
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

	//vector<AstanaBlobParam> availableBlobParams;
	//AstanaBlobParam getNextAvailableBlobParam();
	//map<unsigned int, map<string, AstanaSoundBlobLink> > soundBlobLinks;
	// label blob, map<soundParamName, link>

	map<unsigned int, map<string, size_t> > soundBlobLinks;
	// blob label, indexTextura, playerInstanceId
	string getNextTextura();
	string currentTextura;
	void updateSoundBlobLinks(AstanaBlobType t);

	//void setAvailableSoundParams();
	//void setAvailableParamUse(const string& paramName, bool bUse);
	//string getNextAvailableParamName();
	//bool hasNextAvailableParamName();

	bool addLink(unsigned int label);//, AstanaBlobParam blobParam);
	bool removeLink(unsigned int label);//, AstanaBlobParam blobParam);
	//int findSoundBlobLinkByLabel(unsigned int label);
	map<AstanaBlobParam, ofParameter<float> > mapMinVals, mapMaxVals;
	//map<string, bool> availableParams;

	bool isTexturaValida(string tex);

	ofMutex mutex;

	ofParameter<float>dummyFloatParam;

	AstanaBlobCollection blobsFront, blobsMiddle, blobsBack;

	void clearSoundBlobLinks();

	ofParameter<bool>bUsarCentroX, bUsarCentroY, bUsarArea;//, bUsarVelX, bUsarVelY;
	//ofParameter<void>vResetearLinks;


	ofParameter<float> smoothingParams;

#ifdef ASTANA_PRINT_DEBUG
	ofBuffer fileBuffer;
#endif
};