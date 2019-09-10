#pragma once

#include "ofMain.h"

namespace PiXtionUtils {

	//void imageToBuffer(ofImage _img, ofBuffer& _buffer, int _quality);
	void imageToBuffer(ofImage _img, ofBuffer& _buffer, int _quality) {
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

	void pixelsToBuffer(ofPixels _pix, ofBuffer& _buffer, int _quality);
	void fboToBuffer(ofFbo _fbo, ofBuffer& _buffer, int _quality);

	string uniqueId(int len);
	float rawDepthToMeters(int depthValue);

}
