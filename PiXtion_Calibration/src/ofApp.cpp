#include "ofApp.h"
#include "ofConstants.h"
#include "../../common/src/PiXtionUtils.hpp"

using namespace cv;
using namespace ofxCv;
using namespace PiXtionUtils;

//--------------------------------------------------------------
void ofApp::setup() {
    ofBackground(0, 0, 0, 128);
    
    XML.loadFile("settings.xml");

    settings.width = XML.getValue("settings:width", 320);
    settings.height = XML.getValue("settings:height", 240);
    settings.fps = XML.getValue("settings:fps", 30);;

    settings.doDepth = ofToBool(XML.getValue("settings:doDepth", "true"));
    settings.doRawDepth = true;
    settings.doColor = ofToBool(XML.getValue("settings:doColor", "true")) ;
    settings.doIr = ofToBool(XML.getValue("settings:doIr", "false")) ;
    settings.doRegisterDepthToColor = ofToBool(XML.getValue("settings:registered", "true"));

    settings.depthPixelFormat = PIXEL_FORMAT_DEPTH_1_MM;
    settings.colorPixelFormat = PIXEL_FORMAT_RGB888;
    settings.irPixelFormat = PIXEL_FORMAT_GRAY16;
    settings.useOniFile = false;
    settings.oniFilePath = "UNDEFINED";

    mirror = ofToBool(XML.getValue("settings:mirror", "false"));

    depth.allocate(settings.width * 12, settings.height * 12, OF_IMAGE_COLOR);        
    depthCv.allocate(settings.width * 12, settings.height * 12);        
    rgb.allocate(settings.width, settings.height, OF_IMAGE_COLOR);        

    isReady = oniGrabber.setup(settings);

    rgbVideoQuality = XML.getValue("settings:rgb_video_quality", 3);
    depthVideoQuality = XML.getValue("settings:depth_video_quality", 3);
    host = XML.getValue("settings:host", "127.0.0.1");
    port = XML.getValue("settings:port", 7110);
    
    compname = createCompName("RPi");
    sender.setup(host, port);      
}

//--------------------------------------------------------------
void ofApp::update() {
    if (isReady) {
        oniGrabber.update();
        
        rgb.setFromPixels(oniGrabber.rgbSource.currentPixels->getPixels(), settings.width, settings.height, OF_IMAGE_COLOR);
        imageToBuffer(rgb, rgbVideoBuffer, rgbVideoQuality);

        float pointsData[settings.width * settings.height * 3];
        for (int x=0; x<settings.width; x++) {
            for (int y=0; y<settings.height; y++) {
                int loc = (x + y * settings.width) * 3;
                ofVec3f v = ofVec3f(0,0,0);
                v = oniGrabber.convertDepthToWorld(x, y);  
                pointsData[loc] = v.x;
                pointsData[loc+1] = v.y;
                pointsData[loc+2] = v.z;
            }
        }

        unsigned char * points = reinterpret_cast<unsigned char *>(pointsData);
        //ofPixels pixels;
        //pixels.setFromExternalPixels(points, settings.width * 12, settings.height * 12, 3);
        depthCv.setFromPixels(points, settings.width * 12, settings.height * 12);
        depthCv.flagImageChanged();
        toOf(depthCv.getCvImage(), depth.getPixelsRef());
        imageToBuffer(depth, depthVideoBuffer, depthVideoQuality); 
        //std::string pointsString(pPoints, pPoints + sizeof pointsData);
        //pointsBuffer.set(pointsString); 

        sendOscPoints();
    }
}

//--------------------------------------------------------------
void ofApp::draw() {
    //ofSetColor(255,255,255);
    //ofBackground(0,0,0);
}

//--------------------------------------------------------------
void ofApp::exit() {
    ofLogVerbose() << "\n EXITING, be patient - takes some time \n";
    
    if (isReady) {
        oniGrabber.close();
    }
}
//--------------------------------------------------------------
void ofApp::sendOscPoints() {
    ofxOscMessage msg;
    msg.setAddress("/points");
    msg.addStringArg(compname);
    msg.addBlobArg(rgbVideoBuffer);
    msg.addBlobArg(depthVideoBuffer);

    sender.sendMessage(msg);
}
