#pragma once

#include "ofMain.h"

#include "NoiseGenerator.h"
#include "AstanaLowPassFilter.h"
#include "AstanaDigitalDelay.h"
#include "ofxSoundObjects.h"
#include "ofxGui.h"
#include "ofxBasicSoundPlayer.h"
#include "waveformDraw.h"

//#define USE_THREADED
#ifdef USE_THREADED
class ofApp : public ofBaseApp, public ofThread{
#else
class ofApp : public ofBaseApp{
#endif
public:
#ifdef USE_THREADED
	~ofApp();
#endif
	void setup();
	void update();
	void draw();

	void exit();

	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	ofSoundStream soundStream;
	
	// these are all subclasses of ofSoundObject
	NoiseGenerator noise;
	AstanaLowPassFilter filter;
	AstanaDigitalDelay delay;
	
    ofxSoundOutput output;
    	
	//ofTrueTypeFont font;
	// these are all subclasses of ofSoundObject
	ofxBasicSoundPlayer player;
	waveformDraw wave;

	ofxPanel gui;
#ifdef USE_THREADED
	void threadedFunction();
	std::condition_variable condition;
	void stop();
#endif
};
