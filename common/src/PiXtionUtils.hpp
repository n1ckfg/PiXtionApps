#pragma once

#include "ofMain.h"


void PiXtionUtils::imageToBuffer(ofImage _img, ofBuffer& _buffer, int _quality) {
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

void PiXtionUtils::pixelsToBuffer(ofPixels _pix, ofBuffer& _buffer, int _quality) {
    ofImage img;
    img.setFromPixels(_pix);
    imageToBuffer(img, _buffer, _quality);
}

void PiXtionUtils::fboToBuffer(ofFbo _fbo, ofBuffer& _buffer, int _quality) {
    // jpegs have no alpha, so fbo must be initialized with GL_RGB, not GL_RGBA!
    ofPixels pixels;
    ofImage img;
    _fbo.readToPixels(pixels);
    img.setFromPixels(pixels);
    imageToBuffer(img, _buffer, _quality);
}

string PiXtionUtils::uniqueId(int len) {
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

float PiXtionUtils::rawDepthToMeters(int depthValue) {
  if (depthValue < 2047) {
    return (float)(1.0 / ((double)(depthValue) * -0.0030711016 + 3.3309495161));
  }
  return 0.0;
}


