#pragma once

#include "ofMain.h"

namespace PiXtionUtils {

    string createCompName(string compname) {
        ofFile file;
        ofBuffer buff;
        
        file.open(ofToDataPath("compname.txt"), ofFile::ReadWrite, false);
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
        
        return compname;
    }

    void imageToBuffer(ofImage& img, ofBuffer& buffer, int quality) {
        switch(quality) {
            case 5:
                ofSaveImage(img, buffer, OF_IMAGE_FORMAT_JPEG, OF_IMAGE_QUALITY_BEST);
                break;
            case 4:
                ofSaveImage(img, buffer, OF_IMAGE_FORMAT_JPEG, OF_IMAGE_QUALITY_HIGH);
                break;
            case 3:
                ofSaveImage(img, buffer, OF_IMAGE_FORMAT_JPEG, OF_IMAGE_QUALITY_MEDIUM);
                break;
            case 2:
                ofSaveImage(img, buffer, OF_IMAGE_FORMAT_JPEG, OF_IMAGE_QUALITY_LOW);
                break;
            case 1:
                ofSaveImage(img, buffer, OF_IMAGE_FORMAT_JPEG, OF_IMAGE_QUALITY_WORST);
                break;
        }
    }

    void pixelsToBuffer(ofPixels& pixels, ofBuffer& buffer, int quality) {
        ofImage img;
        img.setFromPixels(pixels);
        imageToBuffer(img, buffer, quality);
    }

    void fboToBuffer(ofFbo& fbo, ofBuffer& buffer, int quality) {
        // jpegs have no alpha, so fbo must be initialized with GL_RGB, not GL_RGBA!
        ofPixels pixels;
        ofImage img;
        fbo.readToPixels(pixels);
        img.setFromPixels(pixels);
        imageToBuffer(img, buffer, quality);
    }

    string uniqueId(int len) {
    	long seed = long(ofRandom(0, 1000000));
    	cout << seed << "   "; 
    	srand(seed);
    	string str = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    	string newstr;
    	int pos;
    	while((int)newstr.size() != len) {
    	   pos = ((rand() % (str.size() - 1)));
    	   newstr += str.substr(pos,1);
    	}

    	cout << newstr << "\n";
    	return newstr;
    }

    float rawDepthToMeters(int depthValue) {
      if (depthValue < 2047) {
        return (float)(1.0 / ((double)(depthValue) * -0.0030711016 + 3.3309495161));
      }
      return 0.0;
    }

}