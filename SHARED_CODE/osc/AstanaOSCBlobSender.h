#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxGui.h"

#include "AstanaKinectBlobFinder.h"
#include "ofxOscParameterSync.h"

class AstanaOSCBlobSender {

public:
	void setup(shared_ptr<AstanaKinectBlobFinder> blobFinder);
	
	void drawGui();
protected:
	ofxPanel gui;
	ofParameter<unsigned int> port;
	ofParameter<string> hostIp;

	shared_ptr<AstanaKinectBlobFinder> blobFinder;
	void onAnyBlobsEvent();
	ofxOscSender sender;
	ofxOscParameterSync sync;
	void update(ofEventArgs&);
private:
	void hostIpChanged(string&);
	bool bIsSetup = false;
	vector<ofEventListener> listeners;

};