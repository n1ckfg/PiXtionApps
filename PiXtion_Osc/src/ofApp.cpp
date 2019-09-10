#include "ofApp.h"
#include "ofConstants.h"

using namespace PiXtionUtils;

//--------------------------------------------------------------
void ofApp::setup() {
	ofBackground(0, 0, 0, 128);
    
    XML.loadFile("settings.xml");

	settings.width = XML.getValue("settings:width", 320);
	settings.height = XML.getValue("settings:height", 240);
	settings.fps = XML.getValue("settings:fps", 30);;

    settings.doColor = ofToBool(XML.getValue("settings:do_color", "true")) ;
    settings.doColor = ofToBool(XML.getValue("settings:do_color", "true")) ;
    settings.doRawDepth = false;
	settings.doRegisterDepthToColor = ofToBool(XML.getValue("settings:registered", "true"));

	settings.depthPixelFormat = PIXEL_FORMAT_DEPTH_1_MM;
	settings.colorPixelFormat = PIXEL_FORMAT_RGB888;
	settings.irPixelFormat = PIXEL_FORMAT_GRAY16;
	settings.useOniFile = false;
	settings.oniFilePath = "UNDEFINED";

	mirror = ofToBool(XML.getValue("settings:mirror", "false"));

	isReady = oniGrabber.setup(settings);

    depth.allocate(settings.width, settings.height, OF_IMAGE_GRAYSCALE);
    rgb.allocate(settings.width, settings.height, OF_IMAGE_COLOR);

    depthVideoQuality = XML.getValue("settings:depth_video_quality", 3);
    rgbVideoQuality = XML.getValue("settings:rgb_video_quality", 3);
	
    host = XML.getValue("settings:host", "127.0.0.1");
	port = XML.getValue("settings:port", 7110);
	compname = "RPi";
    sender.setup(host, port);
    
    file.open(ofToDataPath("compname.txt"), ofFile::ReadWrite, false);
    ofBuffer buff;
    if (file) {
        buff = file.readToBuffer();
        compname = buff.getText();
    } else {
        compname += "_" + ofGetTimestampString("%y-%m-%d-%H-%M-%S-%i");
        ofStringReplace(compname, "-", "");
        ofStringReplace(compname, "\n", "");
        ofStringReplace(compname, "\r", "");
        buff.set(compname.c_str(), compname.size());
        ofBufferToFile("compname.txt", buff);
    }
    cout << compname;

}

//--------------------------------------------------------------
void ofApp::update() {
	if (isReady) {
		oniGrabber.update();

        depth.setFromPixels(oniGrabber.depthSource.noAlphaPixels->getPixels(), settings.width, settings.height, OF_IMAGE_GRAYSCALE);
        PiXtionUtils::imageToBuffer(depth, depthVideoBuffer, depthVideoQuality);
        
        if (settings.doColor) {
            rgb.setFromPixels(oniGrabber.rgbSource.currentPixels->getPixels(), settings.width, settings.height, OF_IMAGE_COLOR);
            PiXtionUtils::imageToBuffer(rgb, rgbVideoBuffer, rgbVideoQuality);
        }

        sendOscVideo();
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
    /*
    ofSetColor(255,255,255);
    ofBackground(0,0,0);
    depth.draw(0,0);
    rgb.draw(settings.width, 0);
    */
}

//--------------------------------------------------------------
void ofApp::exit() {
	ofLogVerbose() << "\n EXITING, be patient - takes some time \n";
	
	if (isReady) {
		oniGrabber.close();
	}
}
//--------------------------------------------------------------
void ofApp::sendOscVideo() {
    ofxOscMessage msg;
    msg.setAddress("/video");
    msg.addStringArg(compname);    
    
    msg.addBlobArg(depthVideoBuffer);
    if (settings.doColor) msg.addBlobArg(rgbVideoBuffer);
    
    sender.sendMessage(msg);
}

