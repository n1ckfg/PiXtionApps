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

    gray.allocate(settings.width, settings.height, OF_IMAGE_GRAYSCALE);        
    color.allocate(settings.width, settings.height, OF_IMAGE_COLOR);        
    colorCv.allocate(settings.width, settings.height);

    isReady = oniGrabber.setup(settings);

    videoQuality = XML.getValue("settings:videoQuality", 3);
    host = XML.getValue("settings:host", "127.0.0.1");
    port = XML.getValue("settings:port", 7110);
    compname = createCompName("RPi");
    sender.setup(host, port);
    
    minZ = 0.21;
}

//--------------------------------------------------------------
void ofApp::update() {
    if (isReady) {
        oniGrabber.update();
        
        colorImage.setFromPixels(oniGrabber.rgbSource.currentPixels->getPixels(), settings.width, settings.height);
        colorImage.mirror(false, mirror);
        colorImage.flagImageChanged();
        toOf(colorImage.getCvImage(), color.getPixelsRef());

        int contourCounter = 0;
        unsigned char * pixels = color.getPixelsRef();

        for (int y=0; y<settings.height; y ++) {
            vector<ofPoint> points;

            int mx = int(settings.width/2);
            int loc = (mx + y * settings.width) * 3;
            ofColor col = ofColor(pixels[loc], pixels[loc + 1], pixels[loc + 2]);

            float colorData[3]; 
            colorData[0] = col.r;
            colorData[1] = col.g;
            colorData[2] = col.b;
            char const * pColor = reinterpret_cast<char const *>(colorData);
            std::string colorString(pColor, pColor + sizeof colorData);
            colorBuffer.set(colorString); 

            for (int x=0; x<settings.width; x++) {
                ofVec3f v;
                v = oniGrabber.convertDepthToWorld(x, y);
                if (v.z > minZ) points.push_back(v);
            }

            float pointsData[points.size() * 3]; 

            for (int x=0; x<points.size(); x++) {
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
    msg.setAddress("/scanline");
    msg.addStringArg(compname);

    msg.addIntArg(index);
    msg.addBlobArg(colorBuffer);
    msg.addBlobArg(pointsBuffer);

    sender.sendMessage(msg);
}

