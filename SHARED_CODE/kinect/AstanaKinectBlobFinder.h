#pragma once
//Kinect Blob Finder
#include "ofMain.h"
#include "ofxCv.h"
#include "ofxKinectForWindows2.h"
#include "AstanaBlobFinderTypes.h"
#include "AstanaBaseHasBlobs.h"
#include "AstanaBlobFinderParams.h"
class AstanaKinectBlobFinder:public ofThread, public AstanaBaseHasBlobs, public AstanaBlobFinderParams {
public:
	AstanaKinectBlobFinder();
	virtual ~AstanaKinectBlobFinder();
	void setup();//int width, int height);
	void draw();
	void drawDebug();
	void drawTracker();
	void update();
	void drawPolylines();
	void drawRects();
	//ofParameterGroup	parameters;
	
	AstanaBlobCollection& getBlobsCollection();
	
protected:
	ofxKFW2::Device kinect;
	void analyze(ofShortPixels& p);
	ofShortImage analyzedImg;
	ofShortImage grayImage, rawImage;
	ofPixels thresholdedPix;
	ofTexture thresholdedTex;
	ofxCv::ContourFinder contourFinder;

	//ofParameter<bool>	holes;
	//ofParameter<bool>	invert;
	//ofParameter<int>	minArea;
	//ofParameter<int>	maxArea;
	//ofParameter<int>	blurAmount;
	//
	//ofParameter<int>	nDilate0;
	//ofParameter<int>	nErode0;
	//ofParameter<int>	nDilate1;
	//ofParameter<int>	nErode1;
	//ofParameter<int>	nearThreshold;
	//ofParameter<int>	farThreshold;
	//ofParameter<bool>   bFindBlobs;
	//ofParameter<int>    trackerPersistence;
	//ofParameter<int>	trackerMaxDist;
	//ofParameter<float>  blobMinVel;
	//ofParameter<double>  blobMinAreaDiff;
	//ofParameter<bool>   bBlur;
	//ofParameter<unsigned int> offsetLabels;
	//ofParameter<float> polySimplify;
	
	/*ofParameter<bool>   bDrawTrackerLabels;
	ofParameter<bool>   bDrawPolylines;
	ofParameter<bool>   bDrawRects;
	ofParameter<bool>   bDrawGhosts;
	ofParameter<bool>   bDrawDebug;*/

	void trackerPersistenceChanged(int& i);
	void trackerMaxDistChanged(int& i);
	
private:

		void threadedFunction();
	//thread channels para traspasar info entre un thread y otro de manera mas sencilla
	ofThreadChannel<ofShortPixels> toAnalyze;
	ofThreadChannel<AstanaBlobCollection> analyzed;

	//triple buffering para los blobs, para irme a la segura.
	AstanaBlobCollection currentBlobsFront;
	AstanaBlobCollection currentBlobsMiddle;
	AstanaBlobCollection currentBlobsBack;

	AstanaBlobCollection previousBlobs;
	
	bool bNewFrame;
	ofMutex mutex;
	bool bIsSetup = false;
	ofEventListener trackerMxDListener, trackerPerst;
	map<int, int>labelIndex, prevLabelIndex;
};