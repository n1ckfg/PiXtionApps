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

    void vec3sToBuffer(vector<ofVec3f> _points, ofBuffer& _buffer) {
        float pointsData[_points.size() * 3]; 

        for (int i=0; i<_points.size(); i++) {
            int index = i * 3;
            pointsData[index] = points[i].x;
            pointsData[index+1] = points[i].y;
            pointsData[index+2] = points[i].z;
        }

        floatsToBuffer(pointsData, _buffer);
    }

    void vec2sToBuffer(vector<ofVec2f> _points, ofBuffer& _buffer) {
        float pointsData[_points.size() * 2]; 

        for (int i=0; i<_points.size(); i++) {
            int index = i * 2;
            pointsData[index] = points[i].x;
            pointsData[index+1] = points[i].y;
        }

        floatsToBuffer(pointsData, _buffer);
    }

    void floatsToBuffer(float _floats[], ofBuffer& _buffer) {
        char const * points = reinterpret_cast<char const *>(_floats);
        std::string pointsString(points, points + sizeof _floats);
        _buffer.set(pointsString); 
    }

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

    void pixelsToBuffer(ofPixels _pix, ofBuffer& _buffer, int _quality) {
        ofImage img;
        img.setFromPixels(_pix);
        imageToBuffer(img, _buffer, _quality);
    }

    void fboToBuffer(ofFbo _fbo, ofBuffer& _buffer, int _quality) {
        // jpegs have no alpha, so fbo must be initialized with GL_RGB, not GL_RGBA!
        ofPixels pixels;
        ofImage img;
        _fbo.readToPixels(pixels);
        img.setFromPixels(pixels);
        imageToBuffer(img, _buffer, _quality);
    }

    string uniqueId(int len) {
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

    float rawDepthToMeters(int depthValue) {
      if (depthValue < 2047) {
        return (float)(1.0 / ((double)(depthValue) * -0.0030711016 + 3.3309495161));
      }
      return 0.0;
    }

}