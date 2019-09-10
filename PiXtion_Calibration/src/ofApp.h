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
		bool doIr;

		ofxOpenNI2Grabber oniGrabber;
		ofxOpenNI2GrabberSettings settings;

		ofImage depth, rgb;

        ofxXmlSettings XML;
        string xmlStructure;
        string message;

		ofFile file;
        string compname;
		string host; // hostname;
		int port; // default 7110;
		int depthVideoQuality; // 5 best to 1 worst, default 3 medium
		int rgbVideoQuality; // 5 best to 1 worst, default 3 medium
		ofBuffer depthVideoBuffer;
		ofBuffer rgbVideoBuffer;
		float minZ;

		ofxOscSender sender;
		void sendOscPoints();
		float rawDepthToMeters(int depthValue);
		string uniqueId(int len);

		void imageToBuffer(ofImage _img, ofBuffer& _buffer, int _quality);
		void pixelsToBuffer(ofPixels _pix, ofBuffer& _buffer, int _quality);
		void fboToBuffer(ofFbo _fbo, ofBuffer& _buffer, int _quality);

};