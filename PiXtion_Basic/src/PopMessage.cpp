#include "PopMessage.h"

PopMessage::PopMessage() {
	willDraw 		= false;
	startTime 		= ofGetElapsedTimeMillis();
	endTime			= startTime + 1000;
	bTimerReached	= false;
	messageString	= ofToString("");

	verdana48.loadFont("verdana.ttf", 48, true, true);
	verdana48.setLineHeight(52.0f);
	verdana48.setLetterSpacing(1.037);
};

void PopMessage::update() {
	if (ofGetElapsedTimeMillis() >  endTime) {
		willDraw 		= false;
		bTimerReached 	= true;
	}
}

void PopMessage::draw() {
	if (willDraw) {
		ofEnableAlphaBlending();
		ofColor textColor(ofColor::red);
		ofSetColor(24,24,24);
		ofFill();
		ofRect(0, (ofGetHeight() - 80) *.5, ofGetWidth() ,80);
		ofNoFill();
		ofRectangle rect = verdana48.getStringBoundingBox(messageString,0,0);
		ofSetColor(255,255,255);
		verdana48.drawString(messageString, (ofGetWidth() - rect.width ) * .5,  ofGetHeight() * 0.5 + 22);
		ofDisableAlphaBlending();
	}
}

void PopMessage::showMessage(float duration, string message) {
	startTime 		= ofGetElapsedTimeMillis();
	endTime 		= startTime + duration;
	messageString 	= message;
	bTimerReached 	= false;
	willDraw 		= true;
}
