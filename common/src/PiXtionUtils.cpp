#include "PiXtionUtils.h"

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