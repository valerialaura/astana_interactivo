#pragma once
#include "AstanaKinectBlobFinder.h"
#include "AstanaOSCBlobReceiver.h"
#include "AstanaBaseHasBlobs.h"
#include "ofxGui.h"
class AstanaBlobsManager: public AstanaBaseHasBlobs, public ofThread {
public:
	AstanaBlobsManager();
	~AstanaBlobsManager();

	void setup();
	
	void draw();
	void drawDebug();
	void drawGui();

	AstanaBlobCollection& getBlobsCollection();
	
protected:
	AstanaKinectBlobFinder blobFinder;
	AstanaOSCBlobReceiver receiver;
	void update(ofEventArgs&);
	void onFinderAnyBlob();
	void onReceiverAnyBlob();
	ofxPanel gui;
//	ofParameter<glm::vec2>receiverOffset;	
	ofParameter<bool>bDrawMerged;
	ofParameter<bool> bDrawActive, bDrawKilled, bDrawGhosts, bDrawMoved, bDrawScaled, bDrawNew;
private:
	void mergeBlobs();
	void threadedFunction();
	ofThreadChannel<AstanaBlobCollection> toMerge, fromMerge;
	bool bFinderBlobsReady = false;
	bool bReceiverBlobsReady = false;
	vector<ofEventListener> listeners;
	AstanaBlobCollection blobsFront, blobsMiddle, blobsBack;


};