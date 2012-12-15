#include "testApp.h"

using namespace ofxCv;

void testApp::setup() {
	ofSetVerticalSync(true);
	cam.initGrabber(640, 480);
	tracker.setup();
    
    srcTracker.setup();
	srcTracker.setIterations(25);
	srcTracker.setAttempts(4);    
}

void testApp::update() {
	cam.update();
	if(cam.isFrameNew()) {
		tracker.update(toCv(cam));
	}
}

void testApp::draw() {
    
	ofSetColor(255);
    
    //cam.draw(0, 0);
    
    
    
    // save camera pixels
	cam.getTextureReference().readToPixels(camImgPixels);
    
	if(tracker.getFound()) {
        
        ofClear(0, 0, 0);
		
		ofSetupScreenOrtho(640, 480, OF_ORIENTATION_DEFAULT, true, -1000, 1000);
        
        ofMesh imageMesh = tracker.getImageMesh();
        
        ofPolyline faceOutline = tracker.getImageFeature(ofxFaceTracker::FACE_OUTLINE);
        
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
        cam.draw(0, 0);
        ofDisableBlendMode();
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
        

        //faceOutline.draw();
        //cam.draw(0, 0);
        
        //cam.getTextureReference().bind();
        
        imageMesh.clearTexCoords();
        imageMesh.addTexCoords(srcPoints);
        src.bind();
        
        imageMesh.draw();
        
		//cam.getTextureReference().unbind();
	}
    
    ofDrawBitmapString(ofToString((int) ofGetFrameRate()), 10, 20);
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

void testApp::keyPressed(int key) {
	if(key == 'r') {
		tracker.reset();
	}
    if (key == ' ' && tracker.getFound()) {
        
        int t = ofGetUnixTime();
        
        camImg.setFromPixels(camImgPixels);
        camImg.saveImage("capture/cam_" + ofToString(t) + ".png");
        
        camImg.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
        camImg.saveImage("capture/face_" + ofToString(t) + ".png");
        
    }
}



