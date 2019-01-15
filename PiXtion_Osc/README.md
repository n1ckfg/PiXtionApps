@author : Patrick Gutlich
@website: www.gepatto.nl
( 2015 )

DISPLAYING PIXEL ACTIVITY IN VERTICAL REGIONS CAPTURED BY AN ASUS XTION LIVE
https://forum.openframeworks.cc/t/xtion-openframeworks-raspberry-pi/20483/31

***IMPORTANT***
copy the file 55-primesense-usb.rules to /etc/udev/rules.d/ and restart

If you have a newer xtion pro live the product number is bumped up to 0601
edit the udev rule ( /etc/udev/rules.d/55-primesense-usb.rules ) and change line

ATTRS{idProduct}=="0600",ATTRS{idVendor}=="1d27",MODE="666",GROUP="video"

to

ATTRS{idProduct}=="0601",ATTRS{idVendor}=="1d27",MODE="666",GROUP="video"

and reboot your pi
then try make run again in the xtionSimple dir
