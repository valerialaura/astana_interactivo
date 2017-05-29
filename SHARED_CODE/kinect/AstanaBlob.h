#pragma once

#include "ofMain.h"

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

};