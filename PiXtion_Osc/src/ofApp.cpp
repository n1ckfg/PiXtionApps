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
	drawColor = ofToBool(XML.getValue("settings:drawColor", "false"));
	drawDepth = ofToBool(XML.getValue("settings:drawDepth", "true"));
	drawIr = ofToBool(XML.getValue("settings:drawIr", "false"));

	grayImage.allocate(settings.width, settings.height);
    gray.allocate(settings.width, settings.height, OF_IMAGE_GRAYSCALE);        
	
	rgbImage.allocate(settings.width, settings.height);
    rgb.allocate(settings.width, settings.height, OF_IMAGE_COLOR);  

    fbo.allocate(2 * settings.width, settings.height, GL_RGBA); 
    finalImage.allocate(2 * settings.width, settings.height, OF_IMAGE_COLOR); 

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

		grayImage.setFromPixels(oniGrabber.depthSource.noAlphaPixels->getPixels(), settings.width, settings.height);
		grayImage.mirror(false, mirror);
		grayImage.flagImageChanged();
		toOf(grayImage.getCvImage(), gray.getPixelsRef());

		rgbImage.setFromPixels(oniGrabber.rgbSource.currentPixels->getPixels(), settings.width, settings.height);
		rgbImage.mirror(false, mirror);
		rgbImage.flagImageChanged();
		toOf(rgbImage.getCvImage(), rgb.getPixelsRef());

		fbo.begin();
		gray.draw(0,0);
		rgb.draw(settings.width, 0);
		fbo.end();

		ofPixels fboPixels;
		fbo.readToPixels(fboPixels);
		finalImage.setFromPixels(fboPixels);


        switch(videoQuality) {
            case 5:
                ofSaveImage(rgbImage, videoBuffer, OF_IMAGE_FORMAT_JPEG, OF_IMAGE_QUALITY_BEST);
                break;
            case 4:
                ofSaveImage(rgbImage, videoBuffer, OF_IMAGE_FORMAT_JPEG, OF_IMAGE_QUALITY_HIGH);
                break;
            case 3:
                ofSaveImage(rgbImage, videoBuffer, OF_IMAGE_FORMAT_JPEG, OF_IMAGE_QUALITY_MEDIUM);
                break;
            case 2:
                ofSaveImage(rgbImage, videoBuffer, OF_IMAGE_FORMAT_JPEG, OF_IMAGE_QUALITY_LOW);
                break;
            case 1:
                ofSaveImage(rgbImage, videoBuffer, OF_IMAGE_FORMAT_JPEG, OF_IMAGE_QUALITY_WORST);
                break;
        }
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
    ofSetColor(255,255,255);
    ofBackground(0,0,0);

	if (isReady) {
		sendOscVideo();

       	// ~ ~ ~ ~ ~

		/*
		if (settings.doDepth && drawDepth) {
			grayImage.draw(0, 0);
		}

		if (settings.doColor && drawColor) {
			ofTexture& color = oniGrabber.getRGBTextureReference();
			color.draw(grayImage.getWidth(), 0);
		}

		if (settings.doIr && drawIr) {
			ofTexture& ir = oniGrabber.getIRTextureReference();
			ir.draw(grayImage.getWidth(), 0);
		}
		*/
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
void ofApp::sendOscVideo() {
    ofxOscMessage m;
    m.setAddress("/video");
    m.addStringArg(compname);    
    
    m.addBlobArg(videoBuffer);
    
    sender.sendMessage(m);
}