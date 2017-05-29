#pragma once
#include "ofxOsc.h"
#include "AstanaBlobFinderTypes.h"
class AstanaBlobsOSCConvert {
private:
	//ofPolyline polyline;
	//ofRectangle boundingRect;
	//glm::vec2 center;
	//double area;
	//double areaDiff;
	//glm::vec2 vel;
	//unsigned int label;
	//shared_ptr<AstanaBlob> mergedWith = nullptr;
	//unsigned int age;
	static void addBlobGroup(ofxOscBundle& bundle, AstanaBlobGroup& blobs, bool bReference, bool bDead,  string addressPrefix) {
	
		ofxOscMessage m;
		for (auto& a : blobs) {
			string baseAddress = addressPrefix + "/" + ofToString(a->label);
			m.setAddress(baseAddress);
			if (bReference) {
				m.addInt64Arg(a->label);
				bundle.addMessage(m);
				m.clear();
			}
			else {
				m.addInt64Arg(a->label);
				m.addInt64Arg(a->age);
				if (!bDead) {
					auto p = a->polyline.getVertices();
					if (p.size() > 0) {
						ofBuffer buffer;
						buffer.allocate(p.size() * sizeof(glm::vec3));
						memcpy(buffer.getData(), &p[0], buffer.size());
						m.addBlobArg(buffer);
					}
					m.addDoubleArg(a->area);
					m.addDoubleArg(a->areaDiff);
					if (auto spt = a->mergedWith.lock()) {
						m.addInt64Arg(spt->label);
					}
					else {
						m.addInt64Arg(-1);
					}
				}
				bundle.addMessage(m);
				m.clear();
				m.setAddress(baseAddress + "/boundingRect");
				m.addFloatArg(a->boundingRect.x);
				m.addFloatArg(a->boundingRect.y);
				m.addFloatArg(a->boundingRect.width);
				m.addFloatArg(a->boundingRect.height);
				bundle.addMessage(m);
				m.clear();
				if (!bDead) {
					m.setAddress(baseAddress + "/vel");
					m.addFloatArg(a->vel.x);
					m.addFloatArg(a->vel.y);
					bundle.addMessage(m);
					m.clear();
				}
			}
		}
	}
public:
	static void toOSC(ofxOscSender& sender, AstanaBlobCollection& blobs) {
		 ofxOscBundle bundle;
		 addBlobGroup(bundle, blobs[ASTANA_ALL_BLOBS],    false, false, "/all");
		 addBlobGroup(bundle, blobs[ASTANA_NEW_BLOBS],    true, false, "/new");
		 addBlobGroup(bundle, blobs[ASTANA_GHOST_BLOBS],  false, true, "/ghost");
		 addBlobGroup(bundle, blobs[ASTANA_KILLED_BLOBS], false, false, "/killed");
		 addBlobGroup(bundle, blobs[ASTANA_MOVED_BLOBS],  true, false, "/moved");
		 addBlobGroup(bundle, blobs[ASTANA_SCALED_BLOBS], true, false, "/scaled");
		 addBlobGroup(bundle, blobs[ASTANA_MERGED_BLOBS], false, false, "/merged");
		 sender.sendBundle(bundle);
	}
	static void toBlobs(ofxOscReceiver& receiver, AstanaBlobCollection& blobs) {
		blobs.clear();
		while(receiver.hasWaitingMessages()){
			ofxOscMessage m;
			receiver.getNextMessage(m);
			cout << m.getAddress() << endl;
			for (int i = 0; i < m.getNumArgs(); i++) {
				cout << m.getArgAsString(i) << "  ";
			}
			cout << endl;
		}
	}
	
};