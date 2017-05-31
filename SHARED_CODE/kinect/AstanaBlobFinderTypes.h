#pragma once
#include "AstanaBlob.h"

enum AstanaBlobType {
	ASTANA_ALL_BLOBS = 0,
	ASTANA_NEW_BLOBS,
	ASTANA_MOVED_BLOBS,
	ASTANA_SCALED_BLOBS,
	ASTANA_MERGED_BLOBS,
	ASTANA_GHOST_BLOBS,
	ASTANA_KILLED_BLOBS
};
typedef vector< shared_ptr<AstanaBlob> > AstanaBlobGroup;
typedef std::map<AstanaBlobType, AstanaBlobGroup > AstanaBlobCollection;

static string AstanaToString(const AstanaBlobType& type) {
	switch (type) {
	case ASTANA_ALL_BLOBS:    return "ASTANA_ALL_BLOBS";
	case ASTANA_NEW_BLOBS:    return "ASTANA_NEW_BLOBS";
	case ASTANA_MOVED_BLOBS:  return "ASTANA_MOVED_BLOBS";
	case ASTANA_SCALED_BLOBS: return "ASTANA_SCALED_BLOBS";
	case ASTANA_MERGED_BLOBS: return "ASTANA_MERGED_BLOBS";
	case ASTANA_GHOST_BLOBS:  return "ASTANA_GHOST_BLOBS";
	case ASTANA_KILLED_BLOBS: return "ASTANA_KILLED_BLOBS";
	}
}