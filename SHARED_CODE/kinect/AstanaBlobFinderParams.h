#pragma once

#include "ofMain.h"
#define DEPTH_WIDTH 512
#define DEPTH_HEIGHT 424
#define DEPTH_SIZE DEPTH_WIDTH * DEPTH_HEIGHT

class AstanaBlobFinderParams{
public:
	ofParameterGroup	parameters;

protected:
	void setupParams(string name = "Blob Finder"){
		parameters.setName(name);
		ofParameterGroup finderParams;
		finderParams.setName("CV Contour Finder");
		finderParams.add(bFindBlobs.set("Find Blobs", false));
		finderParams.add(holes.set("Find Holes", false));
		finderParams.add(invert.set("Invert Blob Finder", false));
		finderParams.add(maxArea.set("maxArea", 150, 0, 2 * DEPTH_SIZE / 3));
		finderParams.add(minArea.set("minArea", 10, 0, DEPTH_SIZE / 16));
		parameters.add(finderParams);

		ofParameterGroup preprocess;
		preprocess.setName("Pre procesos");
		preprocess.add(farThreshold.set("farThreshold", 10000, 0, 10000));
		preprocess.add(nearThreshold.set("nearThreshold", 0, 0, 10000));
		preprocess.add(bBlur.set("Activar Blur", true));
		preprocess.add(blurAmount.set("blurAmount", 2, 0, 10));
		preprocess.add(nErode0.set("nErode0", 1, 0, 5));
		preprocess.add(nDilate0.set("nDilate0", 1, 0, 5));
		preprocess.add(nErode1.set("nErode1", 1, 0, 5));
		preprocess.add(nDilate1.set("nDilate1", 1, 0, 5));
		preprocess.add(polySimplify.set("Blob Simplify", 0.3, 0, 5));
		parameters.add(preprocess);

		ofParameterGroup trackerParams;
		trackerParams.setName("Blob Tracker");
		trackerParams.add(offsetLabels.set("Offset Labels", 0, 0, std::numeric_limits<unsigned int>::max()));

		trackerParams.add(trackerPersistence.set("Tracker Persistance", 15, 0, 100));
		trackerParams.add(trackerMaxDist.set("Tracker Max Dist", 32, 0, 500));
		parameters.add(trackerParams);

		ofParameterGroup triggerParams;
		triggerParams.setName("Event trigger");
		triggerParams.add(blobMinVel.set("Blobs Min Vel", 1, 0, 10));
		triggerParams.add(blobMinAreaDiff.set("Blobs Min Area Diff", 2, 0, 30));
		parameters.add(triggerParams);

		ofParameterGroup drawParams;
		drawParams.setName("DRAW");
		drawParams.add(bDrawTrackerLabels.set("Draw Labels", true));
		drawParams.add(bDrawPolylines.set("Draw Polylines", true));
		drawParams.add(bDrawRects.set("Draw Rects", true));
		drawParams.add(bDrawGhosts.set("Draw Ghosts", true));
		drawParams.add(bDrawDebug.set("Draw Debug", true));
		drawParams.add(bEnableDraw.set("Enable Draw", true));
		parameters.add(drawParams);


	}


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
	ofParameter<unsigned int> offsetLabels;
	ofParameter<float> polySimplify;

	ofParameter<bool>   bDrawTrackerLabels;
	ofParameter<bool>   bDrawPolylines;
	ofParameter<bool>   bDrawRects;
	ofParameter<bool>   bDrawGhosts;
	ofParameter<bool>   bDrawDebug;
	ofParameter<bool>   bEnableDraw;



};