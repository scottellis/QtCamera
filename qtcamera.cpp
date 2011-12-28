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
}

QtCamera::~QtCamera()
{
	if (m_captureThread) {
		stopVideo();
		m_captureThread->disconnectCamera();
		delete m_captureThread;
		m_captureThread = NULL;
	}

	killTimer(m_frameRateTimer);
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
		m_frameRateTimer = startTimer(5000);
	}
}

void QtCamera::stopVideo()
{
	if (m_captureThread) {
		m_captureThread->stopCapture();			
	}

	killTimer(m_frameRateTimer);
}

void QtCamera::setGrabFrame(Mat *grabFrame)
{
	//m_frameRateMutex.lock();
	m_frameCount++;
	//m_frameRateMutex.unlock();
}

void QtCamera::timerEvent(QTimerEvent *event)
{
	QString fps;
	double count;
		
	//m_frameRateMutex.lock();
	count = m_frameCount;
	m_frameCount = 0;
	//m_frameRateMutex.unlock();

	fps.sprintf("%0.1lf fps  ", count / 5.0);
	m_pStatus->setText(fps);		
}
