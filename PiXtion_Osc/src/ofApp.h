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
		bool doColor;
		bool doDepth;

		ofxOpenNI2Grabber oniGrabber;
		ofxOpenNI2GrabberSettings settings;

		ofFbo fbo;
		ofPixels fboPixels;
		ofImage finalImage;

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
		ofxOscSender sender;
		
		void sendOscVideo();
		void imageToBuffer(ofImage img);

};