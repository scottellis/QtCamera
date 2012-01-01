#include "webcam.h"

WebCam::WebCam()
{
	m_device = 0;
}

WebCam::~WebCam()
{
	close();
}

bool WebCam::open()
{
	if (m_vidcap.isOpened())
		return true;

    if (!m_vidcap.open(m_device))
		return false;

	return true;
}

void WebCam::close()
{
	if (m_vidcap.isOpened())
		m_vidcap.release();
}

bool WebCam::isOpen()
{
	return m_vidcap.isOpened();
}

bool WebCam::getNextFrame(Mat *grab)
{
	if (!grab)
		return false;

	m_vidcap >> *grab;	

	return true;
}

// Always fixed using OpenCV interface to USB webcams
QSize WebCam::getImageSize()
{
	QSize sz(640, 480);

	return sz;
}