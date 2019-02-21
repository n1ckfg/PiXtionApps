#include "ofApp.h"
#include "ofConstants.h"

using namespace cv;
using namespace ofxCv;

//--------------------------------------------------------------
void ofApp::setup() {
	ofBackground(0, 0, 0, 128);
    
    XML.loadFile("settings.xml");

	settings.width = XML.getValue("settings:width",320);
	settings.height = XML.getValue("settings:height",240);
	settings.fps = XML.getValue("settings:fps",30);;

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
	drawColor = ofToBool(XML.getValue("settings:drawColor", "true"));
	drawDepth = ofToBool(XML.getValue("settings:drawDepth", "true"));
	drawIr = ofToBool(XML.getValue("settings:drawIr", "false"));

	grayImage.allocate(settings.width, settings.height);
    gray.allocate(settings.width, settings.height, OF_IMAGE_GRAYSCALE);        
    colorImage.allocate(settings.width, settings.height);
    color.allocate(settings.width, settings.height, OF_IMAGE_COLOR);        

	isReady = oniGrabber.setup(settings);

	videoQuality = 3;
	host = XML.getValue("settings:host", "Allosaurus.local");
	port = XML.getValue("settings:port", 7110);
	compname = "RPi";
    sender.setup(host, port);
    
    contourSlices = 10;
    contourThreshold = 2.0;
    contourMinAreaRadius = 1.0;
    contourMaxAreaRadius = 250.0;
    contourFinder.setMinAreaRadius(contourMinAreaRadius);
    contourFinder.setMaxAreaRadius(contourMaxAreaRadius);
    trackingColorMode = TRACK_COLOR_RGB;
    minZ = 0.21;
    simplify = XML.getValue("settings:simplify", 0.5);
    smooth = XML.getValue("settings:smooth", 2);

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
		
        /*
        grayImage.setFromPixels(oniGrabber.depthSource.noAlphaPixels->getPixels(), settings.width, settings.height);
		grayImage.mirror(false, mirror);
        grayImage.flagImageChanged();
		toOf(grayImage.getCvImage(), gray.getPixelsRef());
        */

        colorImage.setFromPixels(oniGrabber.rgbSource.currentPixels->getPixels(), settings.width, settings.height);
        colorImage.mirror(false, mirror);
        colorImage.flagImageChanged();
        toOf(colorImage.getCvImage(), color.getPixelsRef());
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
    ofSetColor(255,255,255);
    ofBackground(0,0,0);

	if (isReady) {
	    //if (debug) {
        //ofSetLineWidth(2);
        //ofNoFill();
        //}

        int contourCounter = 0;
        unsigned char * pixels = color.getPixels();
        //unsigned char * pixelsGray = gray.getPixels();
        int gw = color.getWidth();
        //int gwGray = gray.getWidth();

        for (int h=0; h<255; h += int(255/contourSlices)) {
            contourFinder.setThreshold(h);
            contourFinder.findContours(colorImage);
            contourFinder.draw();            

            int n = contourFinder.size();
            for (int i = 0; i < n; i++) {
                ofPolyline line = contourFinder.getPolyline(i);
                line.simplify(simplify);
                //line = line.getResampledBySpacing(1);
                line = line.getSmoothed(smooth, 0.5);
                vector<ofPoint> cvPoints = line.getVertices();
                //vector<float> cvPointsZ;
                vector<ofVec3f> cvCleanPoints;

                int middle = int(cvPoints.size()/2);
                int x = int(cvPoints[middle].x);
                int y = int(cvPoints[middle].y);
                int loc = (x + y * gw) * 3;
                ofColor col = ofColor(pixels[loc], pixels[loc + 1], pixels[loc + 2]);
                //cout << col;
                
                /*
                for (int j=0; j<cvPoints.size(); j++) {
                    int xg = int(cvPoints[j].x);
                    int yg = int(cvPoints[j].y);
                    ofColor colGray = pixelsGray[xg + yg * gwGray];
                    cvPointsZ.push_back(colGray.r);
                }
                */

                float colorData[3]; 
                colorData[0] = col.r;
                colorData[1] = col.g;
                colorData[2] = col.b;
                char const * pColor = reinterpret_cast<char const *>(colorData);
                std::string colorString(pColor, pColor + sizeof colorData);
                contourColorBuffer.set(colorString); 

                for (int j=0; j<cvPoints.size(); j++) {
                    ofVec3f v;
                    v = oniGrabber.convertDepthToWorld((int) cvPoints[j].x, (int) cvPoints[j].y);
                    if (v.z > minZ) cvCleanPoints.push_back(v);
                }

                float pointsData[cvCleanPoints.size() * 3]; 
                for (int j=0; j<cvCleanPoints.size(); j++) {
                    int index = j * 3;
                    pointsData[index] = cvCleanPoints[j].x;
                    pointsData[index+1] = cvCleanPoints[j].y;
                    pointsData[index+2] = cvCleanPoints[j].z;
                }
                char const * pPoints = reinterpret_cast<char const *>(pointsData);
                std::string pointsString(pPoints, pPoints + sizeof pointsData);
                contourPointsBuffer.set(pointsString); 

                sendOscContours(contourCounter);
                contourCounter++;
            }        
        }

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
void ofApp::sendOscContours(int index) {
    ofxOscMessage m;
    m.setAddress("/contour");
    m.addStringArg(compname);
    //m.addStringArg(uniqueId(36));

    m.addIntArg(index);
    m.addBlobArg(contourColorBuffer);
    m.addBlobArg(contourPointsBuffer);

    sender.sendMessage(m);
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