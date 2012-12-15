#include "testApp.h"

using namespace ofxCv;

string facesDir = "faces";

void testApp::setup() {
#ifdef TARGET_OSX
//	ofSetDataPathRoot("../data/");
#endif
	ofSetVerticalSync(true);
	cloneReady = false;
	cam.initGrabber(640, 480);
	clone.setup(cam.getWidth(), cam.getHeight());
	ofFbo::Settings settings;
	settings.width = cam.getWidth();
	settings.height = cam.getHeight();
	maskFbo.allocate(settings);
	srcFbo.allocate(settings);
	camTracker.setup();
	srcTracker.setup();
	srcTracker.setIterations(25);
	srcTracker.setAttempts(4);

	faces.allowExt("jpg");
	faces.allowExt("png");
	faces.listDir(facesDir);
	currentFace = 0;
	if(faces.size()!=0){
		loadFace(faces.getPath(currentFace));
	}
}

void testApp::update() {
	
    cam.update();
    cam.getTextureReference().readToPixels(camPixels);
    
	if(cam.isFrameNew()) {
        
		camTracker.update(toCv(cam));
		
//		cloneReady = camTracker.getFound();
//		if(cloneReady) {
//			
//            ofMesh camMesh = camTracker.getImageMesh();
//			camMesh.clearTexCoords();
//			camMesh.addTexCoords(srcPoints);
//			
//			
//            maskFbo.begin();
//			ofClear(0, 255);
//			camMesh.draw();
//			maskFbo.end();
//            
//			
//			srcFbo.begin();
//			ofClear(0, 255);
//			src.bind();
//			camMesh.draw();
//			src.unbind();
//			srcFbo.end();
//            			
//			clone.setStrength(16);
//			clone.update(srcFbo.getTextureReference(), cam.getTextureReference(), maskFbo.getTextureReference());
//		}
	}
}

void testApp::draw() {
    
    
    ofTranslate(ofGetWidth(), 0);
    ofScale(-ofGetWidth() / cam.getWidth(), ofGetHeight() / cam.getHeight());
    //    ofScale(-1, 0);
    
    
	ofSetColor(255);
    //cam.draw(0, 0);
    ofClear(0, 0, 0);
    
    //ofSetupScreenOrtho(640, 480, OF_ORIENTATION_DEFAULT, true, -1000, 1000);
    
    ofMesh imageMesh = camTracker.getImageMesh();
    
    ofPolyline faceOutline = camTracker.getImageFeature();
    
    
    ofSetColor(255, 255, 255);
    ofFill();
    ofBeginShape();
    vector<ofVec3f> vertices = faceOutline.getVertices();
    for (int i = 0; i < vertices.size(); i++) {
        ofVec3f p = vertices.at(i);
        ofVertex(p);
    }
    ofEndShape();
    
    ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
    
    

    
    
    
	
	
//    if(src.getWidth() > 0 && cloneReady) {
//		clone.draw(0, 0);
//	} else {
//		cam.draw(0, 0);
//	}
    
    cam.draw(0, 0);
    
    ofDisableBlendMode();
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    
    imageMesh.clearTexCoords();
    imageMesh.addTexCoords(srcPoints);
    src.bind();
    
    imageMesh.draw();
    

    /*
	if(!camTracker.getFound()) {
		drawHighlightString("camera face not found", 10, 10);
	}
	if(src.getWidth() == 0) {
		drawHighlightString("drag an image here", 10, 30);
	} else if(!srcTracker.getFound()) {
		drawHighlightString("image face not found", 10, 30);
	}
     */
    
}

void testApp::loadFace(string face){
	src.loadImage(face);
	if(src.getWidth() > 0) {
		srcTracker.update(toCv(src));
		srcPoints = srcTracker.getImagePoints();
	}
}

void testApp::dragEvent(ofDragInfo dragInfo) {
	loadFace(dragInfo.files[0]);
}

void testApp::keyPressed(int key){
	
    // static face switching
    switch(key){
	
    case OF_KEY_UP:
		currentFace++;
		break;
	
    case OF_KEY_DOWN:
		currentFace--;
		break;
            
    case ' ':
        int t = ofGetUnixTime();
        camImg.setFromPixels(camPixels);
        camImg.saveImage("faces/face_" + ofToString(t) + ".png");               
        break;    
	}
    
    
    if (currentFace < 0 || currentFace > faces.size()-1) {
        currentFace = 0;
    }
	if(faces.size() !=0 ){
        faces.listDir(facesDir);
		loadFace(faces.getPath(currentFace));
	}
}
