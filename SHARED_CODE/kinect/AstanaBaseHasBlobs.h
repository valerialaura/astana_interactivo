#pragma once
#include "AstanaBlobFinderTypes.h"
class AstanaBaseHasBlobs {
public:
	ofEvent<void> newBlobEvent;
	ofEvent<void> killedBlobEvent;
	ofEvent<void> onMoveBlobEvent;
	ofEvent<void> onScaleBlobEvent;
	ofEvent<void> onMergeBlobEvent;
	ofEvent<void> anyBlobEvent;

	virtual AstanaBlobCollection& getBlobsCollection() = 0;
	
	bool notifyEvents() {
		bool n = false;
		if (getNewBlobs().size()) { ofNotifyEvent(newBlobEvent); n = true; }
		if (getMovedBlobs().size()) { ofNotifyEvent(onMoveBlobEvent);  n = true;}
		if (getScaledBlobs().size()) { ofNotifyEvent(onScaleBlobEvent);  n = true;}
		if (getMergedBlobs().size()) { ofNotifyEvent(onMergeBlobEvent);  n = true;}
		if (getKilledBlobs().size()) { ofNotifyEvent(killedBlobEvent);  n = true;}

		if (getAllBlobs().size() || getKilledBlobs().size() || getGhostBlobs().size()) { ofNotifyEvent(anyBlobEvent);  n = true;}
		return n;
	}
	//--------------------------------------------------------------
	vector< shared_ptr<AstanaBlob> >& getBlobs(AstanaBlobType type) {
		if (getBlobsCollection().size()) {
			if (getBlobsCollection().count(type)) {
				return getBlobsCollection()[type];
			}
		}
		return dummyBlobs;
	}
	//--------------------------------------------------------------
	vector< shared_ptr<AstanaBlob> >& getAllBlobs() {
		return getBlobs(ASTANA_ALL_BLOBS);
	}
	//--------------------------------------------------------------
	vector< shared_ptr<AstanaBlob> >& getNewBlobs() {
		return getBlobs(ASTANA_NEW_BLOBS);
	}
	//--------------------------------------------------------------
	vector< shared_ptr<AstanaBlob> >& getMovedBlobs() {
		return getBlobs(ASTANA_MOVED_BLOBS);
	}
	//--------------------------------------------------------------
	vector< shared_ptr<AstanaBlob> >& getScaledBlobs() {
		return getBlobs(ASTANA_SCALED_BLOBS);
	}
	//--------------------------------------------------------------
	vector< shared_ptr<AstanaBlob> >& getMergedBlobs() {
		return getBlobs(ASTANA_MERGED_BLOBS);
	}
	//--------------------------------------------------------------
	vector< shared_ptr<AstanaBlob> >& getKilledBlobs() {
		return getBlobs(ASTANA_KILLED_BLOBS);
	}
	//--------------------------------------------------------------
	vector< shared_ptr<AstanaBlob> >& getGhostBlobs() {
		return getBlobs(ASTANA_GHOST_BLOBS);
	}
private:
	AstanaBlobGroup dummyBlobs;
};