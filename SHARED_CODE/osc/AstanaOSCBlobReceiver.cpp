#include "AstanaOSCBlobReceiver.h"
#include "AstanaDraw.h"
//--------------------------------------------------------------
void AstanaOSCBlobReceiver::setup() {
	gui.setup("Astana OSC Receiver", "osc_receiver_settings.xml");
	gui.add(port.set("Port", PORT, 10000, 20000));
	gui.add(bDrawDebug.set("Draw Debug", true));
	gui.add(bDrawGhost.set("Draw Ghosts", true));
	gui.add(bDrawRects.set("Draw Rects", true));
	gui.add(bDrawPolylines.set("Draw Polylines", true));
	gui.add(bDrawLabels.set("Draw Labels", true));
	gui.add(bEnableDraw.set("Enable Draw", true));
	bIsSetup = receiver.setup(PORT);

	if (bIsSetup) {
		updateListener = ofEvents().update.newListener(this, &AstanaOSCBlobReceiver::update);
	}
}
//--------------------------------------------------------------
void AstanaOSCBlobReceiver::update(ofEventArgs& a) {
	previous = current;
	current.clear();
	AstanaBlobsOSCConvert::toBlobs(receiver, current);
}
//--------------------------------------------------------------
void AstanaOSCBlobReceiver::draw() {
	if (bEnableDraw && current.size() > 0) {
		if (bDrawDebug) AstanaDraw::drawDebug(current);
		if (bDrawGhost) AstanaDraw::drawGhosts(current);
		if (bDrawRects) AstanaDraw::drawRects(current);
		if (bDrawPolylines) AstanaDraw::drawPolylines(current);
		if (bDrawLabels) AstanaDraw::drawLabels(current);
	}
}
//--------------------------------------------------------------
void AstanaOSCBlobReceiver::drawGui() {
	gui.draw();
}