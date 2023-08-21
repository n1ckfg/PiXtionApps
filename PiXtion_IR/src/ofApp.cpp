#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofBackground(0, 0, 0, 128);
    
    XML.loadFile("settings.xml");

	settings.width = 640;//XML.getValue("settings:width", 320);
	settings.height = 480;//XML.getValue("settings:height", 240);
	settings.fps = 30;//XML.getValue("settings:fps", 30);;

	settings.doDepth = false;//ofToBool(XML.getValue("settings:doDepth", "true"));
	settings.doColor = false;//ofToBool(XML.getValue("settings:doColor", "true")) ;
	settings.doIr = true;//ofToBool(XML.getValue("settings:doIr", "false")) ;
    settings.doRawDepth = false;
	settings.doRegisterDepthToColor = false;//ofToBool(XML.getValue("settings:registered", "true"));

	settings.depthPixelFormat = PIXEL_FORMAT_DEPTH_1_MM;
	settings.colorPixelFormat = PIXEL_FORMAT_RGB888;
	settings.irPixelFormat = PIXEL_FORMAT_GRAY16;
	settings.useOniFile = false;
	settings.oniFilePath = "UNDEFINED";

	isReady = oniGrabber.setup(settings);
}

//--------------------------------------------------------------
void ofApp::update() {
	if (isReady) {
		oniGrabber.update();
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
    ofSetColor(255);
    ofBackground(0,0,0);

	if (isReady) {
		/*
		if (settings.doDepth) {
			ofTexture& depth = oniGrabber.getDepthTextureReference();
			depth.draw(0, 0);
		}

		// can't do color and IR togther
		if (settings.doColor) {
			ofTexture& rgb = oniGrabber.getRGBTextureReference();
			rgb.draw(settings.width, 0);
		} else if (settings.doIr) {
			ofTexture& ir = oniGrabber.getIRTextureReference();
			ir.draw(settings.width, 0);
		}
		*/
		ofTexture& ir = oniGrabber.getIRTextureReference();
		ir.draw(0, 0, ofGetWidth(), ofGetHeight());
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
