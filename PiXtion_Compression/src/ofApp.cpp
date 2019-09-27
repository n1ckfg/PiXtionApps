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

    depthLineWidth = settings.width*4;
    depth.allocate(depthLineWidth, 1, OF_IMAGE_COLOR);        
    depthCv.allocate(depthLineWidth, 1);        

    rgb.allocate(settings.width, 1, OF_IMAGE_COLOR);        
    rgbCv.allocate(settings.width, 1);        

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
        
        int contourCounter = 0;

        for (int y=0; y<(int)settings.height; y ++) {
            float pointsData[settings.width*3];
            unsigned char colorData[settings.width*3];
            unsigned char * pixels = oniGrabber.rgbSource.currentPixels->getPixels();

            for (int x=0; x<(int)settings.width; x++) {
                int index=x*3;
                int loc = (x + y * settings.width) * 3;

                ofVec3f v;
                v = oniGrabber.convertDepthToWorld(x, y);
                pointsData[index] = v.x;
                pointsData[index+1] = v.y;
                pointsData[index+2] = v.z;

                colorData[index] = pixels[loc];
                colorData[index+1] = pixels[loc+1];
                colorData[index+2] = pixels[loc+2];
            }

            char const * charData1 = reinterpret_cast<char const *>(pointsData);      
            int n = sizeof(pointsData) / sizeof(pointsData[0]);
            vector<unsigned char> zipPrepVector(charData1, charData1 + n);
            vector<unsigned char> zippedFile = ofxZip::compress(zipPrepVector);

            unsigned char charData2[zippedFile.size()];
            std::copy(zippedFile.begin(), zippedFile.end(), charData2);
            char const * bufferArray = reinterpret_cast<char const *>(charData2);

            depthVideoBuffer.set(bufferArray);

            rgbCv.setFromPixels(colorData, settings.width, 1);
            rgbCv.flagImageChanged();
            toOf(rgbCv.getCvImage(), rgb.getPixelsRef());
            imageToBuffer(rgb, rgbVideoBuffer, rgbVideoQuality);

            sendOscPoints(contourCounter);
            contourCounter++;
        }
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
void ofApp::sendOscPoints(int index) {
    ofxOscMessage msg;
    msg.setAddress("/points");
    msg.addStringArg(compname);
    msg.addIntArg(index);
    msg.addBlobArg(rgbVideoBuffer);
    msg.addBlobArg(depthVideoBuffer);

    sender.sendMessage(msg);
}
