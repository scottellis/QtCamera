#ifndef CAPTURETHREAD_H
#define CAPTURETHREAD_H

#include <QThread>
#include <QtGui>

#include <opencv2/opencv.hpp>
using namespace cv;

class QtCamera;

class CaptureThread : public QThread
{
    Q_OBJECT

public:
    CaptureThread(QtCamera *parent);
	virtual ~CaptureThread();

	bool connectCamera(int device);
    void disconnectCamera();
	bool isConnected();

	bool startCapture();
    void stopCapture();
    bool isStopped() { return m_stopped; }

private:
	QtCamera *m_parent;
    VideoCapture m_vidcap;
    QMutex m_stopMutex;
    volatile bool m_stopped;

protected:
    void run();
};

#endif // CAPTURETHREAD_H
