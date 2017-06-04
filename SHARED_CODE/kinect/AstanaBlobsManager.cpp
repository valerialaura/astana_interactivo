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
	blobsBack.clear();
	auto addBlobGroups = [&](AstanaBlobType t){// bool bAddOffset = false) {
		blobsBack[t] = AstanaBlobGroup();
		if (blobFinder.getBlobsCollection().count(t)) {
			for (auto&b : blobFinder.getBlobs(t)) {
				blobsBack[t].push_back(b);
			}
		}
		if (receiver.getBlobsCollection().count(t)) {
			for (auto&b : receiver.getBlobs(t)) {
				blobsBack[t].push_back(b);
			}
		}
	};

	addBlobGroups(ASTANA_ALL_BLOBS);
	addBlobGroups(ASTANA_NEW_BLOBS);
	addBlobGroups(ASTANA_MOVED_BLOBS);
	addBlobGroups(ASTANA_SCALED_BLOBS);
	addBlobGroups(ASTANA_MERGED_BLOBS);
	addBlobGroups(ASTANA_GHOST_BLOBS);
	addBlobGroups(ASTANA_KILLED_BLOBS);

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
		mutex.lock();
		toMerge.send(b);
		mutex.unlock();
	}
//	bool bNewBlobs = false;
	while (fromMerge.tryReceive(blobsMiddle)) {
	//	bNewBlobs = true;
		mutex.lock();
		blobsFront.clear();
		blobsMiddle.swap(blobsFront);
		notifyEvents();
 		mutex.unlock();
	}
	//if (bNewBlobs) {
		//cout << "Notify events " << endl;
//		mutex.lock();
//		notifyEvents();
	//	mutex.unlock();
	//}

}
//--------------------------------------------------------------
void AstanaBlobsManager::drawDebug(){
	AstanaDraw::drawDebug(getBlobsCollection());
}
//--------------------------------------------------------------
void AstanaBlobsManager::draw() {
	if (bDrawMerged) {
		if (bDrawActive)AstanaDraw::drawGroup(ASTANA_ALL_BLOBS,    getBlobsCollection(), ofColor(120), ofColor::red);
		if (bDrawKilled)AstanaDraw::drawGroup(ASTANA_KILLED_BLOBS, getBlobsCollection(), ofColor(120), ofColor::white);
		if (bDrawGhosts)AstanaDraw::drawGroup(ASTANA_GHOST_BLOBS,  getBlobsCollection(), ofColor(120), ofColor::blue);
		if (bDrawMoved) AstanaDraw::drawGroup(ASTANA_MOVED_BLOBS,  getBlobsCollection(), ofColor(120), ofColor::purple);
		if (bDrawScaled)AstanaDraw::drawGroup(ASTANA_SCALED_BLOBS, getBlobsCollection(), ofColor(120), ofColor::yellow);
		if (bDrawNew)   AstanaDraw::drawGroup(ASTANA_NEW_BLOBS,    getBlobsCollection(), ofColor(120), ofColor::black);
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

