#include "AstanaBlobsManager.h"
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
	gui.add(receiverOffset.set("Offset Blobs OSC", { 520,0 }, {-1000,-1000}, { 1000,1000 }));
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
void AstanaBlobsManager::mergeBlobs(){
	
		blobsBack.clear();
		auto addBlobGroups = [&](AstanaBlobType t) {
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
		fromMerge.send(std::move(blobsBack));
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
		blobsMiddle.swap(blobsFront);
		mutex.unlock();
	}
	if (bNewBlobs) {
		notifyEvents();
	}

}
//--------------------------------------------------------------
void AstanaBlobsManager::draw() {
	blobFinder.draw();
	ofPushMatrix();
	ofTranslate(receiverOffset);
	receiver.draw();
	ofPopMatrix();
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

