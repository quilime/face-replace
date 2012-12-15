#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxFaceTracker.h"

class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
    
    ofImage camImg;
    ofPixels camImgPixels;
    
	ofVideoGrabber cam;
	ofxFaceTracker tracker;
		
	ofEasyCam easyCam;
};
