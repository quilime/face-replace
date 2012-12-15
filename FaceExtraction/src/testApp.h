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
    
    void dragEvent(ofDragInfo dragInfo);
	void loadFace(string face);
    
    ofxFaceTracker srcTracker;
	ofImage src;
	vector<ofVec2f> srcPoints;
};
