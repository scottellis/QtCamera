#ifndef QTCAMERA_H
#define QTCAMERA_H

#include <QtGui/QMainWindow>
#include "ui_qtcamera.h"
#include "capturethread.h"

#include <opencv2/opencv.hpp>
using namespace cv;

class QtCamera : public QMainWindow
{
	Q_OBJECT

public:
	QtCamera(QWidget *parent = 0, Qt::WFlags flags = 0);
	~QtCamera();

	void setGrabFrame(Mat *frame);
	
public slots:
		void startVideo();
		void stopVideo();
		
protected:
	void timerEvent(QTimerEvent *event);

private:
	void updateDisplay(Mat *frame);

	CaptureThread *m_captureThread;

	Ui::QtCameraClass ui;

	QMutex m_frameRateMutex;
	int m_frameRateTimer;
	QLabel *m_pStatus;
	int m_frameCount;
};

#endif // QTCAMERA_H
