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
	static void addBlobGroup(ofxOscBundle& bundle, AstanaBlobGroup& blobs, bool bReference, bool bDead, string addressPrefix) {

		ofxOscMessage m;
		for (auto& a : blobs) {
			string baseAddress = addressPrefix;// +"/" + ofToString(a->label);
			m.setAddress(baseAddress);
			m.addInt64Arg(a->label);//0
			if (!bReference) {
				m.addInt64Arg(a->age);//1
				m.addFloatArg(a->boundingRect.x);
				m.addFloatArg(a->boundingRect.y);
				m.addFloatArg(a->boundingRect.width);
				m.addFloatArg(a->boundingRect.height);
				if (!bDead) {
					m.addDoubleArg(a->area);
					m.addDoubleArg(a->areaDiff);
					if (auto spt = a->mergedWith.lock()) {
						m.addInt64Arg(spt->label);
					}
					else {
						m.addInt64Arg(-1);
					}
					m.addFloatArg(a->vel.x);
					m.addFloatArg(a->vel.y);
					auto p = a->polyline.getVertices();
					if (p.size() > 0) {
						ofBuffer buffer;
						buffer.allocate(p.size() * sizeof(glm::vec3));
						memcpy(buffer.getData(), &p[0], buffer.size());
						m.addBlobArg(buffer);
					}

				}
			}
			bundle.addMessage(m);
			m.clear();
		}
	}
public:
	static void toOSC(ofxOscSender& sender, AstanaBlobCollection& blobs) {
		ofxOscBundle bundle;
		ofxOscMessage m;
		m.setAddress("/sync");
		m.addIntArg(0);
		bundle.addMessage(m);
		m.clear();
		addBlobGroup(bundle, blobs[ASTANA_ALL_BLOBS], false, false, "/all");
		addBlobGroup(bundle, blobs[ASTANA_NEW_BLOBS], true, false, "/new");
		addBlobGroup(bundle, blobs[ASTANA_GHOST_BLOBS], false, true, "/ghost");
		addBlobGroup(bundle, blobs[ASTANA_KILLED_BLOBS], false, false, "/killed");
		addBlobGroup(bundle, blobs[ASTANA_MOVED_BLOBS], true, false, "/moved");
		addBlobGroup(bundle, blobs[ASTANA_SCALED_BLOBS], true, false, "/scaled");
		addBlobGroup(bundle, blobs[ASTANA_MERGED_BLOBS], true, false, "/merged");
		m.setAddress("/sync");
		m.addIntArg(1);
		bundle.addMessage(m);
		m.clear();
		sender.sendBundle(bundle);
	}

	// 0  Int64Arg(a->label)
	// 1  Int64Arg(a->age);
	// 2  FloatArg(a->boundingRect.x);
	// 3  FloatArg(a->boundingRect.y);
	// 4  FloatArg(a->boundingRect.width);
	// 5  FloatArg(a->boundingRect.height);					
	// 6  DoubleArg(a->area);
	// 7  DoubleArg(a->areaDiff);
	// 8  Int64Arg(spt->label);//mergedWith
	// 9  FloatArg(a->vel.x);
	// 10 FloatArg(a->vel.y);
	// 11 BlobArg(buffer);// vector<glm::vec3>

	
};