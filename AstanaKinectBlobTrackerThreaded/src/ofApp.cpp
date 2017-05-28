#include "ofApp.h"

#define DEPTH_WIDTH 512
#define DEPTH_HEIGHT 424
#define DEPTH_SIZE DEPTH_WIDTH * DEPTH_HEIGHT


//--------------------------------------------------------------
void ofApp::setup() {
	ofSetLogLevel(OF_LOG_VERBOSE);

	TIME_SAMPLE_SET_FRAMERATE(60.0f); //set the app's target framerate (MANDATORY)
									  //specify where the widget is to be drawn
	TIME_SAMPLE_SET_DRAW_LOCATION(TIME_MEASUREMENTS_TOP_RIGHT); //specify a drawing location (OPTIONAL)
	TIME_SAMPLE_SET_AVERAGE_RATE(0.1);	//averaging samples, (0..1],
										//1.0 gets you no averaging at all
										//use lower values to get steadier readings
	TIME_SAMPLE_DISABLE_AVERAGE();	//disable averaging
	TIME_SAMPLE_SET_REMOVE_EXPIRED_THREADS(true);



	blobFinder.setup(DEPTH_WIDTH, DEPTH_HEIGHT);
	string bfXml = "blobFinder_settings.xml";
	gui.setup("BlobFinder", bfXml);
	gui.add(bDrawBlobFinder.set("Draw Blob Finder", true));
	gui.add(blobFinder.parameters);
	gui.setSize(300, 400);
	gui.setWidthElements(300);
	gui.loadFromFile(bfXml);
	

	newBlobSound.load("Glass.aiff");
	killBlobSound.load("Morse.aiff");
	movedBlobSound.load("Frog.aiff");

	ofAddListener(blobFinder.newBlobEvent, this, &ofApp::onNewBlobs);
	ofAddListener(blobFinder.killedBlobEvent, this, &ofApp::onKillBlobs);
	ofAddListener(blobFinder.onMoveBlobEvent, this, &ofApp::onMovedBlobs);
	ofAddListener(blobFinder.onScaleBlobEvent, this, &ofApp::onScaledBlobs);
	ofAddListener(blobFinder.onMergeBlobEvent, this, &ofApp::onMergedBlobs);

}
//--------------------------------------------------------------
void ofApp::update() {
	blobFinder.update();
}
//--------------------------------------------------------------
void ofApp::onScaledBlobs() {}
//--------------------------------------------------------------
void ofApp::onMergedBlobs() {}
//--------------------------------------------------------------
void ofApp::onNewBlobs() {
	newBlobSound.play();
	for (auto& b : blobFinder.getAllBlobs()) {
		if (b)	cout << b->polyline.size() << endl;
	}
}
//--------------------------------------------------------------
void ofApp::onKillBlobs() {
	killBlobSound.play();
}
//--------------------------------------------------------------
void ofApp::onMovedBlobs() {
	movedBlobSound.play();
}
//--------------------------------------------------------------
void ofApp::draw() {
	TSGL_START("Draw");
	if (bDrawBlobFinder) {
		TS_START("blobfinder");
		blobFinder.draw();
		TS_STOP("blobfinder");
	}
	TS_START("bitmapstring");
	stringstream ss;
	ss << "fps : " << ofGetFrameRate() << endl;	
	ofDrawBitmapStringHighlight(ss.str(), 20, 20);
	TS_STOP("bitmapstring");
	TS_START("GUI");
	gui.draw();
	TS_STOP("GUI");
	TSGL_STOP("Draw");

	TIME_SAMPLE_GET_INSTANCE()->setUiScale(2.0); //x2 the size for 4k screens
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
	
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
	
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
	
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
	
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
	
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {
	
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {
	
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {
	
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {
	
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {
	
}
