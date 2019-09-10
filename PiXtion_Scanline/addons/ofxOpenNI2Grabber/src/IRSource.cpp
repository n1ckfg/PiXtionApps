/*
 *  IRSource.cpp
 *  OpenNI2AppExample
 *
 *  Created by jason van cleave on 3/2/13.
 *  Copyright 2013 jasonvancleave.com. All rights reserved.
 *
 */

#include "IRSource.h"

IRSource::IRSource()
{
	width = 0;
	height = 0;
	backPixels = NULL;
	currentPixels = NULL;
	doDoubleBuffering = true;
#ifdef TARGET_OPENGLES
	doDoubleBuffering = false;
#endif
	isOn = false;
}
bool IRSource::setup(DeviceController& deviceController)
{
	
	Status status = STATUS_OK;
	
	status = videoStream.create(deviceController.device, SENSOR_IR);
	if (status == STATUS_OK)
	{
		ofLogVerbose() << "Find IRSource videoStream PASS";
		status = videoStream.start();
		if (status == STATUS_OK)
		{
			ofLogVerbose() << "IRSource videoStream start PASS";
		}else 
		{
			
			ofLogError() << "IRSource videoStream start FAIL: " << OpenNI::getExtendedError();
			videoStream.destroy();
		}
	}else
	{
		ofLogError() << "Find IRSource videoStream FAIL: "<< OpenNI::getExtendedError();
		videoStream.destroy();
	}
	if (videoStream.isValid())
	{
		if(!deviceController.settings.useOniFile && !deviceController.isKinect)
		{
			const VideoMode* requestedMode = deviceController.findMode(SENSOR_IR);
			if (requestedMode != NULL) 
			{
				videoStream.setVideoMode(*requestedMode);
			}
			else{
				ofLogError() << "IRSource unsupported mode";
			}
		}
		allocateBuffers();
		status = videoStream.addNewFrameListener(this);
		if (status == STATUS_OK)
		{
			ofLogVerbose() << "IRSource videoStream addNewFrameListener PASS";
		}else
		{
			ofLogVerbose() << "IRSource videoStream addNewFrameListener FAIL" << OpenNI::getExtendedError() ;
		}
		
		isOn = true;
	}else 
	{
		ofLogError() << "IRSource is INVALID";
	}
	return isOn;
}

void IRSource::update()
{
	texture.loadData(*currentPixels);
}
void IRSource::draw()
{
	texture.loadData(*currentPixels);
	texture.draw(0, 0);
}
void IRSource::allocateBuffers()
{
	videoMode = videoStream.getVideoMode();
	width = videoMode.getResolutionX();
	height = videoMode.getResolutionY();
	
	pixels[0].allocate(width, height, OF_IMAGE_GRAYSCALE);
	pixels[1].allocate(width, height, OF_IMAGE_GRAYSCALE);
	currentPixels = &pixels[0];
	backPixels = &pixels[1];
	
	//texture.allocate(width, height, GL_LUMINANCE);
	texture.allocate(pixels[0]);
//	ofTextureData texData;
//	texData.width = width;
//	texData.height = height;
//	texData.glType = GL_LUMINANCE;
//	texData.glTypeInternal = GL_LUMINANCE16;
//	texData.pixelType = GL_UNSIGNED_SHORT;
//	texture.allocate(texData);
	
}
void IRSource::onNewFrame(VideoStream& stream)
{
//	ofLogVerbose() << "IRSource::onNewFrame -- " << backPixels->getPixels()[ 900 ];
	stream.readFrame(&videoFrameRef);
	unsigned short* pix = (unsigned short *)videoFrameRef.getData();	
	if (doDoubleBuffering)
	{
		unsigned char* bpx = backPixels->getPixels();
		for(int i = 0; i < width*height; i++){
			bpx[i] = pix[i]/4;
		}
		swap(backPixels, currentPixels);
	}else
	{
		unsigned char* cpx = currentPixels->getPixels();
		for(int i = 0; i < width*height; i++){
			cpx[i] = pix[i]/4;
		}
	}
}

void IRSource::close()
{
	ofLogVerbose() << "IRSource close";
	videoStream.stop();
	videoStream.removeNewFrameListener(this);
	videoStream.destroy();
}