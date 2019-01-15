/*
 *  ofxOpenNI2Grabber.cpp
 *  OpenNI2AppExample
 *
 *  Created by jason van cleave on 2/28/13.
 *
 */

#include "ofxOpenNI2Grabber.h"


ofxOpenNI2Grabber::ofxOpenNI2Grabber()
{
	isReady = false;	
}

bool ofxOpenNI2Grabber::setup(ofxOpenNI2GrabberSettings settings_)
{
	settings = settings_;
	
	Status status = STATUS_OK;

	status = OpenNI::initialize();
	if (status == STATUS_OK)
	{
		ofLogVerbose() << "initialize PASS";
		deviceController.setup(settings);
	}else 
	{
		ofLog(OF_LOG_ERROR, "initialize FAIL:\n%s\n", OpenNI::getExtendedError());
	}
	
	if (status == STATUS_OK)
	{
		ofLogVerbose() << "Device open PASS";
	}else 
	{
		ofLogError() << "Device open FAIL: " << OpenNI::getExtendedError();
		deviceController.close();
		return false;
	}
	
	if (settings.doDepth) 
	{
		
		if (depthSource.setup(deviceController))
		{
			streams.push_back(&depthSource.videoStream);
		}else 
		{
			settings.doDepth = false;
		}
	}
	
	if (settings.doColor) 
	{
		if (rgbSource.setup(deviceController)) 
		{
			streams.push_back(&rgbSource.videoStream);
		}else 
		{
			settings.doColor = false;
		}
	}
	else if(settings.doIr){
		if(irSource.setup(deviceController))
		{
			streams.push_back(&irSource.videoStream);
		}
		else{
			settings.doIr = false;
		}
	}
	
	if(settings.doRegisterDepthToColor)
	{
		if (settings.doDepth && settings.doColor)
		{
			deviceController.registerDepthToColor();
		}
		
	}
	isReady = true;
	startThread(false, false);
	return isReady;
}

void ofxOpenNI2Grabber::threadedFunction()
{
	while (isThreadRunning()) 
	{
		int changedIndex;
		Status status = OpenNI::waitForAnyStream(&streams[0], streams.size(), &changedIndex);
		if (status != STATUS_OK)
		{
			ofLogError() << "waitForAnyStream FAIL:" << OpenNI::getExtendedError();
		}
	}
}


ofVec3f ofxOpenNI2Grabber::convertDepthToWorld(int depthX, int depthY){	
	ofVec3f result;
	CoordinateConverter::convertDepthToWorld(depthSource.videoStream, depthX, depthY,
											 (DepthPixel)(depthSource.currentRawPixels->getPixels()[depthY * depthSource.width + depthX]),
											 &result.x,&result.y,&result.z );
	return result;
}

void ofxOpenNI2Grabber::update()
{
	if (rgbSource.isOn) rgbSource.update();
	if (irSource.isOn) irSource.update();
	if (depthSource.isOn) depthSource.update();
}

void ofxOpenNI2Grabber::draw()
{
	//lock();
	if (rgbSource.isOn) rgbSource.draw();
	if (irSource.isOn) irSource.draw();
	if (depthSource.isOn) depthSource.draw();
}

ofPixels & ofxOpenNI2Grabber::getDepthPixels()
{
	Poco::ScopedLock<ofMutex> lock(mutex);
	return *depthSource.currentPixels;
}

ofShortPixels & ofxOpenNI2Grabber::getDepthRawPixels()
{
	Poco::ScopedLock<ofMutex> lock(mutex);
	
	if (!settings.doRawDepth)
	{
		ofLogError() << "settings.doRawPixels is FALSE";
		
	}
	return *depthSource.currentRawPixels;
}

ofPixels & ofxOpenNI2Grabber::getRGBPixels()
{
	Poco::ScopedLock<ofMutex> lock(mutex);
	return *rgbSource.currentPixels;
}

ofPixels & ofxOpenNI2Grabber::getIRPixels()
{
	Poco::ScopedLock<ofMutex> lock(mutex);
	return *irSource.currentPixels;
}

ofTexture & ofxOpenNI2Grabber::getDepthTextureReference()
{
	return depthSource.texture;
}

ofTexture & ofxOpenNI2Grabber::getRGBTextureReference()
{
	return rgbSource.texture;
}

ofTexture & ofxOpenNI2Grabber::getIRTextureReference()
{
	return irSource.texture;
}

bool ofxOpenNI2Grabber::close()
{
	ofLogVerbose() << "ofxOpenNI2Grabber::close";
	isReady = false;
	//stopThread();
	waitForThread(true);
    OpenNI::shutdown();
	if (depthSource.isOn) depthSource.close();
	if (rgbSource.isOn) rgbSource.close();
	if (irSource.isOn) irSource.close();
	deviceController.close();
	
	
	return isReady;
	//
}



