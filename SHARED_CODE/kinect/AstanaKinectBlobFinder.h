#pragma once
//Kinect Blob Finder
#include "ofMain.h"
#include "ofxCv.h"
#include "AstanaBlob.h"
#include "ofxKinectForWindows2.h"
enum AstanaBlobType {
	ASTANA_ALL_BLOBS = 0,
	ASTANA_NEW_BLOBS,
	ASTANA_MOVED_BLOBS,
	ASTANA_SCALED_BLOBS,
	ASTANA_MERGED_BLOBS,
	ASTANA_GHOST_BLOBS,
	ASTANA_KILLED_BLOBS
};
typedef vector< shared_ptr<AstanaBlob> > AstanaBlobGroup;
typedef std::map<AstanaBlobType, AstanaBlobGroup > AstanaBlobCollection;

#define ASTANA_USE_THREAD
#ifdef ASTANA_USE_THREAD
class AstanaKinectBlobFinder:public ofThread {
#else
class AstanaKinectBlobFinder {
#endif
public:
	AstanaKinectBlobFinder();
	virtual ~AstanaKinectBlobFinder();
	void setup(int width, int height);
	//void update(ofShortPixels& p);
	void draw();
	void drawDebug();
	void drawTracker();
	void update();
	void drawPolylines();
	void drawRects();
	ofParameterGroup	parameters;

	ofEvent<void> newBlobEvent;
	ofEvent<void> killedBlobEvent;
	ofEvent<void> onMoveBlobEvent;
	ofEvent<void> onScaleBlobEvent;
	ofEvent<void> onMergeBlobEvent;

	vector< shared_ptr<AstanaBlob> >& getAllBlobs();
	vector< shared_ptr<AstanaBlob> >& getNewBlobs();
	vector< shared_ptr<AstanaBlob> >& getMovedBlobs();
	vector< shared_ptr<AstanaBlob> >& getScaledBlobs();
	vector< shared_ptr<AstanaBlob> >& getMergedBlobs();
	vector< shared_ptr<AstanaBlob> >& getKilledBlobs();
	vector< shared_ptr<AstanaBlob> >& getGhostBlobs();

protected:
	ofxKFW2::Device kinect;
#ifdef ASTANA_USE_THREAD
	void analyze(ofShortPixels& p);
	ofShortImage analyzedImg;
#endif
	ofShortImage grayImage, rawImage;
	//ofImage thresholded;
	ofPixels thresholdedPix;
	ofTexture thresholdedTex;
	ofxCv::ContourFinder contourFinder;

	ofParameter<bool>	holes;
	ofParameter<bool>	invert;
	ofParameter<int>	minArea;
	ofParameter<int>	maxArea;
	ofParameter<int>	blurAmount;
	
	ofParameter<int>	nDilate0;
	ofParameter<int>	nErode0;
	ofParameter<int>	nDilate1;
	ofParameter<int>	nErode1;
	ofParameter<int>	nearThreshold;
	ofParameter<int>	farThreshold;
	ofParameter<bool>   bFindBlobs;
	ofParameter<int>    trackerPersistence;
	ofParameter<int>	trackerMaxDist;
	ofParameter<float>  blobMinVel;
	ofParameter<double>  blobMinAreaDiff;
	ofParameter<bool>   bBlur;

	ofParameter<bool>   bDrawTrackerLabels;
	ofParameter<bool>   bDrawPolylines;
	ofParameter<bool>   bDrawRects;
	ofParameter<bool>   bDrawGhosts;
	ofParameter<bool>   bDrawDebug;


	void trackerPersistenceChanged(int& i);
	void trackerMaxDistChanged(int& i);
	
private:

#ifdef ASTANA_USE_THREAD
	
	AstanaBlobGroup& getBlobs(AstanaBlobType type);

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
#endif
	ofMutex mutex;
	bool bIsSetup = false;
	AstanaBlobGroup dummyBlobs;
	ofEventListener trackerMxDListener, trackerPerst;
	map<int, int>labelIndex, prevLabelIndex;
};