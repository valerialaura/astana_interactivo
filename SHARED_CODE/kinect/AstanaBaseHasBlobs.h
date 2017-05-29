#pragma once
#include "AstanaBlobFinderTypes.h"
class AstanaBaseHasBlobs {
public:
	virtual AstanaBlobCollection& getBlobsCollection() = 0;
	//--------------------------------------------------------------
	vector< shared_ptr<AstanaBlob> >& getBlobs(AstanaBlobType type) {
		if (getBlobsCollection().count(type)) {
			return getBlobsCollection()[type];
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