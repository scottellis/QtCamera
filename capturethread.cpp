#include <QtGui>
#include "capturethread.h"
#include "qtcamera.h"


CaptureThread::CaptureThread(QtCamera *parent) 
	: QThread(), m_parent(parent)
{
    m_stopped = true;
}

CaptureThread::~CaptureThread()
{
	disconnectCamera();
}

bool CaptureThread::startCapture()
{
	if (!m_stopped)
		return false;

	if (!connectCamera(0))
		return false;

	m_stopped = false;

	start(QThread::TimeCriticalPriority);

	return true;
}

void CaptureThread::stopCapture()
{    
	m_stopMutex.lock();
    m_stopped = true;
	m_stopMutex.unlock();
}

void CaptureThread::run()
{
	Mat grabFrame;
	
    while (1) {
		if (m_stopped)
			break;

		m_vidcap >> grabFrame;	
		m_parent->setGrabFrame(&grabFrame);
    }
}

bool CaptureThread::connectCamera(int device)
{
	if (m_vidcap.isOpened())
		return true;

    if (!m_vidcap.open(device))
		return false;

	return true;
}

void CaptureThread::disconnectCamera()
{
    if (m_vidcap.isOpened())
		m_vidcap.release();
}

bool CaptureThread::isConnected()
{
    return m_vidcap.isOpened();
}
