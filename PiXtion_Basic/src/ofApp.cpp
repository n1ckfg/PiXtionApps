#include "ofApp.h"

#include "../../../core/common/src/Pinopticon.hpp"

//--------------------------------------------------------------
void ofApp::setup() {
	ofBackground(0, 0, 0, 128);
    
    XML.loadFile("settings.xml");

	settings.width = XML.getValue("settings:width", 320);
	settings.height = XML.getValue("settings:height", 240);
	settings.fps = XML.getValue("settings:fps", 30);;

	settings.doDepth = ofToBool(XML.getValue("settings:doDepth", "true"));
	settings.doColor = ofToBool(XML.getValue("settings:doColor", "true")) ;
	settings.doIr = ofToBool(XML.getValue("settings:doIr", "false")) ;
    settings.doRawDepth = false;
	settings.doRegisterDepthToColor = ofToBool(XML.getValue("settings:registered", "true"));

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
