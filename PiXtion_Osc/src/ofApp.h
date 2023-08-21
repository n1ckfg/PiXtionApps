#pragma once

#include "ofMain.h"
#include "ofConstants.h"
#include "ofxOpenNI2Grabber.h"
#include "ofxOpenNI2Recorder.h"
#include "ofxOsc.h"
#include "ofxXmlSettings.h"

#include "../../../core/common/src/Pinopticon.hpp"

class ofApp : public ofBaseApp {

	public:
		void setup();
		void update();
		void draw();
		void exit();

		bool isReady, mirror;

		ofxOpenNI2Grabber oniGrabber;
		ofxOpenNI2GrabberSettings settings;

		ofImage depth, rgb;

        ofxXmlSettings XML;
        string xmlStructure, message;

        string compname, host; // hostname;
		int port; // default 7110;
		
		int depthVideoQuality, rgbVideoQuality; // 5 best to 1 worst, default 3 medium
		ofBuffer depthVideoBuffer, rgbVideoBuffer;
		
		ofxOscSender sender;
		
		void sendOscVideo();
		
};