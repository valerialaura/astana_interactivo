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
		m.addIntArg(1);
		bundle.addMessage(m);
		m.clear();
		addBlobGroup(bundle, blobs[ASTANA_ALL_BLOBS], false, false, "/all");
		addBlobGroup(bundle, blobs[ASTANA_NEW_BLOBS], true, false, "/new");
		addBlobGroup(bundle, blobs[ASTANA_GHOST_BLOBS], false, true, "/ghost");
		addBlobGroup(bundle, blobs[ASTANA_KILLED_BLOBS], false, false, "/killed");
		addBlobGroup(bundle, blobs[ASTANA_MOVED_BLOBS], true, false, "/moved");
		addBlobGroup(bundle, blobs[ASTANA_SCALED_BLOBS], true, false, "/scaled");
		addBlobGroup(bundle, blobs[ASTANA_MERGED_BLOBS], true, false, "/merged");
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

	static bool toBlobs(ofxOscReceiver& receiver, AstanaBlobCollection& blobs) {

		map<AstanaBlobType, vector<unsigned int> >referencias;
		while (receiver.hasWaitingMessages()) {
			ofxOscMessage m;
			receiver.getNextMessage(m);

			auto address = m.getAddress();
			if (address == "/sync") {
				blobs.clear();
			}
			else {
				if (m.getNumArgs()) {
					AstanaBlobType type;

					bool bTypeFound = false;
					if (address == "/all") {
						type = ASTANA_ALL_BLOBS;
						bTypeFound = true;
					}
					else if (address == "/ghost") {
						type = ASTANA_GHOST_BLOBS;
						bTypeFound = true;
					}
					else if (address == "/killed") {
						type = ASTANA_KILLED_BLOBS;
						bTypeFound = true;
					}
					else if (address == "/new") {
						type = ASTANA_NEW_BLOBS;
						bTypeFound = true;
					}
					else if (address == "/moved") {
						type = ASTANA_MOVED_BLOBS;
						bTypeFound = true;
					}
					else if (address == "/scaled") {
						type = ASTANA_SCALED_BLOBS;
						bTypeFound = true;
					}
					else if (address == "/merged") {
						type = ASTANA_MERGED_BLOBS;
						bTypeFound = true;
					}
					if (bTypeFound) {
						auto nArgs = m.getNumArgs();
						if (type == ASTANA_ALL_BLOBS){
							if (nArgs != 12) {
								cout << "ASTANA_ALL_BLOBS numArgs " << nArgs << " debieran ser 12." << endl;
							}
						}else if (type == ASTANA_GHOST_BLOBS){
							if (nArgs != 6) {
								cout << "ASTANA_ALL_BLOBS numArgs " << nArgs << " debieran ser 6." << endl;
							}
						}else if (type == ASTANA_KILLED_BLOBS){
							if (nArgs != 12) {
								cout << "ASTANA_KILLED_BLOBS numArgs " << nArgs << " debieran ser 12." << endl;
							}
						}
						else {
							if (nArgs != 1) {
								cout << "ASTANA_REFERENCIAS_BLOBS numArgs " << nArgs << " debiera ser 1." << endl;
							}
						}
						if (type == ASTANA_ALL_BLOBS || type == ASTANA_GHOST_BLOBS || type == ASTANA_KILLED_BLOBS) {
							auto& allBlobs = blobs[type];
							allBlobs.push_back(make_shared<AstanaBlob>());
							allBlobs.back()->label = m.getArgAsInt64(0);
							if (m.getNumArgs() > 5) { // not Reference
								allBlobs.back()->age = m.getArgAsInt64(1);
								allBlobs.back()->boundingRect.x = m.getArgAsFloat(2);
								allBlobs.back()->boundingRect.y = m.getArgAsFloat(3);
								allBlobs.back()->boundingRect.width = m.getArgAsFloat(4);
								allBlobs.back()->boundingRect.height = m.getArgAsFloat(5);
								allBlobs.back()->center = allBlobs.back()->boundingRect.getCenter().xy();
								if (m.getNumArgs() > 11) { // not dead
									allBlobs.back()->area = m.getArgAsDouble(6);
									allBlobs.back()->areaDiff = m.getArgAsDouble(7);
									allBlobs.back()->mergedWithLabel = m.getArgAsInt64(8);
									allBlobs.back()->vel.x = m.getArgAsFloat(9);
									allBlobs.back()->vel.y = m.getArgAsFloat(10);
									ofBuffer buffer = m.getArgAsBlob(11);
									if (buffer.size() > 0) {
										if (buffer.size() % sizeof(glm::vec3) == 0) {
											size_t n = buffer.size() / sizeof(glm::vec3);
											vector<glm::vec3> p;
											p.resize(n);
											memcpy(&p[0], buffer.getData(), buffer.size());
											allBlobs.back()->polyline.addVertices(p);
											allBlobs.back()->polyline.setClosed(true);
										}
										else {
											cout << " OSC to BLOBS: buffer de vertices polylines de tamaño invalido " << buffer.size() << "  " << sizeof(glm::vec3) << endl;
										}
									}
									else {
										cout << " OSC to BLOBS: buffer de vertices vacio" << endl;
									}
								}
							}
						}
						else {
							if (m.getNumArgs()) {
								referencias[type].push_back(m.getArgAsInt64(0));
							}
						}
					}

				}
			}
		}
		auto findLabel = [&](ofIndexType& label, const AstanaBlobType& t, shared_ptr<AstanaBlob>& ptr) -> bool {
			if (blobs.count(t)) {
				for (auto& r : blobs[t]) {
					if (r) {
						if (r->label == label) {
							ptr = r;
							return true;
						}
					}
				}
			}
			return false;
		};
		for (auto& r : referencias) {
			for (auto& s : r.second) {
				shared_ptr<AstanaBlob> ptr = nullptr;
				if (findLabel(s, r.first, ptr)) {
					blobs[r.first].push_back(ptr);
				}
			}
		}
		shared_ptr<AstanaBlob> ptr = nullptr;
		auto checkLabels = [&](AstanaBlobType labelsToCheck, AstanaBlobType in1) {
			for (auto& b : blobs[labelsToCheck]) {
				if (findLabel(b->label, in1, ptr)) {
					cout << "OSCtoBlobs, encontrado label duplicado!" << endl;
				}
			}
		};
		checkLabels(ASTANA_ALL_BLOBS, ASTANA_GHOST_BLOBS);
		checkLabels(ASTANA_ALL_BLOBS, ASTANA_KILLED_BLOBS);
		checkLabels(ASTANA_KILLED_BLOBS, ASTANA_GHOST_BLOBS);
		//cout << "000";
		return (blobs[ASTANA_ALL_BLOBS].size() || blobs[ASTANA_GHOST_BLOBS].size() || blobs[ASTANA_KILLED_BLOBS].size());
	} 
};