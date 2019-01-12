#pragma once

#include "ofMain.h"

class PopMessage {

	private:
	    float endTime;
		float startTime;
		bool  bTimerReached; // used as a trigger when we hit the timer
		bool  willDraw;
		string messageString;
		ofTrueTypeFont verdana48;
		
	public:
	    PopMessage();

	    void showMessage(float duration,string message);
	    void update();  // update method, used to refresh your objects properties
	    void draw();    // draw method, this where you'll do the object's drawing

};

