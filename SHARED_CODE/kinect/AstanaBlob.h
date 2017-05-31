#pragma once

#include "ofMain.h"
enum AstanaBlobParam {
	ASTANA_BLOB_POLY,
	ASTANA_BLOB_RECT,
	ASTANA_BLOB_CENTER,
	ASTANA_BLOB_AREA,
	ASTANA_BLOB_AREADIFF,
	ASTANA_BLOB_VEL,
	ASTANA_BLOB_AGE
};

class AstanaBlob {
public:
	AstanaBlob() {}
	ofPolyline polyline;
	ofRectangle boundingRect;
	glm::vec2 center;
	double area;
	double areaDiff;
	glm::vec2 vel;
	unsigned int label;
	weak_ptr<AstanaBlob> mergedWith;// = nullptr;
	unsigned int mergedWithLabel = 0;
	unsigned int age;
	bool bIsTranslated = false;

	template<typename T>
	T& getParam(const AstanaBlobParam& p) {
		switch (p) {
		case ASTANA_BLOB_POLY: return polyline;
		case ASTANA_BLOB_RECT:return boundingRect;
		case ASTANA_BLOB_CENTER:return center;
		case ASTANA_BLOB_AREA:return area;
		case ASTANA_BLOB_AREADIFF:return areaDiff;
		case ASTANA_BLOB_VEL:return vel;
		case ASTANA_BLOB_AGE:return age;
		}
	}
};
