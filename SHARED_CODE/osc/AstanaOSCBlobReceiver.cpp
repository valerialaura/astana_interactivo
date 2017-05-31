#include "AstanaOSCBlobReceiver.h"
#include "AstanaDraw.h"
//--------------------------------------------------------------
void AstanaOSCBlobReceiver::setup() {
	string xml = "osc_receiver_settings.xml";
	gui.setup("Astana OSC Receiver", xml);
	gui.add(hostIp.set("Host IP", "localhost"));
	gui.add(port.set("Port", 12345, 10000, 20000));
	setupParams();//"OSC Blob Finder");
	gui.add(parameters);

	gui.loadFromFile(xml);

	bIsSetup = receiver.setup(PORT);
	if (bIsSetup) {
		updateListener = ofEvents().update.newListener(this, &AstanaOSCBlobReceiver::update);
	}
	string s;
	hostIpChanged(s);
}
//--------------------------------------------------------------
void AstanaOSCBlobReceiver::hostIpChanged(string&) {
	if (bIsSetup) {
		sync.setup(parameters, 6666, hostIp, 7777);
		bIsSyncSetup = true;
	}else {
		cout << "No se pudo setear OSC blob sender" << endl;
	}
}
//--------------------------------------------------------------
AstanaBlobCollection& AstanaOSCBlobReceiver::getBlobsCollection() {
	return current;
}
//--------------------------------------------------------------
void AstanaOSCBlobReceiver::update(ofEventArgs& a) {
	previous = current;
	if (AstanaBlobsOSCConvert::toBlobs(receiver, current)) {
		ofNotifyEvent(anyBlobEvent);
	}
	if (bIsSyncSetup) {
		sync.update();
	}
}
//--------------------------------------------------------------
void AstanaOSCBlobReceiver::draw() {
	if (bEnableDraw && current.size() > 0) {
		if (bDrawDebug) AstanaDraw::drawDebug(current);
		if (bDrawGhosts) AstanaDraw::drawGhosts(current);
		if (bDrawRects) AstanaDraw::drawRects(current);
		if (bDrawPolylines) AstanaDraw::drawPolylines(current);
		if (bDrawTrackerLabels) AstanaDraw::drawLabels(current);
	}
}
//--------------------------------------------------------------
void AstanaOSCBlobReceiver::drawGui() {
	gui.draw();
}