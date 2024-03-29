#include "ofApp.h"

#include "../../../core/common/src/Pinopticon.hpp"

using namespace cv;
using namespace ofxCv;
using namespace Pinopticon;

//--------------------------------------------------------------
void ofApp::setup() {
	ofBackground(0, 0, 0, 128);
    	
    XML.loadFile("settings.xml");

	settings.width = XML.getValue("settings:width", 320);
	settings.height = XML.getValue("settings:height", 240);
	settings.fps = XML.getValue("settings:fps", 30);;

	settings.doDepth = ofToBool(XML.getValue("settings:doDepth", "true"));
	settings.doRawDepth = ofToBool(XML.getValue("settings:doRawDepth", "true")) ;
	settings.doColor = ofToBool(XML.getValue("settings:doColor", "true")) ;
	settings.doIr = ofToBool(XML.getValue("settings:doIr", "false")) ;
	settings.doRegisterDepthToColor = ofToBool(XML.getValue("settings:registered", "true"));

	settings.depthPixelFormat = PIXEL_FORMAT_DEPTH_1_MM;
	settings.colorPixelFormat = PIXEL_FORMAT_RGB888;
	settings.irPixelFormat = PIXEL_FORMAT_GRAY16;
	settings.useOniFile = false;
	settings.oniFilePath = "UNDEFINED";

	mirror = ofToBool(XML.getValue("settings:mirror", "false"));
	drawColor = ofToBool(XML.getValue("settings:drawColor", "true"));
	drawDepth = ofToBool(XML.getValue("settings:drawDepth", "true"));
	drawIr = ofToBool(XML.getValue("settings:drawIr", "false"));
	doOscContours = ofToBool(XML.getValue("settings:do_osc_contours", "true"));
	drawContours = ofToBool(XML.getValue("settings:draw_contours", "false"));
	
	grayImage.allocate(settings.width, settings.height);
    gray.allocate(settings.width, settings.height, OF_IMAGE_GRAYSCALE);        
    colorImage.allocate(settings.width, settings.height);
    color.allocate(settings.width, settings.height, OF_IMAGE_COLOR);   
    fbo.allocate(settings.width, settings.height, GL_RGBA);     

	isReady = oniGrabber.setup(settings);

    videoQuality = XML.getValue("settings:videoQuality", 3);
	host = XML.getValue("settings:host", "127.0.0.1");
	port = XML.getValue("settings:port", 7110);
	compname = createCompName("RPi");
    sender.setup(host, port);
    
    contourSlices = 5;
    contourThreshold = 2.0;
    contourMinAreaRadius = 1.0;
    contourMaxAreaRadius = 250.0;
    contourFinder.setMinAreaRadius(contourMinAreaRadius);
    contourFinder.setMaxAreaRadius(contourMaxAreaRadius);
    trackingColorMode = TRACK_COLOR_RGB;
    minZ = 0.21;
    simplify = XML.getValue("settings:simplify", 0.5);
    smooth = XML.getValue("settings:smooth", 2);
}

//--------------------------------------------------------------
void ofApp::update() {
	if (isReady) {
		oniGrabber.update();
		
        colorImage.setFromPixels(oniGrabber.depthSource.currentPixels->getPixels(), settings.width, settings.height);
        colorImage.mirror(false, mirror);
        colorImage.flagImageChanged();
        toOf(colorImage.getCvImage(), color.getPixelsRef());
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
    fbo.begin();
    ofSetColor(255,255,255);
    ofBackground(0,0,0);

	if (isReady) {
        int contourCounter = 0;
        unsigned char * pixels = color.getPixels();
        int gw = color.getWidth();

        for (int h=0; h<255; h += int(255/contourSlices)) {
            contourFinder.setThreshold(h);
            contourFinder.findContours(colorImage);
            //contourFinder.draw();            

            int n = contourFinder.size();
            
		for (int i = 0; i < n; i++) {

		ofPolyline line = contourFinder.getPolyline(i);
                line.simplify(simplify);
                line = line.getSmoothed(smooth, 0.5);
                
		vector<ofPoint> cvPoints = line.getVertices();
                vector<ofVec3f> cvCleanPoints;

                int middle = int(cvPoints.size()/2);
                int x = int(cvPoints[middle].x);
                int y = int(cvPoints[middle].y);
                int loc = (x + y * gw) * 3;
                ofColor col = ofColor(pixels[loc], pixels[loc + 1], pixels[loc + 2]);
		
		if (drawContours) {
			ofSetColor(col);
			line.draw();
		}

                if (doOscContours) {
			float colorData[3]; 
                	colorData[0] = col.r;
                	colorData[1] = col.g;
                	colorData[2] = col.b;
                	char const * pColor = reinterpret_cast<char const *>(colorData);
                	std::string colorString(pColor, pColor + sizeof colorData);
                	contourColorBuffer.set(colorString); 
		}
		
		if (doOscContours) {
                for (int j=0; j<cvPoints.size(); j++) {
                    ofVec3f v;
                    v = oniGrabber.convertDepthToWorld((int) cvPoints[j].x, (int) cvPoints[j].y);
                    if (v.z > minZ) {
			cvCleanPoints.push_back(v);
			}
		}
                
			float pointsData[cvCleanPoints.size() * 3]; 
                	for (int j=0; j<cvCleanPoints.size(); j++) {
                    		int index = j * 3;
                    		pointsData[index] = cvCleanPoints[j].x;
                    		pointsData[index+1] = cvCleanPoints[j].y;
                    		pointsData[index+2] = cvCleanPoints[j].z;
                		
				
			}
                	char const * pPoints = reinterpret_cast<char const *>(pointsData);
                	std::string pointsString(pPoints, pPoints + sizeof pointsData);
                	contourPointsBuffer.set(pointsString); 

                	sendOscContours(contourCounter);
		}

                contourCounter++;
        	
}
}
	}
    fbo.end();

    fbo.draw(0, 0, ofGetWidth(), ofGetHeight());
}

//--------------------------------------------------------------
void ofApp::exit() {
	ofLogVerbose() << "\n EXITING, be patient - takes some time \n";
	
	if (isReady) {
		oniGrabber.close();
	}
}
//--------------------------------------------------------------
void ofApp::sendOscContours(int index) {
    ofxOscMessage msg;
    msg.setAddress("/contour");
    msg.addStringArg(compname);

    msg.addIntArg(index);
    msg.addBlobArg(contourColorBuffer);
    msg.addBlobArg(contourPointsBuffer);

    sender.sendMessage(msg);
}

