#include "ofApp.h"
#include "ofConstants.h"
//--------------------------------------------------------------
void ofApp::setup() {
	ofBackground(0, 0, 0, 128);
	ofTrueTypeFont::setGlobalDpi(72);
    
    XML.loadFile("settings.xml");

	settings.width 	= XML.getValue("settings:width",320);
	settings.height = XML.getValue("settings:height",240);
	settings.fps 	= XML.getValue("settings:fps",30);;

	settings.doDepth 	= ofToBool(XML.getValue("settings:doDepth", "true"));
	settings.doRawDepth = ofToBool(XML.getValue("settings:doRawDepth", "false")) ;
	settings.doColor 	= ofToBool(XML.getValue("settings:doColor", "false")) ;
	settings.doIr 		= ofToBool(XML.getValue("settings:doIr", "false")) ;

	settings.depthPixelFormat 	= PIXEL_FORMAT_DEPTH_1_MM;
	settings.colorPixelFormat 	= PIXEL_FORMAT_RGB888;
	settings.irPixelFormat 		= PIXEL_FORMAT_GRAY16;
	settings.doRegisterDepthToColor = false;

	settings.useOniFile 	= false;
	settings.oniFilePath 	= "UNDEFINED";

	calculateActivity = ofToBool(XML.getValue("settings:calculateActivity", "false"));
	mirrorThreshold = ofToBool(XML.getValue("settings:mirror", "false"));
	findContours 	= ofToBool(XML.getValue("settings:findContours", "false"));
	drawThresholded = ofToBool(XML.getValue("settings:drawThresholded", "false"));
	drawColor 		= ofToBool(XML.getValue("settings:drawColor", "false"));
	useLowResValues	= ofToBool(XML.getValue("settings:useLowResValue", "false"));
	drawOni			= ofToBool(XML.getValue("settings:drawOni", "false"));
	enableRecorder	= ofToBool(XML.getValue("settings:enableRecorder", "false"));
	nearThreshold 	= XML.getValue("settings:nearThreshold",255);
	farThreshold  	= XML.getValue("settings:farThreshold",20);
	nearClip		= XML.getValue("settings:nearClip",50);
	farClip  		= XML.getValue("settings:farClip",1200);

	thresholdWithOpenCV = ofToBool(XML.getValue("settings:thresholdWithOpenCV", "false"));

	grayImage.allocate(settings.width,settings.height);
	grayThreshNear.allocate(settings.width, settings.height);
	grayThreshFar.allocate(settings.width, settings.height);

	isReady = oniGrabber.setup(settings);
	if (enableRecorder) {
		recorder.setup(&oniGrabber);
	}
	oniGrabber.depthSource.setDepthClipping(nearClip,farClip);

	binCount = 8;
	binWidth = floor(settings.width / binCount);
	bins.resize(binCount);
	emptyBins();
}

void ofApp::saveXMLSettings() {
	XML.setValue("settings:calculateActivity", calculateActivity?"true":"false");
	XML.setValue("settings:mirror", mirrorThreshold?"true":"false");
	XML.setValue("settings:findContours", findContours?"true":"false");
	XML.setValue("settings:drawThresholded", drawThresholded?"true":"false");
	XML.setValue("settings:drawColor", drawColor?"true":"false");
	XML.setValue("settings:useLowResValue", useLowResValues?"true":"false");
	XML.setValue("settings:drawOni", drawOni?"true":"false");
	XML.setValue("settings:enableRecorder", enableRecorder ?"true":"false");
	XML.setValue("settings:nearThreshold",nearThreshold);
	XML.setValue("settings:farThreshold",farThreshold);
	XML.setValue("settings:nearClip",nearClip);
	XML.setValue("settings:farClip",farClip);
	XML.saveFile();
}

