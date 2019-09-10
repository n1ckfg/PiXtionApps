#pragma once

#include "ofMain.h"
#include "ofxOpenNI2Grabber.h"
#include "ofxOpenNI2Recorder.h"
#include "ofxOsc.h"
#include "ofxXmlSettings.h"

class ofApp : public ofBaseApp {

	public:
		void setup();
		void update();
		void draw();
		void exit();

		bool isReady;

		ofxOpenNI2Grabber oniGrabber;
		ofxOpenNI2GrabberSettings settings;

		ofImage depth, rgb;

        ofxXmlSettings XML;
        string xmlStructure, message;

		ofFile file;
        string compname, host; // hostname;
		int port; // default 7110;
		
		int depthVideoQuality, rgbVideoQuality; // 5 best to 1 worst, default 3 medium
		ofBuffer depthVideoBuffer, rgbVideoBuffer;
		
		ofxOscSender sender;
		
		void sendOscVideo();
		
		void imageToBuffer(ofImage _img, ofBuffer& _buffer, int _quality);
		void pixelsToBuffer(ofPixels _pix, ofBuffer& _buffer, int _quality);
		void fboToBuffer(ofFbo _fbo, ofBuffer& _buffer, int _quality);
		
};