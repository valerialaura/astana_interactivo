#pragma once
//Kinect Blob Finder
#include "ofMain.h"
#include "ofxCv.h"
#include "AstanaBlob.h"
class AstanaKinectBlobFinder {
public:

	void setup(int width, int height);
	void update(ofShortPixels& p);
	void draw();
	void drawTracker();
	//void resetBackground();
	ofParameterGroup	parameters;

	ofEvent<void> newBlobEvent;
	ofEvent<void> killBlobEvent;
	ofEvent<void> onMoveBlobEvent;
	//ofEvent<void> onScaleBlobEvent;
	ofEvent<void> onMergeBlobEvent;

	vector< shared_ptr<AstanaBlob> > allBlobs;
	vector< shared_ptr<AstanaBlob> > newBlobs;
	//vector< shared_ptr<AstanaBlob> > killBlobs;
	vector< shared_ptr<AstanaBlob> > movedBlobs;
	//vector< shared_ptr<AstanaBlob> > scaledBlobs;
	vector< shared_ptr<AstanaBlob> > mergedBlob;



protected:
	ofShortImage grayImage, rawImage;
	ofImage thresholded;
	
	ofxCv::ContourFinder contourFinder;

	ofParameter<float>  thresholdValue;
	ofParameter<bool>	holes;
	ofParameter<bool>	invert;
	ofParameter<int>	minArea;
	ofParameter<int>	maxArea;
	ofParameter<float>	mSpacing;
	ofParameter<int>	maxCount;
	ofParameter<int>	blurAmount;
	ofParameter<int>	blurAmount1;
	ofParameter<int>	nDilate0;
	ofParameter<int>	nErode0;
	ofParameter<int>	nDilate1;
	ofParameter<int>	nErode1;
	ofParameter<int>	nearThreshold;
	ofParameter<int>	farThreshold;
	ofParameter<bool>   bFindBlobs;
	ofParameter<int>    trackerPersistence;
	ofParameter<int>	trackerMaxDist;
	ofParameter<bool>   bDrawTrackerLabels;
	ofParameter<float>  blobMinVel;

	void trackerPersistenceChanged(int& i);
	void trackerMaxDistChanged(int& i);
	
	


private:
	ofMutex mutex;
	bool bIsSetup = false;
	ofEventListener trackerMxDListener, trackerPerst;
/*	ofEventListener bgListener;
	bool bNeedsBackgroundReset = true*/;
};