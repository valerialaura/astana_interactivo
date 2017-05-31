#include "AstanaOSCBlobSender.h"
#include "AstanaBlobsOSCConvert.h"
//--------------------------------------------------------------
void AstanaOSCBlobSender::setup(shared_ptr<AstanaKinectBlobFinder> blobFinder) {
	this->blobFinder = blobFinder;
	string xmlPath = "oscBlobSenderSettings.xml";
	gui.setup("OSC Blob Sender", xmlPath);
	gui.add(hostIp.set("Host IP", "localhost"));
	gui.add(port.set("port", 12345, 10000, 20000));
	gui.add(blobFinder->parameters);
	gui.setSize(300, 400);
	gui.setPosition(ofGetWidth() - 340, 40);
	gui.setWidthElements(300);
	gui.loadFromFile(xmlPath);

	string s = "";
	hostIpChanged(s);
}

//--------------------------------------------------------------
void AstanaOSCBlobSender::hostIpChanged(string&){
	bIsSetup = sender.setup(hostIp, port);
	if (bIsSetup) {
		listeners.clear();
		listeners.push_back(blobFinder->anyBlobEvent.newListener(this, &AstanaOSCBlobSender::onAnyBlobsEvent));
		listeners.push_back(hostIp.newListener(this, &AstanaOSCBlobSender::hostIpChanged));
		listeners.push_back(ofEvents().update.newListener(this, &AstanaOSCBlobSender::update));
		sync.setup(blobFinder->parameters, 7777, "localhost", 6666);
	}else {
		cout << "No se pudo setear OSC blob sender" << endl;
	}
}
//--------------------------------------------------------------
void AstanaOSCBlobSender::update(ofEventArgs&) {
	if(bIsSetup) sync.update();
}
//--------------------------------------------------------------
void AstanaOSCBlobSender::drawGui() {
	gui.draw();
}
//--------------------------------------------------------------
void AstanaOSCBlobSender::onAnyBlobsEvent() {
	if (blobFinder != nullptr && bIsSetup) {
		AstanaBlobsOSCConvert::toOSC(sender, blobFinder->getBlobsCollection());
	}
}
