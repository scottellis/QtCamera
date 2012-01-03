#ifndef QTCAMERA_H
#define QTCAMERA_H

#include <QtGui>
#include "ui_qtcamera.h"

#include <opencv2/opencv.hpp>
using namespace cv;

class Camera;
class CaptureThread;

class QtCamera : public QMainWindow
{
	Q_OBJECT

public:
	QtCamera(QWidget *parent = 0, Qt::WFlags flags = 0);
	~QtCamera();
	
public slots:
	void startVideo();
	void stopVideo();
	void toggleScaling();
	void newImage(Mat *grab);
		
protected:
	void timerEvent(QTimerEvent *event);

private:
	void showImage(Mat *frame);
	bool createCamera();
	void clearQueue();

	Ui::QtCameraClass ui;
	CaptureThread *m_captureThread;
	Camera *m_camera;
	QMutex m_frameQMutex;
	QQueue <Mat *> m_frameQ;
	int m_frameRateTimer;
	int m_frameRefreshTimer;
	QLabel *m_pStatus;
	int m_frameCount;
	bool m_scaling;
	QLabel *m_cameraView;
};

#endif // QTCAMERA_H
