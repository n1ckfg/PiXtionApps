#pragma once

#include "ofMain.h"
#include "ofxOpenNI2Grabber.h"
#include "ofxOpenNI2Recorder.h"
#include "ofxOpenCv.h"
#include "ofxCv.h"
#include "ofxOsc.h"
#include "ofxXmlSettings.h"

class ofApp : public ofBaseApp {

	public:
		void setup();
		void update();
		void draw();
		void exit();

		bool isReady;

		bool mirror;
		bool drawColor;
		bool drawDepth;
		bool drawIr;

		ofxOpenNI2Grabber oniGrabber;
		ofxOpenNI2GrabberSettings settings;

		ofxCvGrayscaleImage grayImage; // grayscale depth image
		ofImage gray;

        ofxXmlSettings XML;
        string xmlStructure;
        string message;

		ofFile file;
        string compname;
		string host; // hostname;
		int port; // default 7110;
		int videoQuality; // 5 best to 1 worst, default 3 medium
		bool videoColor;
		ofBuffer videoBuffer;
		
		ofBuffer contourColorBuffer;
		ofBuffer contourPointsBuffer;
	    ofxCv::ContourFinder contourFinder;
		float contourThreshold;  // default 127
		float contourMinAreaRadius; // default 10
		float contourMaxAreaRadius; // default 150
		int contourSlices; // default 20
		ofxCv::TrackingColorMode trackingColorMode; // RGB, HSV, H, HS; default RGB

		ofxOscSender sender;
		void sendOscContours(int index);

};