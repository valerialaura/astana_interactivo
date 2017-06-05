#include "AstanaBlobsManager2.h"
#include "AstanaDraw.h"
AstanaBlobsManager2::AstanaBlobsManager2() {	
}
//--------------------------------------------------------------
AstanaBlobsManager2::~AstanaBlobsManager2() {
}
//--------------------------------------------------------------
void AstanaBlobsManager2::setup() {
	AstanaKinectBlobFinder::setup();
	string bfXml = "blob_manager_settings.xml";
	gui.setup("Blobs Manager", bfXml);
//gui.add(bDrawMerged.set("Draw Merged blobs", false));
	//gui.add(bDrawActive.set("bDrawActive", true));
	//gui.add(bDrawKilled .set("bDrawKilled",true));
	//gui.add(bDrawGhosts .set("bDrawGhosts",true));
	//gui.add(bDrawMoved  .set("bDrawMoved" ,true));
	//gui.add(bDrawScaled .set("bDrawScaled",true));
	//gui.add(bDrawNew    .set("bDrawNew"	  ,true));
//	gui.add(bUseOsc    .set("Usar OSC"	  ,true));
	gui.add(parameters);
	gui.setSize(300, 400);
	gui.setPosition(ofGetWidth() - 340, 40);
	gui.setWidthElements(300);
	gui.loadFromFile(bfXml);

	listeners.clear();
	listeners.push_back(ofEvents().update.newListener(this, &AstanaBlobsManager2::update));
}
//--------------------------------------------------------------
void AstanaBlobsManager2::update(ofEventArgs&) {
	AstanaKinectBlobFinder::update();
}
//--------------------------------------------------------------
void AstanaBlobsManager2::drawDebug(){
	AstanaDraw::drawDebug(getBlobsCollection());
}
//--------------------------------------------------------------
void AstanaBlobsManager2::draw() {
		AstanaDraw::drawDebug(getBlobsCollection());
		AstanaKinectBlobFinder::draw();
}
//--------------------------------------------------------------
void AstanaBlobsManager2::drawGui() {
	gui.draw();

	stringstream ss;
	ss << "fps : " << ofGetFrameRate() << endl;
	ofBitmapFont f;
	auto r = f.getBoundingBox(ss.str(), 0, 0);
	ofDrawBitmapStringHighlight(ss.str(), ofGetWidth() - 20 - r.width, 20);

}
