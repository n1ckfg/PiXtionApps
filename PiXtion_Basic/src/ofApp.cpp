#include "ofApp.h"
#include "ofConstants.h"
//--------------------------------------------------------------
void ofApp::setup() {
	ofBackground(0, 0, 0, 128);
    
    XML.loadFile("settings.xml");

	settings.width = XML.getValue("settings:width", 320);
	settings.height = XML.getValue("settings:height", 240);
	settings.fps = XML.getValue("settings:fps", 30);;

	settings.doDepth = ofToBool(XML.getValue("settings:doDepth", "true"));
	settings.doRawDepth = ofToBool(XML.getValue("settings:doRawDepth", "false")) ;
	settings.doColor = ofToBool(XML.getValue("settings:doColor", "false")) ;
	settings.doIr = ofToBool(XML.getValue("settings:doIr", "false")) ;
	settings.doRegisterDepthToColor = ofToBool(XML.getValue("settings:registered", "false"));

	settings.depthPixelFormat = PIXEL_FORMAT_DEPTH_1_MM;
	settings.colorPixelFormat = PIXEL_FORMAT_RGB888;
	settings.irPixelFormat = PIXEL_FORMAT_GRAY16;
	settings.useOniFile = false;
	settings.oniFilePath = "UNDEFINED";

	mirror = ofToBool(XML.getValue("settings:mirror", "false"));
	drawDepth = ofToBool(XML.getValue("settings:drawDepth", "true"));
	drawColor = ofToBool(XML.getValue("settings:drawColor", "false"));
	drawIr = ofToBool(XML.getValue("settings:drawIr", "false"));

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
    ofSetColor(255,255,255);
    ofBackground(0,0,0);

	if (isReady) {
		if (settings.doDepth && drawDepth) {
			ofTexture& depth = oniGrabber.getDepthTextureReference();
			depth.mirror(false, mirror);
			depth.draw(0, 0);
		}

		if (settings.doColor && drawColor) {
			ofTexture& rgb = oniGrabber.getRGBTextureReference();
			rgb.mirror(false, mirror);
			rgb.draw(settings.width, 0);
		}

		if (settings.doIr && drawIr) {
			ofTexture& ir = oniGrabber.getIRTextureReference();
			ir.mirror(false, mirror);
			ir.draw(settings.width*2, 0);
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
