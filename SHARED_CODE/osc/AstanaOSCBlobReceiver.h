#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxGui.h"
#include "AstanaBlobsOSCConvert.h"

#define PORT 12345
#define NUM_MSG_STRINGS 20

class AstanaOSCBlobReceiver {
public:

	void setup();
	void draw();
	void drawGui();

	ofxOscReceiver receiver;
	AstanaBlobCollection current, previous;
	
private:
	void update(ofEventArgs& a);
	ofxPanel gui;
	ofParameter<unsigned int> port;
	ofParameter<bool> bDrawDebug, bDrawGhost, bDrawRects, bDrawPolylines, bDrawLabels, bEnableDraw;

	bool bIsSetup = false;
	ofEventListener updateListener;
};
