#include "ofApp.h"
#include "ofConstants.h"

using namespace cv;
using namespace ofxCv;

//--------------------------------------------------------------
void ofApp::setup() {
	ofBackground(0, 0, 0, 128);
    
    XML.loadFile("settings.xml");

	settings.width = XML.getValue("settings:width", 320);
	settings.height = XML.getValue("settings:height", 240);
	settings.fps = XML.getValue("settings:fps", 30);;

	settings.doColor = ofToBool(XML.getValue("settings:doColor", "true")) ;
	settings.doRegisterDepthToColor = ofToBool(XML.getValue("settings:registered", "true"));

	settings.depthPixelFormat = PIXEL_FORMAT_DEPTH_1_MM;
	settings.colorPixelFormat = PIXEL_FORMAT_RGB888;
	settings.irPixelFormat = PIXEL_FORMAT_GRAY16;
	settings.useOniFile = false;
	settings.oniFilePath = "UNDEFINED";

	mirror = ofToBool(XML.getValue("settings:mirror", "false"));

    if (settings.doColor) {
    	fbo.allocate(2 * settings.width, settings.height, GL_RGBA); 
    	fboPixels.allocate(2 * settings.width, settings.height, OF_IMAGE_COLOR);
    	finalImage.allocate(2 * settings.width, settings.height, OF_IMAGE_COLOR); 
	} else {
    	fbo.allocate(settings.width, settings.height, GL_RGBA); 
    	fboPixels.allocate(settings.width, settings.height, OF_IMAGE_COLOR);
    	finalImage.allocate(settings.width, settings.height, OF_IMAGE_COLOR); 		
	}

	isReady = oniGrabber.setup(settings);

	videoQuality = XML.getValue("settings:videoQuality", 3);
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

		fbo.begin();
		ofTexture& depth = oniGrabber.getDepthTextureReference();
		depth.draw(0, 0);

		if (settings.doColor) {
			ofTexture& rgb = oniGrabber.getRGBTextureReference();
			rgb.draw(settings.width, 0);
		}
		fbo.end();

		fbo.readToPixels(fboPixels);
		finalImage.setFromPixels(fboPixels);

		imageToBuffer(fboPixels);
	}
}

void ofApp::imageToBuffer(ofImage img) {
        switch(videoQuality) {
            case 5:
                ofSaveImage(img, videoBuffer, OF_IMAGE_FORMAT_JPEG, OF_IMAGE_QUALITY_BEST);
                break;
            case 4:
                ofSaveImage(img, videoBuffer, OF_IMAGE_FORMAT_JPEG, OF_IMAGE_QUALITY_HIGH);
                break;
            case 3:
                ofSaveImage(img, videoBuffer, OF_IMAGE_FORMAT_JPEG, OF_IMAGE_QUALITY_MEDIUM);
                break;
            case 2:
                ofSaveImage(img, videoBuffer, OF_IMAGE_FORMAT_JPEG, OF_IMAGE_QUALITY_LOW);
                break;
            case 1:
                ofSaveImage(img, videoBuffer, OF_IMAGE_FORMAT_JPEG, OF_IMAGE_QUALITY_WORST);
                break;
        }
}

//--------------------------------------------------------------
void ofApp::draw() {
    ofSetColor(255,255,255);
    ofBackground(0,0,0);

	if (isReady) {
		sendOscVideo();
	}

	fbo.draw(0,0);
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
    ofxOscMessage m;
    m.setAddress("/video");
    m.addStringArg(compname);    
    
    m.addBlobArg(videoBuffer);
    
    sender.sendMessage(m);
}