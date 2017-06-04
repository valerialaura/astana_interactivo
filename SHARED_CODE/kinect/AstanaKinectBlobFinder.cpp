#include "AstanaKinectBlobFinder.h"	
#include "AstanaDraw.h"

//--------------------------------------------------------------
AstanaKinectBlobFinder::AstanaKinectBlobFinder():bNewFrame(true) {
	startThread();
}
//--------------------------------------------------------------
AstanaKinectBlobFinder::~AstanaKinectBlobFinder() {
	//ofBufferToFile("filebuffer.txt", fileBuffer);
	toAnalyze.close();
	analyzed.close();
	waitForThread(true);
}
//--------------------------------------------------------------
void AstanaKinectBlobFinder::setup(){//int width, int height) {

	kinect.open();
	kinect.initDepthSource();
	kinect.initColorSource();

	setupParams();

	trackerPerst = trackerPersistence.newListener(this, &AstanaKinectBlobFinder::trackerPersistenceChanged);
	trackerMxDListener = trackerMaxDist.newListener(this, &AstanaKinectBlobFinder::trackerMaxDistChanged);

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
	kinect.update();

	if (kinect.isFrameNew()) {
		auto p = kinect.getDepthSource()->getPixels();
		toAnalyze.send(p);
	}
	bNewFrame = false;
	while (analyzed.tryReceive(currentBlobsMiddle) ){
		bNewFrame = true;
		mutex.lock();
		currentBlobsMiddle.swap(currentBlobsFront);
		mutex.unlock();
	}
	if (bNewFrame) {
		mutex.lock();
		if (!thresholdedTex.isAllocated()) {
			thresholdedTex.allocate(thresholdedPix);
		}
		thresholdedTex.loadData(thresholdedPix);
		mutex.unlock();
		notifyEvents();
	}
}
//--------------------------------------------------------------
void AstanaKinectBlobFinder::analyze(ofShortPixels& p) {
	if (!bIsSetup)return;

	if (p.getWidth() != thresholdedPix.getWidth() || p.getHeight() != thresholdedPix.getHeight()) {
		cout << "Allocating imgs" << endl;
		grayImage.allocate(p.getWidth(), p.getHeight(), OF_IMAGE_GRAYSCALE);
		
		thresholdedPix.allocate(p.getWidth(), p.getHeight(), OF_IMAGE_GRAYSCALE);
		rawImage.allocate(p.getWidth(), p.getHeight(), OF_IMAGE_GRAYSCALE);
	}
	
	memcpy(grayImage.getPixels().getData(), p.getData(), p.size() * sizeof(unsigned short));
	memcpy(rawImage.getPixels().getData(), p.getData(), p.size() * sizeof(unsigned short));
	
	
	if (bBlur) {
		ofxCv::blur(grayImage, blurAmount);
	}
	
	
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
	
	
	
	for (int i = 0; i < nErode0; i++) {
		ofxCv::erode(thresholdedPix);
	}
	
	
	for (int i = 0; i < nDilate0; i++) {
		ofxCv::dilate(thresholdedPix);
	}
	
	
	for (int i = 0; i < nErode1; i++) {
		ofxCv::erode(thresholdedPix);
	}
	
	
	for (int i = 0; i < nDilate1; i++) {
		ofxCv::dilate(thresholdedPix);
	}
	
	
	if (bFindBlobs) {
		contourFinder.setInvert(invert);
		contourFinder.setFindHoles(holes);
		contourFinder.setMinArea(minArea);
		contourFinder.setMaxArea(maxArea);
		contourFinder.findContours(ofxCv::toCv(thresholdedPix));
	}
	
	
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
		allBlobs.back()->polyline.simplify(polySimplify);
		allBlobs.back()->boundingRect = ofxCv::toOf(contourFinder.getBoundingRect(i));
		allBlobs.back()->center = ofxCv::toOf(contourFinder.getCenter(i));
		allBlobs.back()->area = contourFinder.getContourArea(i);
		allBlobs.back()->label = label + offsetLabels;
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
			ghostBlobs.back()->label = label +offsetLabels;
			ghostBlobs.back()->age = tracker.getAge(label);
		}
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
	// objetos que se han fusionado (merged)
	for (auto& k : killedBlobs) {
		for (auto& a : allBlobs) {
			if (a->boundingRect.inside(k->boundingRect)) {
				a->mergedWith = k;
				k->mergedWith = a;
				addUnique(mergedBlobs, a);
//				addUnique(mergedBlobs, k);
				break;
			}
		}	
	}
	//stringstream ss;
	//ss << "=================================================" << endl;
	//for(auto&g : currentBlobsBack){
	//	ss << "------------" << AstanaToString(g.first) << endl;
	//	for(auto& a: g.second){
	//		ss << a->label << endl;
	//	}
	//}
	//fileBuffer.append(ss.str());

	notifyEvents();
	mutex.unlock();

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
	if(bDrawTrackerLabels) AstanaDraw::drawLabels(currentBlobsFront);
}
//--------------------------------------------------------------
void AstanaKinectBlobFinder::drawDebug() {
	if(bDrawDebug) AstanaDraw::drawDebug(currentBlobsFront);
}
//--------------------------------------------------------------
void AstanaKinectBlobFinder::draw() {
	ofSetColor(255, 255, 255);
	if (thresholdedTex.isAllocated()) {
		thresholdedTex.draw(0, DEPTH_HEIGHT);
	}
	if (bDrawGhosts) AstanaDraw::drawGhosts(currentBlobsFront);
	drawTracker();
	drawPolylines();
	drawRects();
	drawDebug();
}
//--------------------------------------------------------------
void AstanaKinectBlobFinder::drawRects() {
	if (bDrawRects) AstanaDraw::drawRects(currentBlobsFront);
}
//--------------------------------------------------------------
void AstanaKinectBlobFinder::drawPolylines() {
	if (bDrawPolylines) AstanaDraw::drawPolylines(currentBlobsFront);
}
//--------------------------------------------------------------
AstanaBlobCollection& AstanaKinectBlobFinder::getBlobsCollection() {
	return currentBlobsFront;
}
