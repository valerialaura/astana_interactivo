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

	kinect.open();
	kinect.initDepthSource();
	kinect.initColorSource();

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
	ofAddListener(blobFinder.killBlobEvent, this, &ofApp::onKillBlobs);
	ofAddListener(blobFinder.onMoveBlobEvent, this, &ofApp::onMovedBlobs);

}

//--------------------------------------------------------------
void ofApp::update() {
	TS_START("Kinect Update");
	kinect.update();
	TS_STOP("Kinect Update");

	if (kinect.isFrameNew()) {

		blobFinder.update(kinect.getDepthSource()->getPixels());
	}	
}
//--------------------------------------------------------------
void ofApp::onNewBlobs() {
	newBlobSound.play();
	cout << "NEW BLOB  " << blobFinder.newBlobs.size() << endl;
	cout << "ALL BLOBS " << blobFinder.allBlobs.size() << endl;
	for (auto& b : blobFinder.allBlobs) {
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
	cout << "MOVED BLOB " << blobFinder.movedBlobs.size() << endl;
}
//--------------------------------------------------------------
void ofApp::draw() {
	TSGL_START("Draw");
	if (bDrawBlobFinder) {
		TS_START("blobfinder");
		blobFinder.draw();
		TS_STOP("blobfinder");
	}
	if (blobFinder.newBlobs.size()) {
		cout << "DRAW new Blobs: " << blobFinder.newBlobs.size() << endl;
	}
	if (blobFinder.movedBlobs.size()) {
		cout << "DRAW moved Blobs: " << blobFinder.movedBlobs.size() << endl;
	}
	TS_START("polylines")
	ofPushStyle();
	ofSetColor(ofxCv::magentaPrint);
	ofSetLineWidth(2);
	for (auto& b : blobFinder.allBlobs) {
		if(b)	b->polyline.draw();
	}
	/*for (auto& b : blobFinder.newBlobs) {
		if (b) {
			ofPath path;
			path.setFilled(true);
			if (b->polyline.size()) {
				for (auto& v : b->polyline.getVertices()) {
					path.lineTo(v);
				}
			}
		
		path.setFillColor(ofxCv::cyanPrint);
		path.draw();
		}
	}*/
	TS_STOP("polylines");
	ofPopStyle();
	TS_START("bitmapstring");
	stringstream ss;
	ss << "fps : " << ofGetFrameRate() << endl;	
	ss << "allBlobs: " << blobFinder.allBlobs.size() << endl;
	ss << "allBlobs tiene NULL ";
	for (auto& b : blobFinder.allBlobs) {
		if (!b) {
			ss << "TRUE";
			break;
		}
	}
	
	ofDrawBitmapStringHighlight(ss.str(), 20, 20);
	TS_STOP("bitmapstring");
	TS_START("GUI");
	gui.draw();
	TS_STOP("GUI");
	TSGL_STOP("Draw");

	TIME_SAMPLE_GET_INSTANCE()->setUiScale(2.0); //x2 the size for 4k screens
	TIME_SAMPLE_GET_INSTANCE()->drawUiWithFontStash("VeraMono-Bold.ttf", 20.f);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

	switch (key) {
		case '1':
		newBlobSound.play();
		break;
		case '2':
		killBlobSound.play();
		break;
		case '3':
		movedBlobSound.play();
		break;

		//case 'm':
		//	bMirror = !bMirror;
		//	break;
		//case ' ':
		//	resetBackground = true;
		//	break;
		//case 'f':
		//	bFreeze = !bFreeze;
		//	if (bFreeze){
  //              ofxCv::copy(kinect.getDepthPixels(), grayCopy);
		//	}
		//	break;
	}
}

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
