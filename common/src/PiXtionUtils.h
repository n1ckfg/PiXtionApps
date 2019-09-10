#pragma once

#include "ofMain.h"

void imageToBuffer(ofImage _img, ofBuffer& _buffer, int _quality);
void pixelsToBuffer(ofPixels _pix, ofBuffer& _buffer, int _quality);
void fboToBuffer(ofFbo _fbo, ofBuffer& _buffer, int _quality);

string uniqueId(int len);
float rawDepthToMeters(int depthValue);

