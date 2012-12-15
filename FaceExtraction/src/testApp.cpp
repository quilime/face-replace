#include "testApp.h"

using namespace ofxCv;



void testApp::setup() {
	ofSetVerticalSync(true);
	cam.initGrabber(1024, 768);
	tracker.setup();
    
        
}

void testApp::update() {
	cam.update();
	if(cam.isFrameNew()) {
		tracker.update(toCv(cam));
	}
}

void testApp::draw() {
    
	ofSetColor(255);
    
    cam.draw(0, 0);
    
	ofDrawBitmapString(ofToString((int) ofGetFrameRate()), 10, 20);
    
    //    ofClear(0, 0, 0);

    // save camera pixels
    
	cam.getTextureReference().readToPixels(camImgPixels);
    
	if(tracker.getFound()) {
        
        //		tracker.draw();
        ofMesh objectMesh   = tracker.getObjectMesh();
        ofMesh meanMesh     = tracker.getMeanObjectMesh();
        ofMesh getImageMesh = tracker.getImageMesh();
		
		ofSetupScreenOrtho(1024, 768, OF_ORIENTATION_DEFAULT, true, -1000, 1000);
        
        //        ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
        //        ofScale(10, 10, 10);
        //        ofRotate(4.5, 0, 0, 1);        
        

        
		cam.getTextureReference().bind();        
        
        //meanMesh.draw();
        getImageMesh.draw();
        
        //objectMesh.draw();
		cam.getTextureReference().unbind();
	}
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



