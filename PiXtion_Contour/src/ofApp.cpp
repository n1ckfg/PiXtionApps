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

	grayImage.allocate(settings.width, settings.height);
    gray.allocate(settings.width, settings.height, OF_IMAGE_GRAYSCALE);        
    colorImage.allocate(settings.width, settings.height);
    color.allocate(settings.width, settings.height, OF_IMAGE_COLOR);        

	isReady = oniGrabber.setup(settings);

    videoQuality = XML.getValue("settings:videoQuality", 3);
	oscHost = XML.getValue("settings:host", "127.0.0.1");
	oscPort = XML.getValue("settings:port", 7110);
    sender.setup(oscHost, oscPort);

    sessionId = getSessionId();   
    hostName = getHostName();
    
    timestamp = 0;
    contourSlices = 10;
    contourThreshold = 2.0;
    contourMinAreaRadius = 1.0;
    contourMaxAreaRadius = 250.0;
    contourFinder.setMinAreaRadius(contourMinAreaRadius);
    contourFinder.setMaxAreaRadius(contourMaxAreaRadius);
    trackingColorMode = TRACK_COLOR_RGB;
    minZ = 0.21;
    contourSlices = XML.getValue("settings:contour_slices", 10);
    simplify = XML.getValue("settings:simplify", 0.5);
    smooth = XML.getValue("settings:smooth", 2);
}

//--------------------------------------------------------------
void ofApp::update() {
	if (isReady) {
		oniGrabber.update();
		
        colorImage.setFromPixels(oniGrabber.rgbSource.currentPixels->getPixels(), settings.width, settings.height);
        colorImage.mirror(false, mirror);
        colorImage.flagImageChanged();
        toOf(colorImage.getCvImage(), color.getPixelsRef());
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
    ofSetColor(255,255,255);
    ofBackground(0,0,0);

	if (isReady) {
        int contourCounter = 0;
        unsigned char * pixels = color.getPixels();
        int gw = color.getWidth();

        for (int h=0; h<255; h += int(255/contourSlices)) {
            contourFinder.setThreshold(h);
            contourFinder.findContours(colorImage);
            contourFinder.draw();            

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

                float colorData[3]; 
                colorData[0] = col.r;
                colorData[1] = col.g;
                colorData[2] = col.b;
                char const * pColor = reinterpret_cast<char const *>(colorData);
                std::string colorString(pColor, pColor + sizeof colorData);
                contourColorBuffer.set(colorString); 

                for (int j=0; j<cvPoints.size(); j++) {
                    ofVec3f v;
                    v = oniGrabber.convertDepthToWorld((int) cvPoints[j].x, (int) cvPoints[j].y);
                    if (v.z > minZ) cvCleanPoints.push_back(v);
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
                contourCounter++;
            }        
        }
	}
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
    
    msg.addStringArg(hostName);
    msg.addStringArg(sessionId);
    msg.addIntArg(index);
    msg.addBlobArg(contourColorBuffer);
    msg.addBlobArg(contourPointsBuffer);
    msg.addIntArg(timestamp);

    sender.sendMessage(msg);
}

