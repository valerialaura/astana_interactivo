#pragma once

#include "ofMain.h"
enum AstanaBlobParam {
//	ASTANA_BLOB_POLY,
//	ASTANA_BLOB_RECT,
	ASTANA_BLOB_CENTER_X,
	ASTANA_BLOB_CENTER_Y,
	ASTANA_BLOB_AREA,
	ASTANA_BLOB_AREADIFF,
	ASTANA_BLOB_VEL_X,
	ASTANA_BLOB_VEL_Y,
	ASTANA_BLOB_AGE
};
static string AstanaToString(AstanaBlobParam& p) {
	switch(p){
		case ASTANA_BLOB_CENTER_X:return "ASTANA_BLOB_CENTER_X";
		case ASTANA_BLOB_CENTER_Y:return "ASTANA_BLOB_CENTER_Y";
		case ASTANA_BLOB_AREA:	  return "ASTANA_BLOB_AREA";
		case ASTANA_BLOB_AREADIFF:return "ASTANA_BLOB_AREADIFF";
		case ASTANA_BLOB_VEL_X:	  return "ASTANA_BLOB_VEL_X";
		case ASTANA_BLOB_VEL_Y:	  return "ASTANA_BLOB_VEL_Y";
		case ASTANA_BLOB_AGE:	  return "ASTANA_BLOB_AGE";
	}
}
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

	float getParam(const AstanaBlobParam& p) {
		switch (p) {
		case ASTANA_BLOB_CENTER_X:return center.x;
		case ASTANA_BLOB_CENTER_Y:return center.y;
		case ASTANA_BLOB_AREA:return area;
		case ASTANA_BLOB_AREADIFF:return areaDiff;
		case ASTANA_BLOB_VEL_X:return vel.x;
		case ASTANA_BLOB_VEL_Y:return vel.y;
		case ASTANA_BLOB_AGE:return age;
		}
	}
};
