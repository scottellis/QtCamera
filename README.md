  QtCamera
=======

A simple test framework for my first experiment with Qt beyond HelloWorld.

The QtCamera.sln is a VisualStudio 2010 solution.

The QtCamera.pro is for use with QtCreator, though I only tested with
QtCreator on Linux.

The project uses Qt 4.8 built from source on Windows 7 32-bit and uses 
Qt 4.7.1 on an Ubuntu 11.04 64-bit from the standard apt-get sources.

The project uses OpenCV 2.3.1 built from source for both systems.

You can use the debugger to step through both OpenCV and Qt code which is 
kind of cool. I'll write up some instructions for setting up the dev machines.


I tested with two Logitech USB web cameras, a C510 and C910. Both cameras
are always running at 30 fps.

CPU usage goes up to around 22% on Win7 displaying the video fullscreen on 
a 1680x1050 display which seems excessive. A reduced display drops the load 
considerably, under 2% for a 320x240 display. 

I got similar results on Linux. Displaying fullscreen at 1920x1280 the load
is around 20% on a 6-core machine. Dropping to 320x240 brings the load under
2%.

I don't really see a way to improve the OpenCV UVC capture code on Windows.

The V4L2 capture on Linux might have a chance for some optimization. I'm not
sure whether it's pulling MJPG or YUV images from the camera. I think I might
try using the V4L2 interface directly for capture on Linux instead of OpenCV.


The [gldraw] branch was a quick attempt at faster image drawing using OpenGL, 
but it didn't result in any improvement. I only tested this on Windows. It
needs to be looked at further. I've never tried anything with OpenGL with Qt
or OpenGL alone. I just copied some code I found on the interwebs. I may be 
doing it completely wrong ;-) 

