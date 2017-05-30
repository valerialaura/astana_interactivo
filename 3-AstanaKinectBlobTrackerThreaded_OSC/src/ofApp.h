#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxGui.h"
#include "ofxCv.h"

#include "AstanaKinectBlobFinder.h"
#include "AstanaOSCBlobSender.h"

class ofApp : public ofBaseApp {
    
public:
    void setup();
    void update();
    void draw();
          
	shared_ptr<AstanaKinectBlobFinder >blobFinder;

	AstanaOSCBlobSender sender;
	ofxPanel gui;
    
    bool bAllocatedImgs;
  
	ofParameter<bool> bDrawBlobFinder;

};
