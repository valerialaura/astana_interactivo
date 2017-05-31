#pragma once
#include "ofMain.h"
#include "AstanaBlobFinderTypes.h"
class AstanaDraw {
public:
	//--------------------------------------------------------------
	static void drawLabels(AstanaBlobCollection& blobs) {
		if (blobs.count(ASTANA_ALL_BLOBS)) {
			ofSetColor(ofColor::red);
			for (auto& a : blobs[ASTANA_ALL_BLOBS]) {
				string msg = ofToString(a->label) + ":" + ofToString(a->age);
				ofDrawBitmapString(msg, a->center);
			}
		}
	}
	//--------------------------------------------------------------
	static void drawDebug(AstanaBlobCollection& blobs) {
		string s;
		auto f = [&](AstanaBlobType type, const string& prefix) {
			stringstream ss;
			if (blobs.count(type)) {
				ss << prefix << " [" << blobs[type].size() << "] : ";
				for (auto& a : blobs[type]) {
					ss << a->label << ", ";
				}
			}
			ss << endl;
			s += ss.str();//getBlobsAsString(prefix, blobs[type]);
		};
		f(ASTANA_ALL_BLOBS,   "ALL   ");
		f(ASTANA_NEW_BLOBS,   "NEW   ");
		f(ASTANA_MOVED_BLOBS, "MOVED ");
		f(ASTANA_SCALED_BLOBS,"SCALED");
		f(ASTANA_MERGED_BLOBS,"MERGED");
		f(ASTANA_KILLED_BLOBS,"KILLED");
		f(ASTANA_GHOST_BLOBS, "GHOST ");

			ofBitmapFont font;
			auto r = font.getBoundingBox(s, 0, 0);

			ofDrawBitmapStringHighlight(s, ofGetWidth() - r.width - 20, ofGetHeight() - 20 - r.height);
	}
	//--------------------------------------------------------------
	//--------------------------------------------------------------
	static void draw(AstanaBlobCollection& blobs) {
		drawLabels(blobs);
		drawPolylines(blobs);
		drawRects(blobs);
		drawDebug(blobs);
	}
	//--------------------------------------------------------------
	static void drawGroup(AstanaBlobType type, AstanaBlobCollection& blobs, const ofColor& rectColor = ofColor(120), const ofColor& polyColor = ofColor::red) {
		if (blobs.count(type)) {
			ofPushStyle();
			ofSetColor(rectColor);
			ofSetLineWidth(1);
			ofNoFill();
			for (auto& b : blobs[type]) {
				if (b) {
					ofDrawRectangle(b->boundingRect);
				}
			}
			ofSetColor(polyColor);
			ofSetLineWidth(2);
			for (auto& b : blobs[type]) {
				if (b) {
					b->polyline.draw();
				}
			}
			ofPopStyle();
		}
	}
	static void drawGhosts(AstanaBlobCollection& blobs) {
		if (blobs.count(ASTANA_GHOST_BLOBS)) {
			ofPushStyle();
			ofSetColor(120);
			ofSetLineWidth(1);
			ofNoFill();
			for (auto& b : blobs[ASTANA_GHOST_BLOBS]) {
				if (b)	ofDrawRectangle(b->boundingRect);
			}
			ofPopStyle();
		}
	}
	//--------------------------------------------------------------
	static void drawRects(AstanaBlobCollection& blobs) {
		if (blobs.count(ASTANA_ALL_BLOBS)) {
			ofPushStyle();
			ofSetHexColor(0xffee00);
			ofSetLineWidth(1);
			ofNoFill();
			for (auto& b : blobs[ASTANA_ALL_BLOBS]) {
				if (b)	ofDrawRectangle(b->boundingRect);
			}
			ofPopStyle();
		}
	}
	//--------------------------------------------------------------
	static void drawPolylines(AstanaBlobCollection& blobs) {
		if (blobs.count(ASTANA_ALL_BLOBS)) {
			ofPushStyle();
			ofSetHexColor(0xec008c);
			ofSetLineWidth(3);
			for (auto& b : blobs[ASTANA_ALL_BLOBS]) {
				if (b)	b->polyline.draw();
			}
			ofPopStyle();
		}
	}
};