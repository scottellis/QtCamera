#include <QtGui>
#include "qtcamera.h"

QtCamera::QtCamera(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);

	m_frameCount = 0;
	m_captureThread = NULL;
	m_frameRateTimer = 0;

	connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(close()));
	connect(ui.actionStart, SIGNAL(triggered()), this, SLOT(startVideo()));
	connect(ui.actionStop, SIGNAL(triggered()), this, SLOT(stopVideo()));

	m_pStatus = new QLabel(this);
	m_pStatus->setAlignment(Qt::AlignCenter | Qt::AlignLeft);
	m_pStatus->setText("0.0 fps  ");
	ui.statusBar->addPermanentWidget(m_pStatus);

	ui.actionStop->setEnabled(false);
	ui.actionStart->setEnabled(true);
}

QtCamera::~QtCamera()
{
	if (m_captureThread) {
		stopVideo();
		m_captureThread->disconnectCamera();
		delete m_captureThread;
		m_captureThread = NULL;
	}
}

void QtCamera::startVideo()
{
	if (!m_captureThread) {
		m_captureThread = new CaptureThread(this);

		if (!m_captureThread)
			return;
	}

	if (m_captureThread->startCapture()) {
		m_frameCount = 0;
		m_frameRateTimer = startTimer(3000);
		m_frameRefreshTimer = startTimer(30);
		ui.actionStart->setEnabled(false);
		ui.actionStop->setEnabled(true);
	}
}

void QtCamera::stopVideo()
{
	if (m_captureThread) {
		m_captureThread->stopCapture();			
	}

	killTimer(m_frameRateTimer);
	killTimer(m_frameRefreshTimer);

	ui.actionStop->setEnabled(false);
	ui.actionStart->setEnabled(true);
}

void QtCamera::setGrabFrame(Mat *grabFrame)
{
	m_frameCount++;

	if (m_grabFrameMutex.tryLock()) {
		
		if (m_frameQueue.empty())
			m_frameQueue.enqueue(*grabFrame);

		m_grabFrameMutex.unlock();
	}
}

void QtCamera::timerEvent(QTimerEvent *event)
{
	if (event->timerId() == m_frameRateTimer) {
		QString fps;
		double count = m_frameCount;
		m_frameCount = 0;
		fps.sprintf("%0.1lf fps  ", count / 3.0);
		m_pStatus->setText(fps);		
	}
	else {
		m_grabFrameMutex.lock();
		if (m_frameQueue.empty()) {
			m_grabFrameMutex.unlock();
		}
		else {
			Mat frame = m_frameQueue.dequeue();
			m_grabFrameMutex.unlock();
			showImage(&frame);
		}
	}
}

void QtCamera::showImage(Mat *frame)
{	
	if (isMinimized()) 
		return;
	
	QImage img((const uchar*) frame->data, frame->cols, frame->rows, frame->step, QImage::Format_RGB888);		
	QImage swappedImg = img.rgbSwapped();
	ui.cameraView->setPixmap(QPixmap::fromImage(swappedImg));
}