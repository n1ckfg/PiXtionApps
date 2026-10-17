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
    host = XML.getValue("settings:host", "127.0.0.1");
    port = XML.getValue("settings:port", 7110);
    compname = getHostName();
    sender.setup(host, port);
    
    minZ = 0.21;
    simplify = XML.getValue("settings:simplify", 0.5);
    smooth = XML.getValue("settings:smooth", 2);

    xScale = 720.0 / 320.0;
    yScale = 480.0 / 240.0;    

    // start at the sensor, looking into the middle of its range; drag to orbit
    cam.setTarget(glm::vec3(0, 0, -2000));
    cam.setDistance(2000);
    cam.setNearClip(10);
    cam.setFarClip(20000);
}

//--------------------------------------------------------------
void ofApp::update() {
    if (isReady) {
        oniGrabber.update();
        
        // depth pixels are RGBA, colorCv expects RGB
        ofPixels depthPixels = *oniGrabber.depthSource.currentPixels;
        depthPixels.setNumChannels(3);
        colorCv.setFromPixels(depthPixels);
        colorCv.mirror(false, mirror);
        colorCv.flagImageChanged();
        toOf(colorCv.getCvMat(), color.getPixels());
    }
}

//--------------------------------------------------------------
void ofApp::draw() {
    if (isReady) {
        // drawing must happen in draw(), anything drawn in update() is cleared
        cam.begin();
        // the points are real-world mm with z pointing away from the sensor;
        // flip z so they sit in front of the camera
        ofScale(1, 1, -1);

        //int contourCounter = 0;
        unsigned char * pixels = color.getPixels().getData();

        for (int y=0; y<(int)settings.height; y ++) {
            line.clear();
            vector<int> lineX;

            for (int x=0; x<(int)settings.width; x++) {
                ofVec3f v;
                v = oniGrabber.convertDepthToWorld(x, y);
                if (v.z > minZ) {
                	//v.x *= xScale;
                	//v.y *+ yScale;
                	line.addVertex(v);
                	lineX.push_back(x);
                }
            }

            // color by the depth at the line's middle point, like the contour apps;
            // the image center is often empty, which left the whole line black
            int mx = lineX.empty() ? int(settings.width/2) : lineX[lineX.size()/2];
            if (mirror) mx = settings.width - 1 - mx;
            int loc = (mx + y * settings.width) * 3;
            ofColor col = ofColor(pixels[loc], pixels[loc + 1], pixels[loc + 2]);
            ofSetColor(col);

            /*
            float colorData[3]; 
            colorData[0] = pixels[loc]; //col.r;
            colorData[1] = pixels[loc+1]; //col.g;
            colorData[2] = pixels[loc+2]; //col.b;
            
            char const * pColor = reinterpret_cast<char const *>(colorData);
            std::string colorString(pColor, pColor + sizeof colorData);
            colorBuffer.set(colorString); 
			*/

            line.simplify(simplify);
            line = line.getSmoothed(smooth, 0.5);
            line.draw();

            /*
            vector<glm::vec3> points = line.getVertices();

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
            */
            //contourCounter++;
        }

        cam.end();
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
void ofApp::sendOscScanline(int index) {
    ofxOscMessage msg;
    msg.setAddress("/scanline");
    msg.addStringArg(compname);

    msg.addIntArg(index);
    msg.addBlobArg(colorBuffer);
    msg.addBlobArg(pointsBuffer);

    sender.sendMessage(msg);
}

