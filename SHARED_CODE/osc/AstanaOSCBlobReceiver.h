#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxGui.h"
#include "AstanaBlobsOSCConvert.h"
#include "AstanaBaseHasBlobs.h"
#include "AstanaBlobFinderParams.h"
#include "ofxOscParameterSync.h"

#define PORT 12345
#define NUM_MSG_STRINGS 20

class AstanaOSCBlobReceiver: public AstanaBaseHasBlobs, public AstanaBlobFinderParams {
public:

	void setup();
	void draw();
	void drawGui();

	ofxOscReceiver receiver;
	ofEvent<void> anyBlobEvent;
	AstanaBlobCollection& getBlobsCollection();
private:
	void hostIpChanged(string&);
	AstanaBlobCollection current, previous;
	void update(ofEventArgs& a);
	ofxPanel gui;
	ofParameter<unsigned int> port;
	ofParameter<string> hostIp;
	//ofParameter<bool> bDrawDebug, bDrawGhost, bDrawRects, bDrawPolylines, bDrawLabels, bEnableDraw;
	ofxOscParameterSync sync;
	bool bIsSyncSetup = false;
	bool bIsSetup = false;
	ofEventListener updateListener;
	string senderIp ="";
};
