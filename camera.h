#ifndef CAMERA_H
#define CAMERA_H

#include <QThread>
#include <QSize>

#include <opencv2/opencv.hpp>
using namespace cv;

class Camera : protected QThread
{
public:
	Camera() {}
	virtual ~Camera() {}

	virtual bool open(int device) = 0;
	virtual void close() = 0;
	virtual bool isOpen() = 0;
	virtual bool startCapture() { return true; }
	virtual bool stopCapture() { return true; }
	virtual bool getNextFrame(Mat *grab) = 0;

	// should be available after open()
	virtual QSize getImageSize() = 0;
};

#endif // CAMERA_H
