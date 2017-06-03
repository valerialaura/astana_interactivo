#include "AstanaOSCBlobReceiver.h"
#include "AstanaDraw.h"
//--------------------------------------------------------------
void AstanaOSCBlobReceiver::setup() {
	string xml = "osc_receiver_settings.xml";
	gui.setup("Astana OSC Receiver", xml);
	ofParameterGroup netGroup ("Network");
	netGroup.add(hostIp.set("Remote IP", "localhost"));
	netGroup.add(port.set("Port", 12345, 10000, 20000));
	gui.add(netGroup);

	ofParameterGroup blobCalib("Blobs transform (calibracion)");
	blobCalib.add(bTransformBlobs.set("Transform Blobs", true));
	blobCalib.add(blobTranslation.set("Translate", {0,0}, {-1000,-1000}, {1000,1000}));
	blobCalib.add(blobRotation.set("Rotation", 0, -PI, PI ));
	gui.add(blobCalib);

	setupParams();//"OSC Blob Finder");
	gui.add(parameters);

	gui.loadFromFile(xml);

	bIsSetup = receiver.setup(PORT);
	if (bIsSetup) {
		updateListener = ofEvents().update.newListener(this, &AstanaOSCBlobReceiver::update);
	}
	string s;
	hostIpChanged(s);
}
//--------------------------------------------------------------
void AstanaOSCBlobReceiver::hostIpChanged(string&) {
	if (bIsSetup) {
		sync.setup(parameters, 6666, hostIp, 7777);
		bIsSyncSetup = true;
	}else {
		cout << "No se pudo setear OSC blob sender" << endl;
	}
}
//--------------------------------------------------------------
AstanaBlobCollection& AstanaOSCBlobReceiver::getBlobsCollection() {
	return current;
}
//--------------------------------------------------------------
void AstanaOSCBlobReceiver::update(ofEventArgs& a) {
	previous = current;
	receive();
//	ofNotifyEvent(anyBlobEvent);
	//}
	if (bIsSyncSetup) {
		sync.update();
	}
}
//--------------------------------------------------------------
void AstanaOSCBlobReceiver::draw() {
	if (bEnableDraw && current.size() > 0) {
		if (bDrawDebug) AstanaDraw::drawDebug(current);
		if (bDrawGhosts) AstanaDraw::drawGhosts(current);
		if (bDrawRects) AstanaDraw::drawRects(current);
		if (bDrawPolylines) AstanaDraw::drawPolylines(current);
		if (bDrawTrackerLabels) AstanaDraw::drawLabels(current);
	}
}
//--------------------------------------------------------------
void AstanaOSCBlobReceiver::drawGui() {
	gui.draw();
}
//--------------------------------------------------------------
void AstanaOSCBlobReceiver::checkLabels(AstanaBlobType labelsToCheck, AstanaBlobType in1) {
	for (auto& b : current[labelsToCheck]) {
		if (findLabel(b->label, in1)) {
			cout << "OSCtoBlobs, encontrado label duplicado!" << endl;
		}
	}
};
//--------------------------------------------------------------
shared_ptr<AstanaBlob> AstanaOSCBlobReceiver::findLabel(ofIndexType& label, const AstanaBlobType& type){//, shared_ptr<AstanaBlob>& ptr) {
	if (current.count(type)) {
		for (auto& r : current[type]) {
			if (r) {
				if (r->label == label) {
					//ptr = r;
					//return true;
					return r;
				}
			}
		}
	}
	return nullptr;
}
//--------------------------------------------------------------
void AstanaOSCBlobReceiver::transformBlobs(AstanaBlobType type, bool bHasPolyline){
	if(bTransformBlobs){
		bool bRotate = blobRotation.get() != 0.0f;
		glm::vec3 m = { DEPTH_WIDTH*0.5, DEPTH_HEIGHT*0.5, 0 };
		if(current.count(type)){
			for(auto&b: current[type]){
				if(bHasPolyline){
				for(auto&p: b->polyline.getVertices()){
					if (bRotate) {
						p = glm::rotateZ(p-m, blobRotation.get());
						p += m;
					}
					p.x += blobTranslation->x;
					p.y += blobTranslation->y;
				}
				}
				if(bRotate && bHasPolyline){
					b->boundingRect = b->polyline.getBoundingBox();
					b->center = b->boundingRect.getCenter().xy();
				}else{
					b->boundingRect.x += blobTranslation->x;
					b->boundingRect.y += blobTranslation->y;
					b->center += blobTranslation.get();				
				}
			}
		}
	}
}
//--------------------------------------------------------------
void AstanaOSCBlobReceiver::receive(){
	while (receiver.hasWaitingMessages()) {
		ofxOscMessage m;
		receiver.getNextMessage(m);

		auto address = m.getAddress();
		if (address == "/sync") {
			if ( m.getArgAsInt(0) == 0) {
				current.clear();
				referencias.clear();
			} else {
				// fin blobs.
				if (current.count(ASTANA_ALL_BLOBS)) {
					for (auto& ref : referencias) {
						for (auto& refLabel : ref.second) {
							for (auto& c : current[ASTANA_ALL_BLOBS]) {
								if (c) {
									if (c->label == refLabel) {
										current[ref.first].push_back(c);
										break;
									}
								}
							}
						}
					}
				}
			
				checkLabels(ASTANA_ALL_BLOBS, ASTANA_GHOST_BLOBS);
				checkLabels(ASTANA_ALL_BLOBS, ASTANA_KILLED_BLOBS);
				checkLabels(ASTANA_KILLED_BLOBS, ASTANA_GHOST_BLOBS);

				transformBlobs(ASTANA_ALL_BLOBS, true);
				transformBlobs(ASTANA_KILLED_BLOBS, true);
				transformBlobs(ASTANA_GHOST_BLOBS, false);

				if (current[ASTANA_ALL_BLOBS].size() || current[ASTANA_GHOST_BLOBS].size() || current[ASTANA_KILLED_BLOBS].size()) {
					ofNotifyEvent(anyBlobEvent);
				}
			}
		}
		else {
			if (m.getNumArgs()) {
				AstanaBlobType type;

				bool bTypeFound = false;
				     if (address == "/all")    { type = ASTANA_ALL_BLOBS;    bTypeFound = true; }
				else if (address == "/ghost")  { type = ASTANA_GHOST_BLOBS;  bTypeFound = true; }
				else if (address == "/killed") { type = ASTANA_KILLED_BLOBS; bTypeFound = true; }
				else if (address == "/new")    { type = ASTANA_NEW_BLOBS;    bTypeFound = true; }
				else if (address == "/moved")  { type = ASTANA_MOVED_BLOBS;  bTypeFound = true; }
				else if (address == "/scaled") { type = ASTANA_SCALED_BLOBS; bTypeFound = true; }
				else if (address == "/merged") { type = ASTANA_MERGED_BLOBS; bTypeFound = true; }
				if (bTypeFound) {
					auto nArgs = m.getNumArgs();
						 if (type == ASTANA_ALL_BLOBS)    { if (nArgs != 12) { cout << "ASTANA_ALL_BLOBS numArgs " << nArgs << " debieran ser 12." << endl; } }
					else if (type == ASTANA_GHOST_BLOBS)  { if (nArgs != 6) {  cout << "ASTANA_ALL_BLOBS numArgs " << nArgs << " debieran ser 6." << endl; } }
					else if (type == ASTANA_KILLED_BLOBS) { if (nArgs != 12) { cout << "ASTANA_KILLED_BLOBS numArgs " << nArgs << " debieran ser 12." << endl; } }
					else { if (nArgs != 1) { cout << "ASTANA_REFERENCIAS_BLOBS numArgs " << nArgs << " debiera ser 1." << endl; } }

					if (type == ASTANA_ALL_BLOBS || type == ASTANA_GHOST_BLOBS || type == ASTANA_KILLED_BLOBS) {
						auto& allBlobs = current[type];
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
									}else {
										cout << " OSC to BLOBS: buffer de vertices polylines de tamaño invalido " << buffer.size() << "  " << sizeof(glm::vec3) << endl;
									}
								}
								else {
									cout << " OSC to BLOBS: buffer de vertices vacio" << endl;
								}
							}
						}
					} else if (m.getNumArgs()) {
						referencias[type].push_back(m.getArgAsInt64(0));
					}
				}
				else {
					cout << "oscToBlob -> type not found!" << endl;
				}
			}
			else {
				cout << "oscToBlob -> zero args osc message" << endl;
			}
		}
	}

	//cout << "000";
  //	return (blobs[ASTANA_ALL_BLOBS].size() || blobs[ASTANA_GHOST_BLOBS].size() || blobs[ASTANA_KILLED_BLOBS].size());
}