#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxGui.h"
#include "ofxCv.h"
#include "avatarLogo.h"

#include "ofxKinectForWindows2.h"
#include "AstanaKinectBlobFinder.h"

class ofApp : public ofBaseApp {
    
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
  
	AstanaKinectBlobFinder blobFinder;

	ofxPanel gui;
    
    bool bAllocatedImgs;
  
	void onNewBlobs();
	void onKillBlobs();
	void onMovedBlobs();
	ofSoundPlayer newBlobSound, killBlobSound,movedBlobSound;
	ofParameter<bool> bDrawBlobFinder;
	avatarLogo avatar1;
	int vergui;
	vector<avatarLogo> avatarList;

};
