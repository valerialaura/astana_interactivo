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
