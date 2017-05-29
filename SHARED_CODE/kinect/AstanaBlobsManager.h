#pragma once
#include "AstanaKinectBlobFinder.h"
#include "AstanaOSCBlobReceiver.h"
#include "AstanaBaseHasBlobs.h"
#include "ofxGui.h"
class AstanaBlobsManager: public AstanaBaseHasBlobs {
public:
	AstanaKinectBlobFinder blobFinder;
	AstanaOSCBlobReceiver receiver;

	void setup();
	void update();
	void draw();
	void drawGui();

	ofEvent<void> newBlobEvent;
	ofEvent<void> killedBlobEvent;
	ofEvent<void> onMoveBlobEvent;
	ofEvent<void> onScaleBlobEvent;
	ofEvent<void> onMergeBlobEvent;
	ofEvent<void> anyBlobEvent;
	AstanaBlobCollection& getBlobsCollection();
	
protected:
	void onFinderAnyBlob();
	void onReceiverAnyBlob();
	ofxPanel gui;
	ofParameter<glm::vec2>receiverOffset;
	
	void mergeBlobs();
private:
	bool bFinderBlobsReady = false;
	bool bReceiverBlobsReady = false;
	vector<ofEventListener> listeners;
	AstanaBlobCollection blobs;

};