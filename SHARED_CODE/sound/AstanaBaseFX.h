#pragma once
#include "ofMain.h"
#include "ofxSoundObjects.h"
class AstanaBaseFX: public ofxSoundObject {
public:
	AstanaBaseFX() {}
	virtual ~AstanaBaseFX(){}

	ofParameterGroup parameters;
protected:
	ofMutex mutex;
};