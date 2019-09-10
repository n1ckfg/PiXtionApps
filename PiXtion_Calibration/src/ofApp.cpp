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
    rgb.allocate(settings.width, settings.height, OF_IMAGE_COLOR);        

    isReady = oniGrabber.setup(settings);

    rgbVideoQuality = XML.getValue("settings:rgb_video_quality", 3);
    depthVideoQuality = XML.getValue("settings:depth_video_quality", 3);
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
        compname += "_" + ofGetTimestampString("%y%m%d%H%M%S%i");
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

        //char const * pPoints = reinterpret_cast<char const *>(pointsData);
        unsigned char* points = reinterpret_cast<unsigned char*>(pointsData);
        depth.setFromPixels(points, settings.width * 12, settings.height * 12, OF_IMAGE_COLOR);
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

float ofApp::rawDepthToMeters(int depthValue) {
  if (depthValue < 2047) {
    return (float)(1.0 / ((double)(depthValue) * -0.0030711016 + 3.3309495161));
  }
  return 0.0;
}

string ofApp::uniqueId(int len) {
    long seed = long(ofRandom(0, 1000000));
    cout << seed << "   "; 
    srand(seed);
    string str = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    string newstr;
    int pos;
    while(newstr.size() != len) {
       pos = ((rand() % (str.size() - 1)));
       newstr += str.substr(pos,1);
    }

    cout << newstr << "\n";
    return newstr;
}

void ofApp::imageToBuffer(ofImage _img, ofBuffer& _buffer, int _quality) {
    switch(_quality) {
        case 5:
            ofSaveImage(_img, _buffer, OF_IMAGE_FORMAT_JPEG, OF_IMAGE_QUALITY_BEST);
            break;
        case 4:
            ofSaveImage(_img, _buffer, OF_IMAGE_FORMAT_JPEG, OF_IMAGE_QUALITY_HIGH);
            break;
        case 3:
            ofSaveImage(_img, _buffer, OF_IMAGE_FORMAT_JPEG, OF_IMAGE_QUALITY_MEDIUM);
            break;
        case 2:
            ofSaveImage(_img, _buffer, OF_IMAGE_FORMAT_JPEG, OF_IMAGE_QUALITY_LOW);
            break;
        case 1:
            ofSaveImage(_img, _buffer, OF_IMAGE_FORMAT_JPEG, OF_IMAGE_QUALITY_WORST);
            break;
    }
}

void ofApp::pixelsToBuffer(ofPixels _pix, ofBuffer& _buffer, int _quality) {
    ofImage img;
    img.setFromPixels(_pix);
    imageToBuffer(img, _buffer, _quality);
}

void ofApp::fboToBuffer(ofFbo _fbo, ofBuffer& _buffer, int _quality) {
    // jpegs have no alpha, so fbo must be initialized with GL_RGB, not GL_RGBA!
    ofPixels pixels;
    ofImage img;
    _fbo.readToPixels(pixels);
    img.setFromPixels(pixels);
    imageToBuffer(img, _buffer, _quality);
}