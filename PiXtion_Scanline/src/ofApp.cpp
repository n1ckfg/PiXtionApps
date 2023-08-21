#include "ofApp.h"

#include "../../../core/common/src/Pinopticon.hpp"

using namespace cv;
using namespace ofxCv;
using namespace Pinopticon;

//--------------------------------------------------------------
void ofApp::setup() {
    ofBackground(0, 0, 0, 128);
    
    XML.loadFile("settings.xml");

    settings.width = XML.getValue("settings:width", 320);
    settings.height = XML.getValue("settings:height", 240);
    settings.fps = XML.getValue("settings:fps", 30);;

    settings.doDepth = ofToBool(XML.getValue("settings:doDepth", "true"));
    settings.doRawDepth = ofToBool(XML.getValue("settings:doRawDepth", "true")) ;
    settings.doColor = ofToBool(XML.getValue("settings:doColor", "true")) ;
    settings.doIr = ofToBool(XML.getValue("settings:doIr", "false")) ;
    settings.doRegisterDepthToColor = ofToBool(XML.getValue("settings:registered", "true"));

    settings.depthPixelFormat = PIXEL_FORMAT_DEPTH_1_MM;
    settings.colorPixelFormat = PIXEL_FORMAT_RGB888;
    settings.irPixelFormat = PIXEL_FORMAT_GRAY16;
    settings.useOniFile = false;
    settings.oniFilePath = "UNDEFINED";

    mirror = ofToBool(XML.getValue("settings:mirror", "false"));

    color.allocate(settings.width, settings.height, OF_IMAGE_COLOR);        
    colorCv.allocate(settings.width, settings.height);

    isReady = oniGrabber.setup(settings);

    depthVideoQuality = XML.getValue("settings:depth_video_quality", 3);
    rgbVideoQuality = XML.getValue("settings:rgb_video_quality", 3);
    oscHost = XML.getValue("settings:host", "127.0.0.1");
    oscPort = XML.getValue("settings:port", 7110);
    
    hostName = getHostName();
    sessionId = getSessionId();
    
    sender.setup(oscHost, oscPort);
    
    minZ = 0.21;
    simplify = XML.getValue("settings:simplify", 0.5);
    smooth = XML.getValue("settings:smooth", 2);
}

//--------------------------------------------------------------
void ofApp::update() {
    if (isReady) {
        oniGrabber.update();
        
        colorCv.setFromPixels(oniGrabber.rgbSource.currentPixels->getPixels(), settings.width, settings.height);
        colorCv.mirror(false, mirror);
        colorCv.flagImageChanged();
        toOf(colorCv.getCvImage(), color.getPixelsRef());

        int contourCounter = 0;
        unsigned char * pixels = color.getPixels();

        for (int y=0; y<(int)settings.height; y ++) {
            line.clear();

            int mx = int(settings.width/2);
            int loc = (mx + y * settings.width) * 3;
           //ofColor col = ofColor(pixels[loc], pixels[loc + 1], pixels[loc + 2]);

            float colorData[3]; 
            colorData[0] = pixels[loc]; //col.r;
            colorData[1] = pixels[loc+1]; //col.g;
            colorData[2] = pixels[loc+2]; //col.b;
            
            char const * pColor = reinterpret_cast<char const *>(colorData);
            std::string colorString(pColor, pColor + sizeof colorData);
            colorBuffer.set(colorString); 

            for (int x=0; x<(int)settings.width; x++) {
                ofVec3f v;
                v = oniGrabber.convertDepthToWorld(x, y);
                if (v.z > minZ) line.addVertex(v);
            }

            line.simplify(simplify);
            line = line.getSmoothed(smooth, 0.5);
            vector<ofPoint> points = line.getVertices();

            float pointsData[points.size() * 3]; 

            for (int x=0; x<(int)points.size(); x++) {
                int index = x * 3;
                pointsData[index] = points[x].x;
                pointsData[index+1] = points[x].y;
                pointsData[index+2] = points[x].z;
            }

            char const * pPoints = reinterpret_cast<char const *>(pointsData);
            std::string pointsString(pPoints, pPoints + sizeof pointsData);
            pointsBuffer.set(pointsString); 

            sendOscScanline(contourCounter);
            contourCounter++;
        }
    }
}

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
void ofApp::sendOscScanline(int index) {
    ofxOscMessage msg;
    msg.setAddress("/contour");
    msg.addStringArg(hostName);
    msg.addStringArg(sessionId);

    msg.addIntArg(index);
    msg.addBlobArg(colorBuffer);
    msg.addBlobArg(pointsBuffer);

    msg.addIntArg(0);

    sender.sendMessage(msg);
}

