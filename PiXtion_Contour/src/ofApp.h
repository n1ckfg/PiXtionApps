#pragma once

#include "ofMain.h"
#include "ofConstants.h"
#include "ofxOpenNI2Grabber.h"
#include "ofxOpenNI2Recorder.h"
#include "ofxOpenCv.h"
#include "ofxCv.h"
#include "ofxOsc.h"
#include "ofxXmlSettings.h"

#include "../../../core/common/src/Pinopticon.hpp"

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
		ofxCvColorImage colorImage; // rgb cam image
		ofImage gray;
		ofImage color;

        ofxXmlSettings XML;
        string xmlStructure;
        string message;

		string sessionId, hostName; 
		string oscHost; // hostname;
		int timestamp;
		int oscPort; // default 7110;
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
		float minZ;
		float simplify;
		int smooth;

		ofxOscSender sender;
		void sendOscContours(int index);
		string cleanString(string input);

};