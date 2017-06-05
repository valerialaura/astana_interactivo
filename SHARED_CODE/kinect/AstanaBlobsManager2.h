#pragma once
#include "AstanaKinectBlobFinder.h"
#include "AstanaBaseHasBlobs.h"
#include "ofxGui.h"
class AstanaBlobsManager2: public AstanaKinectBlobFinder {
public:
	AstanaBlobsManager2();
	~AstanaBlobsManager2();

	void setup();
	
	void draw();
	void drawDebug();
	void drawGui();
	
protected:
	void update(ofEventArgs&);
	
	ofxPanel gui;
	ofParameter<bool>bDrawMerged;
	ofParameter<bool> bDrawActive, bDrawKilled, bDrawGhosts, bDrawMoved, bDrawScaled, bDrawNew, bUseOsc;
private:
	vector<ofEventListener> listeners;

	

};