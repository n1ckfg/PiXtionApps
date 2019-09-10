#include "PiXtionUtils.h"

string ofApp::uniqueId(int len) {
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