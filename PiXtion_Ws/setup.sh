# https://github.com/Azure/azure-iot-sdk-c/issues/265

DIR=$PWD

cd ../../../addons
git clone https://github.com/n1ckfg/ofxHTTP
git clone https://github.com/n1ckfg/ofxIO
git clone https://github.com/n1ckfg/ofxMediaType
git clone https://github.com/n1ckfg/ofxNetworkUtils
git clone https://github.com/n1ckfg/ofxSSLManager
git clone https://github.com/n1ckfg/ofxOMXCamera
git clone https://github.com/n1ckfg/ofxJSON
git clone https://github.com/n1ckfg/ofxCv

cd $DIR
