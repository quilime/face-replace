#include "testApp.h"

using namespace ofxCv;

string facesDir = "faces";
bool showcam = false;

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
    staticFbo.allocate(settings);
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
    
    ofClear(0);
        
    if(camTracker.getFound()) {

        staticFbo.begin();
        
        ofClear(0);
        
        // video mask via face outline
        ofMesh imageMesh = camTracker.getImageMesh();        
        ofPolyline faceOutline = camTracker.getImageFeature();        
        ofSetColor(255);        
        
        // create mask from face outline
        if (showcam) {
            ofFill(); 
            ofBeginShape();
            vector<ofVec3f> vertices = faceOutline.getVertices();
            for (int i = 0; i < vertices.size(); i++) {
                ofVec3f p = vertices.at(i);
                ofVertex(p);
            }
            ofEndShape();
            ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
        }
        cam.draw(0, 0);
                
        // draw face
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);        
        imageMesh.clearTexCoords();
        imageMesh.addTexCoords(srcPoints);
        src.bind();
        imageMesh.draw();
        
        staticFbo.end();
    }
    
    staticFbo.draw(0, 0);
    
}

void testApp::loadFace(string face){
    src.loadImage(face);    
	if(src.getWidth() > 0) {
		srcTracker.update( toCv( src ));
		srcPoints = srcTracker.getImagePoints();
	}
}

void testApp::dragEvent(ofDragInfo dragInfo) {
	loadFace(dragInfo.files[0]);
}

void testApp::keyPressed(int key){
	
    // static face switching
    switch(key) {
	
        case OF_KEY_RIGHT:
            currentFace++;
            break;
        
        case OF_KEY_LEFT:
            currentFace--;
            break;
            
        case 'c' :
            showcam = !showcam;
            break;
            
        case ' ':
            if(camTracker.getFound()) {  
                saveFace(ofToString(ofGetUnixTime()));
                currentFace = faces.size() - 1;
            }
            break;    
	}

    if(faces.size() !=0 ) {
        if (currentFace < 0) {
            currentFace = faces.size() - 1;
        } 
        if (currentFace > faces.size() - 1) {
            currentFace = 0;
        } 
        loadFace(faces.getPath(currentFace));
    }

}

void testApp::saveFace( string face ) {
    camImg.setFromPixels(camPixels);
    camImg.saveImage("faces/face_" + face + ".png");
    faces.listDir(facesDir);
}
