@author : Patrick Gutlich
@website: www.gepatto.nl
( 2015 )

DISPLAYING PIXEL ACTIVITY IN VERTICAL REGIONS CAPTURED BY AN ASUS XTION LIVE
https://forum.openframeworks.cc/t/xtion-openframeworks-raspberry-pi/20483/31

***64-BIT PI OS (aarch64)***
The bundled OpenNI2 libs are 32-bit only, so on 64-bit the app links the system OpenNI2 instead.
It is usually already installed as a dependency of gstreamer1.0-plugins-bad; if not:

sudo apt install libopenni2-0

That package also installs udev rules for the Xtion (including 0601), so the udev steps below are only needed on 32-bit.

***IMPORTANT***
copy the file 55-primesense-usb.rules to /etc/udev/rules.d/ and restart

If you have a newer xtion pro live the product number is bumped up to 0601
edit the udev rule ( /etc/udev/rules.d/55-primesense-usb.rules ) and change line

ATTRS{idProduct}=="0600",ATTRS{idVendor}=="1d27",MODE="666",GROUP="video"

to

ATTRS{idProduct}=="0601",ATTRS{idVendor}=="1d27",MODE="666",GROUP="video"

and reboot your pi
then try make run again in the xtionSimple dir
