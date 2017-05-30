#include "ofApp.h"

#define DEPTH_WIDTH 512
#define DEPTH_HEIGHT 424
#define DEPTH_SIZE DEPTH_WIDTH * DEPTH_HEIGHT


//--------------------------------------------------------------
void ofApp::setup() {
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofEnableSetupScreen();
	ofSetupScreen();
	//kinect.open();
	//kinect.initDepthSource();
	//kinect.initColorSource();
	//

	blobFinder.setup();//DEPTH_WIDTH, DEPTH_HEIGHT);
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

	vergui = -1;
	
}

//--------------------------------------------------------------
void ofApp::update() {	
	blobFinder.update();
	for (auto& b : blobFinder.getAllBlobs()) {
		for (int i = 0; i < avatarList.size(); i++)
		{

			if (b->label == avatarList[i].id) avatarList[i].update(b->center, ofPoint(b->boundingRect.getWidth(), b->boundingRect.getHeight()));
		}
	}
	
}
//--------------------------------------------------------------
void ofApp::onNewBlobs() {
	newBlobSound.play();
	cout << "NEW BLOB  " << blobFinder.getNewBlobs().size() << endl;
	cout << "ALL BLOBS " << blobFinder.getAllBlobs().size() << endl;
	cout << "AVATARRRRR " << avatarList.size() << endl;
	for (auto& b : blobFinder.getAllBlobs()) {
		if (b)	cout << b->polyline.size() << endl;
	}
	for (auto& b : blobFinder.getNewBlobs()) {
		avatarLogo ava;
		ava.setup(b->label);

		avatarList.push_back(ava);
	}


}
//--------------------------------------------------------------
void ofApp::onKillBlobs() {
	killBlobSound.play();
	cout << "-------------------BORRAR " << endl;
	for (int i = 0; i < avatarList.size(); i++)
	{
		bool borrar = true;
		for (auto& b : blobFinder.getAllBlobs()) {
			if (avatarList[i].id == b->label) {
				borrar = false;
				cout << "-" << b->label << endl;
			}
			

		}
		if (borrar == true) {
			avatarList.erase(avatarList.begin() + i);
		}
	}

	cout << "-------------------BORRAR "  << endl;
	
}
//--------------------------------------------------------------
void ofApp::onMovedBlobs() {
	//movedBlobSound.play();
	//cout << "MOVED BLOB " << blobFinder.movedBlobs.size() << endl;
}
//--------------------------------------------------------------
void ofApp::draw() {
	ofBackground(0, 0, 0);
	if (vergui == 1)
	{
		if (bDrawBlobFinder) {
			blobFinder.draw();
		}
		if (blobFinder.getNewBlobs().size()) {
			cout << "DRAW new Blobs: " << blobFinder.getNewBlobs().size() << endl;
		}
		if (blobFinder.getMovedBlobs().size()) {
			//cout << "DRAW moved Blobs: " << blobFinder.movedBlobs.size() << endl;
		}
		ofPushStyle();
		ofSetColor(ofxCv::magentaPrint);
		ofSetLineWidth(2);
		for (auto& b : blobFinder.getAllBlobs()) {
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
		}
		ofPopStyle();*/

		stringstream ss;
		ss << "fps : " << ofGetFrameRate() << endl;	
		ss << "allBlobs: " << blobFinder.getAllBlobs().size() << endl;
		ss << "allBlobs tiene NULL ";
		for (auto& b : blobFinder.getAllBlobs()) {
			if (!b) {
				ss << "TRUE";
				break;
			}
		}
	
		ofDrawBitmapStringHighlight(ss.str(), 20, 20);
		gui.draw();
	}
	ofPushMatrix();
		//ofTranslate(ofGetMouseX(), 0, ofGetMouseY() - 500);
		//ofRotateX(ofGetMouseX() / 4);
		//ofRotateY(ofGetMouseX() / 4);
		for (int i = 0; i < avatarList.size(); i++)
		{
			avatarList[i].draw();
		}
	ofPopMatrix();
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
		case 'p':
			vergui *= -1;
			cout << "keyyyyyyy "  << endl;
			break;
		case 's':
			gui.saveToFile("blobFinder_settings.xml");
			break;
		case '0':
				for (int i = 0; i < avatarList.size(); i++)
				{
					
						avatarList.erase(avatarList.begin() + i);
					
				}
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
