#include "ofApp.h"

//--------------------------------------------------------------
#ifdef USE_THREADED
ofApp::~ofApp(){
        stop();
        waitForThread(false);
}
#endif
//--------------------------------------------------------------
void ofApp::setup(){
	ofSetLogLevel(OF_LOG_VERBOSE);
	
	ofFileDialogResult result = ofSystemLoadDialog();
	if (result.bSuccess) {
		if (player.load(result.getPath())) {
			ofSoundStreamSettings settings;
			settings.bufferSize = 256;
			settings.numBuffers = 1;
			settings.numInputChannels = 0;
			settings.numOutputChannels = 2;
			settings.sampleRate = player.getSoundFile().getSampleRate();
			cout << "SampleRate: " << settings.sampleRate << endl;
			wave.setup(0, 0, ofGetWidth(), ofGetHeight());
			player.connectTo(wave);

			//player.getSoundFile().readTo(wave.buffer, player.getSoundFile().getNumSamples());
			soundStream.setup(settings);
			//stream.setOutput(player);
			//stream.setOutput(output);
			soundStream.setOutput(wave);
		


			//player.connectTo(wave).connectTo(output);
			//player.connectTo(output);
			player.play();

			ofBackground(0);
		}
	}
 //  	ofSoundStreamSettings settings;
	//		settings.bufferSize = 256;
	//		settings.numBuffers = 1;
	//		settings.numInputChannels = 0;
	//		settings.numOutputChannels = 2;
	//		settings.sampleRate = 48000;

	//		soundStream.setup(settings);
 //   // connect each object to the next in a chain, finishing with the output.
	//noise.connectTo(filter).connectTo(delay);//.connectTo(output);
	//
	//
	//soundStream.setOutput(delay);

	//ofBackground(0);


	gui.setup("ASTANA AUDIO FX");
	gui.add(delay.parameters);
	gui.add(filter.parameters);






#ifdef USE_THREADED
	 startThread();
#endif
}


//--------------------------------------------------------------
void ofApp::update(){

}


//--------------------------------------------------------------
void ofApp::draw(){
	if (player.isLoaded()) {
		wave.draw();
	}
	gui.draw();
}

//--------------------------------------------------------------
void ofApp::exit(){
	if (player.isLoaded()) {
		soundStream.close();
	}
}
#ifdef USE_THREADED
//--------------------------------------------------------------
void ofApp::stop() {
	  std::unique_lock<std::mutex> lck(mutex);
        stopThread();
        condition.notify_all();
}
//--------------------------------------------------------------
void ofApp::threadedFunction(){
	while(isThreadRunning()){
            std::unique_lock<std::mutex> lock(mutex);

			float f = ofSignedNoise((float)ofGetFrameNum());
	
			f = f * f;
			filter.fc = ofMap(f, 0, 1, 50, 11025, true);
	
			delay.feedback = ofMap(ofSignedNoise(ofGetElapsedTimef()), -1, 1, 0.99, 0.1, true);
            condition.wait(lock);
        }
}
#endif
//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	if(key == ' '){
		wave.connectTo(filter).connectTo(delay);
		soundStream.setOutput(delay);

	}
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}