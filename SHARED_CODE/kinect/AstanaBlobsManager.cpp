#include "AstanaBlobsManager.h"

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

}
void AstanaBlobsManager::onFinderAnyBlob() {
	bFinderBlobsReady = true;
	mergeBlobs();
}
void AstanaBlobsManager::onReceiverAnyBlob() {
	bReceiverBlobsReady = true;
	mergeBlobs();
}
void AstanaBlobsManager::mergeBlobs(){
	if (bReceiverBlobsReady && bFinderBlobsReady) {
		bReceiverBlobsReady = false;
		bFinderBlobsReady = false;
		blobs.clear();
		auto addBlobGroups = [&](AstanaBlobType t) {
			if (blobFinder.getBlobsCollection().count(t)) {
				for (auto&b : blobFinder.getBlobs(t)) {
					blobs[t].push_back(b);
				}
			}
			if (receiver.getBlobsCollection().count(t)) {
				for (auto&b : receiver.getBlobs(t)) {
					blobs[t].push_back(b);
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
		if (blobs[ASTANA_NEW_BLOBS].size()) { ofNotifyEvent(newBlobEvent); }
		if (blobs[ASTANA_MOVED_BLOBS].size()) { ofNotifyEvent(onMoveBlobEvent); }
		if (blobs[ASTANA_SCALED_BLOBS].size()) { ofNotifyEvent(onScaleBlobEvent); }
		if (blobs[ASTANA_MERGED_BLOBS].size()) { ofNotifyEvent(onMergeBlobEvent); }
		if (blobs[ASTANA_KILLED_BLOBS].size()) { ofNotifyEvent(killedBlobEvent); }

		if (blobs[ASTANA_ALL_BLOBS].size() || blobs[ASTANA_KILLED_BLOBS].size()) { ofNotifyEvent(anyBlobEvent); }
	}
}
void AstanaBlobsManager::update() {
	blobFinder.update();
}
void AstanaBlobsManager::draw() {
	blobFinder.draw();
	ofPushMatrix();
	ofTranslate(receiverOffset);
	receiver.draw();
	ofPopMatrix();
}
void AstanaBlobsManager::drawGui() {
	gui.draw();
	receiver.drawGui();
}
AstanaBlobCollection& AstanaBlobsManager::getBlobsCollection() {
	return blobs;
}
