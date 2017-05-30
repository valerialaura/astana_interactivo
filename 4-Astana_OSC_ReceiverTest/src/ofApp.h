#pragma once

#include "ofMain.h"
#include "AstanaOSCBlobReceiver.h"
#define PORT 12345
#define NUM_MSG_STRINGS 20

class ofApp : public ofBaseApp {
public:

	void setup();
	void draw();

	AstanaOSCBlobReceiver receiver;

};
