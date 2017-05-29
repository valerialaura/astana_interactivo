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
	void update(ofEventArgs& a);
	void draw();

	ofxOscReceiver receiver;

	/*int current_msg_string;
	string msg_strings[NUM_MSG_STRINGS];
	float timers[NUM_MSG_STRINGS];

	float mouseXf, mouseYf;
	int mouseButtonInt;
	string mouseButtonState;

	ofImage receivedImage;*/

	ofxPanel gui;
	ofParameter<unsigned int> port;

	AstanaBlobCollection current, previous;
private:
	bool bIsSetup = false;
	ofEventListener updateListener;
};
