#include "AstanaBlobsManager.h"
#include "AstanaDraw.h"
AstanaBlobsManager::AstanaBlobsManager() {
	startThread();
}
//--------------------------------------------------------------
AstanaBlobsManager::~AstanaBlobsManager() {
	toMerge.close();
	fromMerge.close();
	waitForThread(true);
}
//--------------------------------------------------------------
void AstanaBlobsManager::setup() {
	blobFinder.setup();
	string bfXml = "blob_manager_settings.xml";
	gui.setup("Blobs Manager", bfXml);
	gui.add(receiverOffset.set("Offset Blobs OSC", { 520,0 }, { -1000,-1000 }, { 1000,1000 }));
	gui.add(bDrawMerged.set("Draw Merged blobs", false));
	gui.add(bDrawActive.set("bDrawActive", true));
	gui.add(bDrawKilled .set("bDrawKilled",true));
	gui.add(bDrawGhosts .set("bDrawGhosts",true));
	gui.add(bDrawMoved  .set("bDrawMoved" ,true));
	gui.add(bDrawScaled .set("bDrawScaled",true));
	gui.add(bDrawNew    .set("bDrawNew"	  ,true));
	
		gui.add(blobFinder.parameters);
	gui.setSize(300, 400);
	gui.setPosition(ofGetWidth() - 340, 40);
	gui.setWidthElements(300);
	gui.loadFromFile(bfXml);

	receiver.setup();
	listeners.clear();
	listeners.push_back(blobFinder.anyBlobEvent.newListener(this, &AstanaBlobsManager::onFinderAnyBlob));
	listeners.push_back(receiver.anyBlobEvent.newListener(this, &AstanaBlobsManager::onReceiverAnyBlob));
	listeners.push_back(ofEvents().update.newListener(this, &AstanaBlobsManager::update));
}
//--------------------------------------------------------------
void AstanaBlobsManager::onFinderAnyBlob() {
	bFinderBlobsReady = true;
}
//--------------------------------------------------------------
void AstanaBlobsManager::onReceiverAnyBlob() {
	bReceiverBlobsReady = true;
}
//--------------------------------------------------------------
void AstanaBlobsManager::mergeBlobs() {
	// aplicar offset a los blobs OSC
	//map<unsigned int, unsigned int>addedLabels;
	//auto addOffset = [&](AstanaBlobType t, bool bOffsetPolyline = true) {
	//	if (receiver.getBlobsCollection().count(t)) {
	//		for (auto&b : receiver.getBlobs(t)) {
	//			if (!addedLabels.count(b->label)) {
	//				addedLabels[b->label] = b->label;
	//				if (bOffsetPolyline) {
	//					for (auto&v : b->polyline.getVertices()) {
	//						v.x += receiverOffset->x;
	//						v.y += receiverOffset->y;
	//					}
	//				}
	//				b->boundingRect.translate(receiverOffset.get());
	//				b->center += receiverOffset.get();
	//			}
	//			else {
	//				cout << "merge blobs add offset, ya existia este blob. ";
	//				switch (t) {
	//				case ASTANA_ALL_BLOBS:
	//					cout << "ASTANA_ALL_BLOBS" << endl;
	//					break;
	//				case ASTANA_GHOST_BLOBS:
	//					cout << "ASTANA_GHOST_BLOBS" << endl;
	//					break;
	//				case ASTANA_KILLED_BLOBS:
	//					cout << "ASTANA_KILLED_BLOBS" << endl;
	//					break;
	//				default:
	//					break;
	//				}
	//			}
	//		}
	//	}
	//};
//	addOffset(ASTANA_ALL_BLOBS);
//	addOffset(ASTANA_GHOST_BLOBS, false);
	//addOffset(ASTANA_KILLED_BLOBS);

	mutex.lock();
	glm::vec2 offset = receiverOffset.get();
	mutex.unlock();
	blobsBack.clear();
	auto addBlobGroups = [&](AstanaBlobType t, bool bAddOffset = false) {
		if (blobFinder.getBlobsCollection().count(t)) {
			for (auto&b : blobFinder.getBlobs(t)) {
				blobsBack[t].push_back(b);
			}
		}
		if (receiver.getBlobsCollection().count(t)) {
			for (auto&b : receiver.getBlobs(t)) {
				blobsBack[t].push_back(b);
				auto bb = blobsBack[t].back();
				if (bAddOffset){
					if (!bb->bIsTranslated) {
						bb->bIsTranslated = true;
						for (auto&v : bb->polyline.getVertices()) {
							v.x += offset.x;
							v.y += offset.y;
						}
						bb->boundingRect.translate(offset);
						bb->center += offset;
					}
					/*else {
						cout << bb->label << " ya estaba offsetiado!" << AstanaToString(t) << endl;
					}*/
				}
			}
		}
	};

	addBlobGroups(ASTANA_ALL_BLOBS,true);
	addBlobGroups(ASTANA_NEW_BLOBS);
	addBlobGroups(ASTANA_MOVED_BLOBS);
	addBlobGroups(ASTANA_SCALED_BLOBS);
	addBlobGroups(ASTANA_MERGED_BLOBS);
	addBlobGroups(ASTANA_GHOST_BLOBS, true);
	addBlobGroups(ASTANA_KILLED_BLOBS, true);

}
//--------------------------------------------------------------
void AstanaBlobsManager::threadedFunction() {

	AstanaBlobCollection b;
	while (toMerge.receive(b)) {
		mergeBlobs();
		blobsMiddle.clear();
		blobsMiddle = blobsBack;
		fromMerge.send(std::move(blobsMiddle));
	}
}
//--------------------------------------------------------------
void AstanaBlobsManager::update(ofEventArgs&) {
	blobFinder.update();
	if (bReceiverBlobsReady && bFinderBlobsReady) {
		AstanaBlobCollection b;
		bReceiverBlobsReady = false;
		bFinderBlobsReady = false;
		toMerge.send(b);
	}
	bool bNewBlobs = false;
	while (fromMerge.tryReceive(blobsMiddle)) {
		bNewBlobs = true;
		mutex.lock();
		blobsFront.clear();
		blobsMiddle.swap(blobsFront);
		mutex.unlock();
	}
	if (bNewBlobs) {
		notifyEvents();
	}

}
//--------------------------------------------------------------
void AstanaBlobsManager::draw() {
	if (bDrawMerged) {
		if (bDrawActive)AstanaDraw::drawGroup(ASTANA_ALL_BLOBS, getBlobsCollection(), ofColor(120), ofColor::red);
		if (bDrawKilled)AstanaDraw::drawGroup(ASTANA_KILLED_BLOBS, getBlobsCollection(), ofColor(120), ofColor::white);
		if (bDrawGhosts)AstanaDraw::drawGroup(ASTANA_GHOST_BLOBS, getBlobsCollection(), ofColor(120), ofColor::blue);
		if (bDrawMoved)AstanaDraw::drawGroup(ASTANA_MOVED_BLOBS, getBlobsCollection(), ofColor(120), ofColor::purple);
		if (bDrawScaled)AstanaDraw::drawGroup(ASTANA_SCALED_BLOBS, getBlobsCollection(), ofColor(120), ofColor::yellow);
		if (bDrawNew)AstanaDraw::drawGroup(ASTANA_NEW_BLOBS, getBlobsCollection(), ofColor(120), ofColor::black);
		AstanaDraw::drawDebug(getBlobsCollection());

	}
	else {
		blobFinder.draw();
		//ofPushMatrix();
		//ofTranslate(receiverOffset);
		receiver.draw();
		//ofPopMatrix();
	}
}
//--------------------------------------------------------------
void AstanaBlobsManager::drawGui() {
	gui.draw();
	receiver.drawGui();
}
//--------------------------------------------------------------
AstanaBlobCollection& AstanaBlobsManager::getBlobsCollection() {
	return blobsFront;
}
//--------------------------------------------------------------

