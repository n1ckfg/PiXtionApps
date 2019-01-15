WORK IN PROGRESS

OpenNI2 wrapper for openFrameworks for the Mac/Raspberry Pi

Note:

OSX:

When using project generator with this addon, several project file changes must be made:

1. Remove all *.dylib entries from the search path.
2. Add the following to the "Run Script" build phase (just under the libmodex entry).

```
cp -R ../../../addons/ofxOpenNI2Grabber/libs/openni2/libs/osx/OpenNI2 "$TARGET_BUILD_DIR/$PRODUCT_NAME.app/Contents/MacOS/";
```