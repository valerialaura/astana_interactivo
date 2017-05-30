#pragma once

#include "ofMain.h"
#include "AstanaBlobsManager.h"


class ofApp : public ofBaseApp {
    
public:
    void setup();
    void update();
    void draw();
      
	void onNewBlobs();
	void onKillBlobs();
	void onMovedBlobs();
	void onScaledBlobs();
	void onMergedBlobs();
	
	ofSoundPlayer newBlobSound, killBlobSound,movedBlobSound;
	
	AstanaBlobsManager blobManager;
};
