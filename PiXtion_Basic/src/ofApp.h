#pragma once

#include "ofMain.h"
#include "ofxOpenNI2Grabber.h"
#include "ofxOpenNI2Recorder.h"
#include "ofxOpenCv.h"
#include "ofxXmlSettings.h"
#include "PopMessage.h"

class ofApp : public ofBaseApp {

	public:
		void setup();
		void update();
		void draw();
		void exit();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		void saveXMLSettings();
		void emptyBins(); 

		bool isReady;

		bool calculateActivity;
		bool mirrorThreshold;
		bool findContours;
		bool drawThresholded;
		bool drawColor;
		bool drawOni;
		bool thresholdWithOpenCV;
		bool useLowResValues;
		bool enableRecorder;

		int nearThreshold;
		int farThreshold;
		int nearClip;
		int farClip;

		ofFbo fbo;

		ofxOpenNI2Recorder recorder;
		ofxOpenNI2Grabber oniGrabber;
		ofxOpenNI2GrabberSettings settings;

		ofxCvGrayscaleImage grayImage; // grayscale depth image
		ofxCvGrayscaleImage grayThreshNear; // the near thresholded image
		ofxCvGrayscaleImage grayThreshFar; // the far thresholded image

		ofxCvContourFinder contourFinder;

		ofTrueTypeFont	verdana48;
		PopMessage statusMessage;

		vector<int> bins;
		int binCount;
		int binWidth;
		float damping;
		
        ofxXmlSettings XML;
        string xmlStructure;
        string message;

};