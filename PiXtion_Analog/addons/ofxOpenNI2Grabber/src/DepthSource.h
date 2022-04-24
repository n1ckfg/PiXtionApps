#pragma once

#include "ofMain.h"
#include "OpenNI.h"
#include "DeviceController.h"

using namespace openni;

class DepthSource : public VideoStream::NewFrameListener
{
public:
	DepthSource();
	int width;
	int height;
	VideoStream videoStream;
    int nearClipping;
    int farClipping;

	ofTexture texture;
	ofPixels pixels[3];
	ofPixels* backPixels;
	ofPixels* currentPixels;
	ofPixels* noAlphaPixels;

	ofShortPixels rawPixels[2];
	ofShortPixels* backRawPixels;
	ofShortPixels* currentRawPixels;

	ofVec3f convertDepthToWorld(int depthX, int depthY);

	VideoMode videoMode;

	VideoFrameRef videoFrameRef;
	bool setup(DeviceController& deviceController);
	void allocateBuffers();
	void update();
	void draw();
	void close();
	int	deviceMaxDepth;
	void onNewFrame(VideoStream&);
	bool doDoubleBuffering;
	bool isOn;
	bool doRawDepth;
    void setDepthClipping(float nearClip=500, float farClip=4000);
};

