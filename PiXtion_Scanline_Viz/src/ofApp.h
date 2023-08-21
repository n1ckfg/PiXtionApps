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

		ofxCvColorImage colorCv;
		ofImage color;

        ofxXmlSettings XML;
        string xmlStructure;
        string message;

        string compname;
		string host; // hostname;
		int port; // default 7110;
		int depthVideoQuality, rgbVideoQuality; // 5 best to 1 worst, default 3 medium
		ofBuffer depthVideoBuffer, rgbVideoBuffer;
		
		ofBuffer colorBuffer, pointsBuffer;
		float minZ;
		float simplify;
		int smooth;
		
		ofPolyline line;
		float xScale, yScale;
		
		ofxOscSender sender;
		void sendOscScanline(int index);

};