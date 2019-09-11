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

		ofxCvColorImage colorCv;
		ofImage color;

        ofxXmlSettings XML;
        string xmlStructure;
        string message;

        string compname;
		string host; // hostname;
		int port; // default 7110;
		int videoQuality; // 5 best to 1 worst, default 3 medium
		bool videoColor;
		ofBuffer videoBuffer;
		
		ofBuffer colorBuffer;
		ofBuffer pointsBuffer;
		float minZ;

		ofxOscSender sender;
		void sendOscScanline(int index);

};