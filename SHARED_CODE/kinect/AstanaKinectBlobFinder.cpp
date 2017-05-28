#include "AstanaKinectBlobFinder.h"
#include "ofxTimeMeasurements.h"

#define DEPTH_WIDTH 512
#define DEPTH_HEIGHT 424
#define DEPTH_SIZE DEPTH_WIDTH * DEPTH_HEIGHT
//--------------------------------------------------------------
#ifdef ASTANA_USE_THREAD
AstanaKinectBlobFinder::AstanaKinectBlobFinder():bNewFrame(true) {
	startThread();
}
#else 
AstanaKinectBlobFinder::AstanaKinectBlobFinder(){}
#endif // ASTANA_USE_THREAD
//--------------------------------------------------------------
AstanaKinectBlobFinder::~AstanaKinectBlobFinder() {
#ifdef ASTANA_USE_THREAD
	toAnalyze.close();
	analyzed.close();
	waitForThread(true);
#endif
}
//--------------------------------------------------------------
void AstanaKinectBlobFinder::setup(int width, int height) {

	kinect.open();
	kinect.initDepthSource();
	kinect.initColorSource();

	parameters.setName("Blob Finder");
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
	parameters.add(preprocess);

	ofParameterGroup trackerParams;
	trackerParams.setName("Blob Tracker");
	trackerParams.add(trackerPersistence.set("Tracker Persistance", 15, 0, 100));
	trackerParams.add(trackerMaxDist.set("Tracker Max Dist", 32, 0, 500));
	parameters.add(trackerParams);

	trackerPerst = trackerPersistence.newListener(this, &AstanaKinectBlobFinder::trackerPersistenceChanged);
	trackerMxDListener = trackerMaxDist.newListener(this, &AstanaKinectBlobFinder::trackerMaxDistChanged);
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
	parameters.add(drawParams);

	contourFinder.getTracker().setPersistence(trackerPersistence);
	contourFinder.getTracker().setMaximumDistance(trackerMaxDist);

	bIsSetup = true;
}
//--------------------------------------------------------------
void AstanaKinectBlobFinder::trackerPersistenceChanged(int& i) {
	contourFinder.getTracker().setPersistence(trackerPersistence);
}
//--------------------------------------------------------------
void AstanaKinectBlobFinder::trackerMaxDistChanged(int& i) {
	contourFinder.getTracker().setMaximumDistance(trackerMaxDist);
}
//--------------------------------------------------------------
void AstanaKinectBlobFinder::update() {
	TS_START("Kinect Update");
	kinect.update();
	TS_STOP("Kinect Update");

	if (kinect.isFrameNew()) {
		auto p = kinect.getDepthSource()->getPixels();
#ifdef ASTANA_USE_THREAD
		toAnalyze.send(p);
#else
		analyze(p);
#endif
	}
#ifdef ASTANA_USE_THREAD
	bNewFrame = false;
	while (analyzed.tryReceive(currentBlobsMiddle) ){
		bNewFrame = true;
		mutex.lock();
		currentBlobsMiddle.swap(currentBlobsFront);
		mutex.unlock();
	}
	//*
	if (bNewFrame) {
	//	rawImage.update();
		//grayImage.update();
		mutex.lock();
		if (!thresholdedTex.isAllocated()) {
			thresholdedTex.allocate(thresholdedPix);
		}
		thresholdedTex.loadData(thresholdedPix);//  update();
		mutex.unlock();
	}//*/
#endif
}
//--------------------------------------------------------------
void AstanaKinectBlobFinder::analyze(ofShortPixels& p) {
	TS_START("BLOB FINDER UPDATE");
	if (!bIsSetup)return;

	if (p.getWidth() != thresholdedPix.getWidth() || p.getHeight() != thresholdedPix.getHeight()) {
		cout << "Allocating imgs" << endl;
		grayImage.allocate(p.getWidth(), p.getHeight(), OF_IMAGE_GRAYSCALE);
		//thresholded.allocate(p.getWidth(), p.getHeight(), OF_IMAGE_GRAYSCALE);
		thresholdedPix.allocate(p.getWidth(), p.getHeight(), OF_IMAGE_GRAYSCALE);
		rawImage.allocate(p.getWidth(), p.getHeight(), OF_IMAGE_GRAYSCALE);
	}
	TS_START("memcpy");
	memcpy(grayImage.getPixels().getData(), p.getData(), p.size() * sizeof(unsigned short));
	memcpy(rawImage.getPixels().getData(), p.getData(), p.size() * sizeof(unsigned short));
	TS_STOP("memcpy");
	TS_START("blur");
	if (bBlur) {
		ofxCv::blur(grayImage, blurAmount);
	}
	TS_STOP("blur");
	TS_START("thresholding");
	int nPix = grayImage.getPixels().size();
	auto d = grayImage.getPixels().getData();
	auto t = thresholdedPix.getData();
	for (int i = 0; i < nPix; i++) {
		if (d[i] > nearThreshold && d[i] < farThreshold) {
			t[i] = 255;
		}
		else {
			t[i] = 0;
		}
	}
	TS_STOP("thresholding");
	TS_START("erode/dilate");
	TS_START("erode0");
	for (int i = 0; i < nErode0; i++) {
		ofxCv::erode(thresholdedPix);
	}
	TS_STOP("erode0");
	TS_START("dilate0");
	for (int i = 0; i < nDilate0; i++) {
		ofxCv::dilate(thresholdedPix);
	}
	TS_STOP("dilate0");
	TS_START("erode1");
	for (int i = 0; i < nErode1; i++) {
		ofxCv::erode(thresholdedPix);
	}
	TS_STOP("erode1");
	TS_START("dilate1");
	for (int i = 0; i < nDilate1; i++) {
		ofxCv::dilate(thresholdedPix);
	}
	TS_STOP("dilate1");
	TS_STOP("erode/dilate");

//	thresholded.getPixels() = thresholdedPix;
	//memcpy(thresholdedPix.getData(), thresholdedPix.getData(), thresholdedPix.size() * sizeof(unsigned short));


	TS_START("Find Blobs");
	if (bFindBlobs) {
		contourFinder.setInvert(invert);
		contourFinder.setFindHoles(holes);
		contourFinder.setMinArea(minArea);
		contourFinder.setMaxArea(maxArea);
		contourFinder.findContours(ofxCv::toCv(thresholdedPix));
	}
	TS_STOP("Find Blobs");
	TS_START("Blob Processing");
	auto tracker = contourFinder.getTracker();
	auto currentLabels = tracker.getCurrentLabels();
	auto previousLabels = tracker.getPreviousLabels();
	auto newLabels = tracker.getNewLabels();
	auto deadLabels = tracker.getDeadLabels();
	mutex.lock();
	previousBlobs = currentBlobsFront;
	prevLabelIndex = labelIndex;
	AstanaBlobGroup& allBlobs = currentBlobsBack[ASTANA_ALL_BLOBS];
	AstanaBlobGroup& newBlobs = currentBlobsBack[ASTANA_NEW_BLOBS];
	AstanaBlobGroup& ghostBlobs = currentBlobsBack[ASTANA_GHOST_BLOBS];
	AstanaBlobGroup& killedBlobs = currentBlobsBack[ASTANA_KILLED_BLOBS];
	AstanaBlobGroup& movedBlobs = currentBlobsBack[ASTANA_MOVED_BLOBS];
	AstanaBlobGroup& scaledBlobs = currentBlobsBack[ASTANA_SCALED_BLOBS];
	AstanaBlobGroup& mergedBlobs = currentBlobsBack[ASTANA_MERGED_BLOBS];
	allBlobs.clear();
	newBlobs.clear();
	ghostBlobs.clear();
	killedBlobs.clear();
	movedBlobs.clear();
	scaledBlobs.clear();
	mergedBlobs.clear();

	//Crear objetos con todos los blobs.
	labelIndex.clear();
	for (int i = 0; i < contourFinder.size(); i++) {
		ofPoint center = ofxCv::toOf(contourFinder.getCenter(i));
		auto label = contourFinder.getLabel(i);
		labelIndex[label] = i;
		allBlobs.push_back(make_shared<AstanaBlob>());
		allBlobs.back()->polyline = contourFinder.getPolyline(i);
		allBlobs.back()->boundingRect = ofxCv::toOf(contourFinder.getBoundingRect(i));
		allBlobs.back()->center = ofxCv::toOf(contourFinder.getCenter(i));
		allBlobs.back()->area = contourFinder.getContourArea(i);
		allBlobs.back()->label = label;
		allBlobs.back()->age = contourFinder.getTracker().getAge(label);
	}
	// objetos solo con blobs nuevos
	for (int i = 0; i < newLabels.size(); i++) {
		if (labelIndex.count(newLabels[i])) {
			newBlobs.push_back(allBlobs[labelIndex[newLabels[i]]]);
		}
	}
	// objetos con elementos recientemente muertos(killed) y que ya llevan un tiempo muertos(ghost)
	for (int i = 0; i < deadLabels.size(); i++) {
//		if (tracker.existsPrevious(deadLabels[i])) {
		auto label = deadLabels[i];
		if(prevLabelIndex.count(label)){
			auto ind = prevLabelIndex[label];
			auto& pb = previousBlobs[ASTANA_ALL_BLOBS];
			if (ind >= 0 && ind < pb.size()) {
				killedBlobs.push_back(pb[ind]);
			}
		}
		else if(tracker.existsCurrent(label)){
			ghostBlobs.push_back(make_shared<AstanaBlob>());
			ghostBlobs.back()->boundingRect = ofxCv::toOf(tracker.getCurrent(label));
			ghostBlobs.back()->center = ghostBlobs.back()->boundingRect.getCenter().xy();
			ghostBlobs.back()->label = label;
			ghostBlobs.back()->age = tracker.getAge(label);
		}
//		ghostBlobs.push_back(allBlobs[labelIndex[deadLabels[i]]]);
	}
	// objetos que se han movido mas que el minimo determinado por blobMinVel
	// o que su area ha cambiado mas que blobMinAreaDiff
	for (int i = 0; i < currentLabels.size(); i++) {
		auto& label = currentLabels[i];
		if (tracker.existsPrevious(label) && labelIndex.count(label)) {
			auto a = allBlobs[labelIndex[label]];
			auto vel = ofxCv::toOf(tracker.getCurrent(label)).getCenter() - ofxCv::toOf(tracker.getPrevious(label)).getCenter();
			if (glm::length2(vel) > blobMinVel) {
				a->vel = vel.xy();
				movedBlobs.push_back(a);
			}
			if (prevLabelIndex.count(label) && previousBlobs.count(ASTANA_ALL_BLOBS)) {
				auto& pb = previousBlobs[ASTANA_ALL_BLOBS];
				if (prevLabelIndex[label] >= 0 && prevLabelIndex[label] < pb.size())
					a->areaDiff = a->area - pb[prevLabelIndex[label]]->area;
				if (a->areaDiff > blobMinAreaDiff) {
					scaledBlobs.push_back(a);
				}
			}
		}
	}
	auto addUnique = [&](AstanaBlobGroup& group, shared_ptr<AstanaBlob>& a) {
		bool bFound = false;
		for (auto& g : group) {
			if (g == a) {
				bFound = true;
				break;
			}
		}
		if (!bFound) {
			group.push_back(a);
		}
	};
	TS_START("merged blobs");
	for (auto& k : killedBlobs) {
		for (auto& a : allBlobs) {
			if (a->boundingRect.inside(k->boundingRect)) {
				a->mergedWith = k;
				k->mergedWith = a;
				addUnique(mergedBlobs, a);
				addUnique(mergedBlobs, k);
				break;
			}
		}	
	}
	TS_STOP("merged blobs");
	mutex.unlock();
	TS_START("Blobs notifications");
	if(newLabels.size()) ofNotifyEvent(newBlobEvent);
	if(killedBlobs.size())  ofNotifyEvent(killedBlobEvent);
	if(movedBlobs.size()) ofNotifyEvent(onMoveBlobEvent);
	if (scaledBlobs.size()) ofNotifyEvent(onScaleBlobEvent);
	if(mergedBlobs.size()) ofNotifyEvent(onMergeBlobEvent);

	TS_STOP("Blob Processing");
	TS_STOP("Blobs notifications");

	TS_STOP("BLOB FINDER UPDATE");
}
//--------------------------------------------------------------
void AstanaKinectBlobFinder::threadedFunction() {
	ofShortPixels pixels;
	while (toAnalyze.receive(pixels)) {
		analyze(pixels);
		analyzed.send(std::move(currentBlobsBack));
	}
}
//--------------------------------------------------------------
void AstanaKinectBlobFinder::drawTracker() {
	if (bDrawTrackerLabels) {
		ofSetColor(ofColor::red);
		for (auto& a : getAllBlobs()) {
			string msg = ofToString(a->label) + ":" + ofToString(a->age);
			ofDrawBitmapString(msg, a->center);
		}
	}
}
string getBlobsAsString(string titulo, vector< shared_ptr<AstanaBlob> >& b) {
	stringstream ss;
	ss << titulo << " [" << b.size() << "] : ";
	for (auto& a : b) {
		ss << a->label << ", ";
	}
	ss << endl;
	return ss.str();
}
//--------------------------------------------------------------
void AstanaKinectBlobFinder::drawDebug() {
	if (bDrawDebug) {
		string s;
		s += getBlobsAsString("ALL   ", getAllBlobs());
		s += getBlobsAsString("NEW   ", getNewBlobs());
		s += getBlobsAsString("MOVED ", getMovedBlobs());
		s += getBlobsAsString("SCALED", getScaledBlobs());
		s += getBlobsAsString("MERGED", getMergedBlobs());
		s += getBlobsAsString("KILLED", getKilledBlobs());
		s += getBlobsAsString("GHOST ", getGhostBlobs());

		ofBitmapFont f;
		auto r = f.getBoundingBox(s, 0, 0);

		ofDrawBitmapStringHighlight(s, 20, ofGetHeight() - 20 - r.height);
	}
}
//--------------------------------------------------------------
void AstanaKinectBlobFinder::draw() {
	ofSetColor(255, 255, 255);
	if (thresholdedTex.isAllocated()) {
		thresholdedTex.draw(0, DEPTH_HEIGHT);
	}
	if (bDrawGhosts) {
		TS_START("ghosts");
		ofPushStyle();
		ofSetColor(120);
		ofSetLineWidth(1);
		ofNoFill();
		if (currentBlobsFront.count(ASTANA_GHOST_BLOBS)) {
			for (auto& b : currentBlobsFront[ASTANA_GHOST_BLOBS]) {
				if (b)	ofDrawRectangle(b->boundingRect);
			}
		}
		TS_STOP("rects");
		ofPopStyle();
	}
	drawTracker();
	drawPolylines();
	drawRects();
	drawDebug();
}
//--------------------------------------------------------------
void AstanaKinectBlobFinder::drawRects() {
	if (bDrawRects) {
		TS_START("rects");
		ofPushStyle();
		ofSetColor(ofxCv::yellowPrint);
		ofSetLineWidth(1);
		ofNoFill();
		if (currentBlobsFront.count(ASTANA_ALL_BLOBS)) {
			for (auto& b : currentBlobsFront[ASTANA_ALL_BLOBS]) {
				if (b)	ofDrawRectangle(b->boundingRect);
			}
		}
		TS_STOP("rects");
		ofPopStyle();
	}
}
//--------------------------------------------------------------
void AstanaKinectBlobFinder::drawPolylines() {
	if (bDrawPolylines) {
		TS_START("polylines")
			ofPushStyle();
		ofSetColor(ofxCv::magentaPrint);
		ofSetLineWidth(3);
		if (currentBlobsFront.count(ASTANA_ALL_BLOBS)) {
			for (auto& b : currentBlobsFront[ASTANA_ALL_BLOBS]) {
				if (b)	b->polyline.draw();
			}
		}
		TS_STOP("polylines");
		ofPopStyle();
	}
}
//--------------------------------------------------------------
vector< shared_ptr<AstanaBlob> >& AstanaKinectBlobFinder::getBlobs(AstanaBlobType type) {
	if (currentBlobsFront.count(type)) {
		return currentBlobsFront[type];
	}
	return dummyBlobs;
}
//--------------------------------------------------------------
vector< shared_ptr<AstanaBlob> >& AstanaKinectBlobFinder::getAllBlobs() {
	return getBlobs(ASTANA_ALL_BLOBS);
}
//--------------------------------------------------------------
vector< shared_ptr<AstanaBlob> >& AstanaKinectBlobFinder::getNewBlobs() {
	return getBlobs(ASTANA_NEW_BLOBS);
}
//--------------------------------------------------------------
vector< shared_ptr<AstanaBlob> >& AstanaKinectBlobFinder::getMovedBlobs() {
	return getBlobs(ASTANA_MOVED_BLOBS);
}
//--------------------------------------------------------------
vector< shared_ptr<AstanaBlob> >& AstanaKinectBlobFinder::getScaledBlobs() {
	return getBlobs(ASTANA_SCALED_BLOBS);
}
//--------------------------------------------------------------
vector< shared_ptr<AstanaBlob> >& AstanaKinectBlobFinder::getMergedBlobs() {
	return getBlobs(ASTANA_MERGED_BLOBS);
}
//--------------------------------------------------------------
vector< shared_ptr<AstanaBlob> >& AstanaKinectBlobFinder::getKilledBlobs() {
	return getBlobs(ASTANA_KILLED_BLOBS);
}
//--------------------------------------------------------------
vector< shared_ptr<AstanaBlob> >& AstanaKinectBlobFinder::getGhostBlobs() {
	return getBlobs(ASTANA_GHOST_BLOBS);
}
