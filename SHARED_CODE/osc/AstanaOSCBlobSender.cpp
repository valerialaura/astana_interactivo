#include "AstanaOSCBlobSender.h"
#include "AstanaBlobsOSCConvert.h"
//--------------------------------------------------------------
void AstanaOSCBlobSender::setup(shared_ptr<AstanaKinectBlobFinder> blobFinder) {
	this->blobFinder = blobFinder;
	string xmlPath = "oscBlobSenderSettings.xml";
	gui.setup("OSC Blob Sender", xmlPath);
	gui.add(hostIp.set("Host IP", "localhost"));
	gui.add(port.set("port", 12345, 10000, 20000));
	gui.loadFromFile(xmlPath);

	bIsSetup = sender.setup(hostIp, port);
	if (bIsSetup) {
		listeners.clear();
		listeners.push_back(blobFinder->anyBlobEvent.newListener(this, &AstanaOSCBlobSender::onAnyBlobsEvent));
	}
	else {
		cout << "No se pudo setear OSC blob sender" << endl;
	}
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
