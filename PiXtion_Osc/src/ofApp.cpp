#include "ofApp.h"
#include "ofConstants.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofBackground(0, 0, 0, 128);
    
    XML.loadFile("settings.xml");

	settings.width = XML.getValue("settings:width", 320);
	settings.height = XML.getValue("settings:height", 240);
	settings.fps = XML.getValue("settings:fps", 30);;

	settings.doColor = ofToBool(XML.getValue("settings:do_color", "true")) ;
	settings.doRegisterDepthToColor = ofToBool(XML.getValue("settings:registered", "true"));

	settings.depthPixelFormat = PIXEL_FORMAT_DEPTH_1_MM;
	settings.colorPixelFormat = PIXEL_FORMAT_RGB888;
	settings.irPixelFormat = PIXEL_FORMAT_GRAY16;
	settings.useOniFile = false;
	settings.oniFilePath = "UNDEFINED";

	mirror = ofToBool(XML.getValue("settings:mirror", "false"));

	isReady = oniGrabber.setup(settings);

    depth.allocate(settings.width, settings.height, OF_IMAGE_COLOR);
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

        depth.setFromPixels(oniGrabber.depthSource.currentPixels->getPixels(), settings.width, settings.height);
        imageToBuffer(depth, depthVideoBuffer, depthVideoQuality);
        if (doColor) {
            rgb.setFromPixels(oniGrabber.rgbSource.currentPixels->getPixels(), settings.width, settings.height);
            imageToBuffer(rgb, rgbVideoBuffer, rgbVideoQuality);
        }
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
    //ofSetColor(255,255,255);
    //ofBackground(0,0,0);

	if (isReady) {
		sendOscVideo();
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
    ofxOscMessage msg;
    msg.setAddress("/video");
    msg.addStringArg(compname);    
    
    msg.addBlobArg(depthVideoBuffer);
    if (doColor) msg.addBlobArg(rgbVideoBuffer);
    
    sender.sendMessage(msg);
}

void ofApp::pixelsToBuffer(ofPixels* _pix, ofBuffer& _buffer, int _quality) {
    switch(_quality) {
        case 5:
            ofSaveImage(_pix, _buffer, OF_IMAGE_FORMAT_JPEG, OF_IMAGE_QUALITY_BEST);
            break;
        case 4:
            ofSaveImage(_pix, _buffer, OF_IMAGE_FORMAT_JPEG, OF_IMAGE_QUALITY_HIGH);
            break;
        case 3:
            ofSaveImage(_pix, _buffer, OF_IMAGE_FORMAT_JPEG, OF_IMAGE_QUALITY_MEDIUM);
            break;
        case 2:
            ofSaveImage(_pix, _buffer, OF_IMAGE_FORMAT_JPEG, OF_IMAGE_QUALITY_LOW);
            break;
        case 1:
            ofSaveImage(_pix, _buffer, OF_IMAGE_FORMAT_JPEG, OF_IMAGE_QUALITY_WORST);
            break;
    }
}

void ofApp::imageToBuffer(ofImage _img, ofBuffer& _buffer, int _quality) {
    pixelsToBuffer(_img.getPixels(), _buffer, _quality);
}

void ofApp::fboToBuffer(ofFbo _fbo, ofBuffer& _buffer, int _quality) {
    // jpegs have no alpha, so fbo must be initialized with GL_RGB, not GL_RGBA!
    ofPixels fboPixels;
    _fbo.readToPixels(fboPixels);
    pixelsToBuffer(fboPixels, _buffer, _quality);
}