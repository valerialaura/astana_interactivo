#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxGui.h"

#include "AstanaKinectBlobFinder.h"


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

private:
	bool bIsSetup;
	vector<ofEventListener> listeners;
};