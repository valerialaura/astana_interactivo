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
	unsigned int age;
	//AstanaBlob(ofPolyline* pl, cv::Rect* r, cv::Point2f* p, int l):polyline(pl), rect(r), center(p), label(l) {}
	//ofPolyline* polyline = nullptr;
	//cv::Rect* rect = nullptr;
	//cv::Point2f* center = nullptr;

};