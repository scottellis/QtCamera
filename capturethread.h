#ifndef CAPTURETHREAD_H
#define CAPTURETHREAD_H

#include <QThread>

#include "camera.h"

class CaptureThread : public QThread
{
    Q_OBJECT

public:
	bool startCapture(Camera *camera);
	void stopCapture();

signals:
	void newImage(Mat *grab);

protected:
    void run();

private:
	Camera *m_camera;
	volatile bool m_stop;
};

#endif // CAPTURETHREAD_H
