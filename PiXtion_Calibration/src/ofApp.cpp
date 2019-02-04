#include "ofApp.h"
#include "ofConstants.h"

using namespace cv;
using namespace ofxCv;

//--------------------------------------------------------------
void ofApp::setup() {
	ofBackground(0, 0, 0, 128);
    
    XML.loadFile("settings.xml");

	settings.width = XML.getValue("settings:width",320);
	settings.height = XML.getValue("settings:height",240);
	settings.fps = XML.getValue("settings:fps",30);;

	settings.doDepth = ofToBool(XML.getValue("settings:doDepth", "true"));
	settings.doRawDepth = ofToBool(XML.getValue("settings:doRawDepth", "false")) ;
	settings.doColor = ofToBool(XML.getValue("settings:doColor", "false")) ;
	settings.doIr = ofToBool(XML.getValue("settings:doIr", "false")) ;
	settings.doRegisterDepthToColor = ofToBool(XML.getValue("settings:registered", "true"));

	settings.depthPixelFormat = PIXEL_FORMAT_DEPTH_1_MM;
	settings.colorPixelFormat = PIXEL_FORMAT_RGB888;
	settings.irPixelFormat = PIXEL_FORMAT_GRAY16;
	settings.useOniFile = false;
	settings.oniFilePath = "UNDEFINED";

	mirror = ofToBool(XML.getValue("settings:mirror", "false"));
	drawColor = ofToBool(XML.getValue("settings:drawColor", "false"));
	drawDepth = ofToBool(XML.getValue("settings:drawDepth", "true"));
	drawIr = ofToBool(XML.getValue("settings:drawIr", "false"));

	grayImage.allocate(settings.width, settings.height);
    gray.allocate(settings.width, settings.height, OF_IMAGE_GRAYSCALE);        

	isReady = oniGrabber.setup(settings);

	videoQuality = 3;
	host = "Allosaurus.local";
	port = 7110;
	compname = "RPi";
    sender.setup(host, port);
    
    contourSlices = 10;
    contourThreshold = 2.0;
    contourMinAreaRadius = 1.0;
    contourMaxAreaRadius = 250.0;
    contourFinder.setMinAreaRadius(contourMinAreaRadius);
    contourFinder.setMaxAreaRadius(contourMaxAreaRadius);
    trackingColorMode = TRACK_COLOR_RGB;

    file.open(ofToDataPath("compname.txt"), ofFile::ReadWrite, false);
    ofBuffer buff;
    if (file) {
        buff = file.readToBuffer();
        compname = buff.getText();
    } else {
        compname += "_" + ofGetTimestampString("%y-%m-%d-%H-%M-%S-%i");
        ofStringReplace(compname, "-", "");
        ofStringReplace(compname, "\n", "");
        ofStringReplace(compname, "\r", "");
        buff.set(compname.c_str(), compname.size());
        ofBufferToFile("compname.txt", buff);
    }
    cout << compname;

}

//--------------------------------------------------------------
void ofApp::update() {
	if (isReady) {
		oniGrabber.update();

		grayImage.setFromPixels(oniGrabber.depthSource.noAlphaPixels->getPixels(), settings.width, settings.height);
		grayImage.mirror(false, mirror);
        grayImage.flagImageChanged();
		toOf(grayImage.getCvImage(), gray.getPixelsRef());
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
    ofSetColor(255,255,255);
    ofBackground(0,0,0);

	if (isReady) {
	    //if (debug) {
        //ofSetLineWidth(2);
        //ofNoFill();
        //}

        int contourCounter = 0;
        unsigned char * pixels = gray.getPixels();
        int gw = gray.getWidth();

        contourFinder.setThreshold(contourThreshold);
        contourFinder.findContours(gray);
        contourFinder.draw();            

        int n = contourFinder.size();
        for (int i = 0; i < n; i++) {
            ofPolyline line = contourFinder.getPolyline(i);
            vector<ofPoint> cvPoints = line.getVertices();

            int x = int(cvPoints[0].x);
            int y = int(cvPoints[0].y);
            ofColor col = pixels[x + y * gw];
            float colorData[3]; 
            colorData[0] = col.r;
            colorData[1] = col.g;
            colorData[2] = col.b;
            char const * pColor = reinterpret_cast<char const *>(colorData);
            std::string colorString(pColor, pColor + sizeof colorData);
            contourColorBuffer.set(colorString); 

            float pointsData[cvPoints.size() * 2]; 
            for (int j=0; j<cvPoints.size(); j++) {
                int index = j * 2;
                pointsData[index] = cvPoints[j].x;
                pointsData[index+1] = cvPoints[j].y;
            }
            char const * pPoints = reinterpret_cast<char const *>(pointsData);
            std::string pointsString(pPoints, pPoints + sizeof pointsData);
            contourPointsBuffer.set(pointsString); 

            sendOscContours(contourCounter);
            contourCounter++;
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
    ofxOscMessage m;
    m.setAddress("/contour");
    m.addStringArg(compname);
    
    m.addIntArg(index);
    m.addBlobArg(contourColorBuffer);
    m.addBlobArg(contourPointsBuffer);

    sender.sendMessage(m);
}