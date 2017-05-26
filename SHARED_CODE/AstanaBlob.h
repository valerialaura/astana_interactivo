#pragma once

#include "ofMain.h"
#include "ofxCv.h"

class AstanaBlob {
public:
	AstanaBlob() {}
	AstanaBlob(ofPolyline pl, cv::Rect r, cv::Point2f p, double a):polyline(pl), boundingRect(r.x, r.y,r.width, r.height), center(p.x,p.y), area(a){}
	ofPolyline polyline;
	ofRectangle boundingRect;
	glm::vec2 center;
	double area;
	glm::vec2 vel;
	unsigned int label;
	//AstanaBlob(ofPolyline* pl, cv::Rect* r, cv::Point2f* p, int l):polyline(pl), rect(r), center(p), label(l) {}
	//ofPolyline* polyline = nullptr;
	//cv::Rect* rect = nullptr;
	//cv::Point2f* center = nullptr;

};