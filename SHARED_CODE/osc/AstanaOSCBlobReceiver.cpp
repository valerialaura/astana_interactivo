#include "AstanaOSCBlobReceiver.h"
//--------------------------------------------------------------
void AstanaOSCBlobReceiver::setup() {
	// listen on the given port
	gui.setup("Astana OSC Receiver", "osc_receiver_settings.xml");
	gui.add(port.set("Port", PORT, 10000, 20000));
	cout << "listening for osc messages on port " << PORT << "\n";
	bIsSetup = receiver.setup(PORT);
	if (bIsSetup) {
		updateListener = ofEvents().update.newListener(this, &AstanaOSCBlobReceiver::update);
	}
}

//--------------------------------------------------------------
void AstanaOSCBlobReceiver::update(ofEventArgs& a) {
	previous = current;
	current.clear();
	AstanaBlobsOSCConvert::toBlobs(receiver, current);
	/*
	 (receiver.hasWaitingMessages()) {
		// get the next message
		ofxOscMessage m;
		receiver.getNextMessage(m);

		// check for mouse moved message
		if (m.getAddress() == "/mouse/position") {
			// both the arguments are float's
			mouseXf = m.getArgAsFloat(0);
			mouseYf = m.getArgAsFloat(1);
		}
		// check for mouse button message
		else if (m.getAddress() == "/mouse/button") {
			// first argument is int32, second is a string
			mouseButtonInt = m.getArgAsInt32(0);
			mouseButtonState = m.getArgAsString(1);
		}
		// check for an image being sent (note: the size of the image depends greatly on your network buffer sizes - if an image is too big the message won't come through )
		else if (m.getAddress() == "/image") {
			ofBuffer buffer = m.getArgAsBlob(0);
			receivedImage.load(buffer);
		}
		else {
			// unrecognized message: display on the bottom of the screen
			string msg_string;
			msg_string = m.getAddress();
			msg_string += ":";
			for (int i = 0; i < m.getNumArgs(); i++) {
				// get the argument type
				msg_string += " ";
				msg_string += m.getArgTypeName(i);
				msg_string += ":";
				// display the argument - make sure we get the right type
				if (m.getArgType(i) == OFXOSC_TYPE_INT32) {
					msg_string += ofToString(m.getArgAsInt32(i));
				}
				else if (m.getArgType(i) == OFXOSC_TYPE_FLOAT) {
					msg_string += ofToString(m.getArgAsFloat(i));
				}
				else if (m.getArgType(i) == OFXOSC_TYPE_STRING) {
					msg_string += m.getArgAsString(i);
				}
				else {
					msg_string += "unknown";
				}
			}
			// add to the list of strings to display
			msg_strings[current_msg_string] = msg_string;
			timers[current_msg_string] = ofGetElapsedTimef() + 5.0f;
			current_msg_string = (current_msg_string + 1) % NUM_MSG_STRINGS;
			// clear the next line
			msg_strings[current_msg_string] = "";
		}
	}
	 //*/
}


//--------------------------------------------------------------
void AstanaOSCBlobReceiver::draw() {


}
