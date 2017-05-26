#include "AstanaKinectBlobFinder.h"


#define DEPTH_WIDTH 512
#define DEPTH_HEIGHT 424
#define DEPTH_SIZE DEPTH_WIDTH * DEPTH_HEIGHT

void AstanaKinectBlobFinder::setup(int width, int height) {


	parameters.setName("Blob Finder");
	parameters.add(bFindBlobs.set("Find Blobs", false));
	parameters.add(holes.set("Send Holes", false));
	parameters.add(invert.set("invert Blob Finder", false));
	parameters.add(maxArea.set("maxArea", 150, 0, 2 * DEPTH_SIZE / 3));
	parameters.add(minArea.set("minArea", 10, 0, DEPTH_SIZE / 16));

	parameters.add(farThreshold.set("farThreshold", 10000, 0, 10000));
	parameters.add(nearThreshold.set("nearThreshold", 0, 0, 10000));
	parameters.add(blurAmount.set("blurAmount", 2, 0, 10));

	parameters.add(blurAmount1.set("blurAmount1", 2, 0, 10));
	parameters.add(nErode0.set("nErode0", 1, 0, 5));
	parameters.add(nDilate0.set("nDilate0", 1, 0, 5));
	parameters.add(nErode1.set("nErode1", 1, 0, 5));
	parameters.add(nDilate1.set("nDilate1", 1, 0, 5));
	parameters.add(trackerPersistence.set("Tracker Persistance", 15, 0, 100));
	parameters.add(trackerMaxDist.set("Tracker Max Dist", 32, 0, 500));
	trackerPerst = trackerPersistence.newListener(this, &AstanaKinectBlobFinder::trackerPersistenceChanged);
	trackerMxDListener = trackerMaxDist.newListener(this, &AstanaKinectBlobFinder::trackerMaxDistChanged);
	parameters.add(bDrawTrackerLabels.set("Draw tracker labels", true));
	parameters.add(blobMinVel.set("Blobs Min Vel", 1, 0, 10));


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
void AstanaKinectBlobFinder::update(ofShortPixels& p) {
	if (!bIsSetup)return;
	if (p.getWidth() != thresholded.getWidth() || p.getHeight() != thresholded.getHeight()) {
		grayImage.allocate(p.getWidth(), p.getHeight(), OF_IMAGE_GRAYSCALE);
		thresholded.allocate(p.getWidth(), p.getHeight(), OF_IMAGE_GRAYSCALE);
		rawImage.allocate(p.getWidth(), p.getHeight(), OF_IMAGE_GRAYSCALE);
	}
	memcpy(grayImage.getPixels().getData(), p.getData(), p.size() * sizeof(unsigned short));
	memcpy(rawImage.getPixels().getData(), p.getData(), p.size() * sizeof(unsigned short));


	int nPix = grayImage.getPixels().size();
	auto d = grayImage.getPixels().getData();
	auto t = thresholded.getPixels().getData();
	for (int i = 0; i < nPix; i++) {
		if (d[i] > nearThreshold && d[i] < farThreshold) {
			t[i] = 255;
		}
		else {
			t[i] = 0;
		}
	}

	for (int i = 0; i < nErode0; i++) {
		ofxCv::erode(thresholded);
	}
	for (int i = 0; i < nDilate0; i++) {
		ofxCv::dilate(thresholded);
	}
	for (int i = 0; i < nErode1; i++) {
		ofxCv::erode(thresholded);
	}
	for (int i = 0; i < nDilate1; i++) {
		ofxCv::dilate(thresholded);
	}
	if (bFindBlobs) {
		contourFinder.setInvert(invert);
		contourFinder.setFindHoles(holes);
		contourFinder.setMinArea(minArea);
		contourFinder.setMaxArea(maxArea);
		contourFinder.findContours(ofxCv::toCv(thresholded));
	}
	auto tracker = contourFinder.getTracker();
	auto currentLabels = tracker.getCurrentLabels();
	auto previousLabels = tracker.getPreviousLabels();
	auto newLabels = tracker.getNewLabels();
	auto deadLabels = tracker.getDeadLabels();
	mutex.lock();
	allBlobs.clear();
	map<int, int>labelIndex;
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
	}
	newBlobs.clear();
	for (int i = 0; i < newLabels.size(); i++) {
		if (labelIndex.count(newLabels[i])) {
			newBlobs.push_back(allBlobs[labelIndex[newLabels[i]]]);
		}
	}

	for (int i = 0; i < deadLabels.size(); i++) {
		if (tracker.existsPrevious(deadLabels[i])) {
			ofNotifyEvent(killBlobEvent);
	//		killBlobs[i] = allBlobs[labelIndex[deadLabels[i]]];
		}
	}
	movedBlobs.clear();
	for (int i = 0; i < currentLabels.size(); i++) {
		auto& label = currentLabels[i];
		if(tracker.existsPrevious(label) && labelIndex.count(label)){
			auto a = allBlobs[labelIndex[label]];
			auto vel = ofxCv::toOf(tracker.getCurrent(label)).getCenter() - ofxCv::toOf(tracker.getPrevious(label)).getCenter();
			if (glm::length2(vel) > blobMinVel) {
				a->vel = vel.xy();
				movedBlobs.push_back(a);
			}
		}
	}
	//scaledBlobs.clear();
	//for (int i = 0; i < previousLabels.size(); i++) {
	//	if(tracker.existsCurrent(previousLabels[i])){
	//		scaledBlobs.push_back(allBlobs[labelIndex[previousLabels[i]]]);
	//	}
	//}


	mutex.unlock();
	if(newLabels.size()) ofNotifyEvent(newBlobEvent);
	//if(killBlobs.size())  ofNotifyEvent(killBlobEvent);
	if(movedBlobs.size()) ofNotifyEvent(onMoveBlobEvent);
	if(mergedBlob.size()) ofNotifyEvent(onMergeBlobEvent);

	//int blobCount = 0;

	//	if (p.size() > 2) {\
	//		ofxOscMessage b;
	//		b.setAddress("/blob");
	//		ofBuffer buffer;
	//		buffer.allocate(p.size() * sizeof(cv::Point));
	//		memcpy(buffer.getData(), &p[0], buffer.size());
	//
	//		points.clear();
	//		int size = buffer.size() / sizeof(cv::Point);
	//		char *tempData = buffer.getData();
	//		points.resize(size);
	//		memcpy(&points[0], tempData, buffer.size());
	//
	//		b.addBlobArg(buffer);
	//		b.addIntArg(contourFinder.getHole(blobCount) ? 1 : 0);
	//
	//		sender.sendMessage(b);
	//	}
	//	blobCount++;
	//}
}
//--------------------------------------------------------------
void AstanaKinectBlobFinder::drawTracker() {
	auto tracker = contourFinder.getTracker();

	if (bDrawTrackerLabels) {
		ofSetColor(255);
		for (int i = 0; i < contourFinder.size(); i++) {
			ofPoint center = ofxCv::toOf(contourFinder.getCenter(i));
			ofPushMatrix();
			ofTranslate(center.x, center.y);
			int label = contourFinder.getLabel(i);
			string msg = ofToString(label) + ":" + ofToString(tracker.getAge(label));
			ofDrawBitmapString(msg, 0, 0);
			ofVec2f velocity = ofxCv::toOf(contourFinder.getVelocity(i));
			ofScale(5, 5);
			ofDrawLine(0, 0, velocity.x, velocity.y);
			ofPopMatrix();
		}
	}
	else {
		for (int i = 0; i < contourFinder.size(); i++) {
			unsigned int label = contourFinder.getLabel(i);
			// only draw a line if this is not a new label
			if (tracker.existsPrevious(label)) {
				// use the label to pick a random color
				ofSeedRandom(label << 24);
				ofSetColor(ofColor::fromHsb(ofRandom(255), 255, 255));
				// get the tracked object (cv::Rect) at current and previous position
				const cv::Rect& previous = tracker.getPrevious(label);
				const cv::Rect& current = tracker.getCurrent(label);
				// get the centers of the rectangles
				ofVec2f previousPosition(previous.x + previous.width / 2, previous.y + previous.height / 2);
				ofVec2f currentPosition(current.x + current.width / 2, current.y + current.height / 2);
				ofDrawLine(previousPosition, currentPosition);
			}
		}
	}
}
//--------------------------------------------------------------
void AstanaKinectBlobFinder::draw() {
	ofSetColor(255, 255, 255);
	rawImage.update();
	grayImage.update();
	thresholded.update();
	rawImage.draw(0, 0);
	grayImage.draw(DEPTH_WIDTH, 0);
	thresholded.draw(0, DEPTH_HEIGHT);
	ofPushMatrix();
	ofSetColor(255, 0, 255);
	contourFinder.draw();
	ofPopMatrix();


	drawTracker();
}