//--------------------------------------------------------------
void ofApp::update() {
	if (isReady) {
		oniGrabber.update();
		grayImage.setFromPixels(oniGrabber.depthSource.noAlphaPixels->getPixels(), settings.width, settings.height);
		grayImage.mirror(false,mirrorThreshold);

		if (thresholdWithOpenCV) {
			grayThreshNear = grayImage;
			grayThreshFar  = grayImage;
			grayThreshNear.threshold(nearThreshold, true);
			grayThreshFar.threshold(farThreshold);
			cvAnd(grayThreshNear.getCvImage(), grayThreshFar.getCvImage(), grayImage.getCvImage(), NULL);
		}

		grayImage.flagImageChanged();

		if (findContours) {
			contourFinder.findContours(grayImage, 10, (settings.width*settings.height)/2, 3, false , true);
		}
	}

    if (calculateActivity) {
        for(uint i = 0; i < bins.size(); i++) {
            bins[i] = floor( grayImage.countNonZeroInRegion(i*binWidth,0,binWidth,settings.height) / binWidth);
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw() {
    ofSetColor(255,255,255);
    ofBackground(0,0,0);

	if (isReady) {
		if (drawOni) {
			oniGrabber.draw();
		}

		if (drawThresholded) {
			grayImage.draw(0,0);
		}

		if (settings.doColor && drawColor) {
			ofTexture& color = oniGrabber.getRGBTextureReference();
			color.draw(color.getWidth(), 0);
		}

		if (settings.doIr) {
			ofTexture& ir = oniGrabber.getIRTextureReference();
			ir.draw(settings.width + ir.getWidth(), 0);
		}

		if (findContours) {
			contourFinder.draw(0, 0, settings.width, settings.height);
		}

        if(calculateActivity) {
        	fbo.begin();
            for (uint i = 0; i < bins.size(); i++) {
                int bValue = useLowResValues? floor(bins[i]/8)*8 : bins[i];
                ofPushMatrix();
                ofNoFill();
                ofTranslate(i*binWidth,settings.height-bValue);
                ofSetColor(0,255,0);
                ofRect(0,0, binWidth, bValue );
                ofTranslate(0,bValue + 20);
                ofDrawBitmapString( ofToString( bValue * 16 ), 8, 0);
                ofPopMatrix();
            }
            fbo.end();
            ofEnableAlphaBlending();
            fbo.draw(0,0);
            ofDisableAlphaBlending();
        }
	}
	
	if (enableRecorder) {
		ofEnableAlphaBlending();
		ofColor textColor(ofColor::green);
		string recordingStatus = "Recording status: ";
		if (recorder.isRecording) {
			textColor = ofColor::red;
			recordingStatus+= "RECORDING";
		} else {
			recordingStatus+= "NOT RECORDING";
		}

		ofDrawBitmapStringHighlight(recordingStatus, 20, 420, ofColor(0, 0, 0, 128), textColor);
		ofDisableAlphaBlending();
	}

	stringstream reportStream;
	reportStream
	<< "set near threshold " << nearThreshold << " ( press: n N )" << endl
	<< "set far threshold " << farThreshold << "  ( press:  f F )" << endl
	<< "set near clipping " << nearClip << " ( press: + - )" << endl
	<< "set far clipping " << farClip << " ( press: < > )" << endl 
	<< " " << endl;
	
	if (settings.doColor) {
		reportStream << "draw Color (press r)"  << endl;
	}

	reportStream
	<< "find & draw Contours " << (findContours?"yes":"no")   << " ( press: c )" << endl
	<< "draw Threshold " << (drawThresholded?"yes":"no") << " ( press: t )" << endl
	<< "threshWithOpenCV " << thresholdWithOpenCV << " ( press: d )" << endl;
	
	ofDrawBitmapStringHighlight(ofToString(ofGetFrameRate()) + " fps", 20, 300, ofColor(0, 0, 0, 128), ofColor::yellow);
	ofDrawBitmapStringHighlight(reportStream.str(), 20, 320, ofColor(0, 0, 0, 128), ofColor::yellow);
}

//--------------------------------------------------------------
void ofApp::emptyBins() {
	for (uint i = 0; i < bins.size(); i++) {
  		bins[i] = 0; // set value using index
	}
}

//--------------------------------------------------------------
void ofApp::exit() {
	ofLogVerbose() << "\n EXITING, be patient - takes some time \n";
	
	if (enableRecorder) {
		ofLogVerbose() << "\n CLosing Recorder\n";
		recorder.close();
	}

	if (isReady) {
		oniGrabber.close();
	}
	
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key) {}
//--------------------------------------------------------------
void ofApp::keyReleased(int key) {}
//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {}
//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {}
//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {}
//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {}
//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {}
//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {}
//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {}
