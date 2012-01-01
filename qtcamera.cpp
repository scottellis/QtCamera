#include <QtGui>
#include "qtcamera.h"

#include "capturethread.h"
#include "camera.h"
#include "webcam.h"
#include "qglcanvas.h"

QtCamera::QtCamera(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);

	m_frameCount = 0;
	m_captureThread = NULL;
	m_frameRateTimer = 0;
	m_frameRefreshTimer = 0;
	m_camera = NULL;

	QWidget *centralWidget = new QWidget(this);
	QVBoxLayout *verticalLayout = new QVBoxLayout(centralWidget);
	verticalLayout->setSpacing(6);
	verticalLayout->setContentsMargins(0, 0, 0, 0);

#ifdef USING_OPENGL
	m_cameraView = new QGLCanvas(centralWidget);
#else
	m_cameraView = new QLabel(centralWidget);
#endif	
	
	QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(m_cameraView->sizePolicy().hasHeightForWidth());
    m_cameraView->setSizePolicy(sizePolicy);
    m_cameraView->setMinimumSize(QSize(320, 240));

#ifndef USING_OPENGL
    m_cameraView->setAlignment(Qt::AlignCenter);
#endif

    verticalLayout->addWidget(m_cameraView);

    setCentralWidget(centralWidget);


	connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(close()));
	connect(ui.actionStart, SIGNAL(triggered()), this, SLOT(startVideo()));
	connect(ui.actionStop, SIGNAL(triggered()), this, SLOT(stopVideo()));
	connect(ui.actionScale, SIGNAL(triggered()), this, SLOT(toggleScaling()));

	m_pStatus = new QLabel(this);
	m_pStatus->setAlignment(Qt::AlignCenter | Qt::AlignLeft);
	m_pStatus->setText("0.0 fps  ");
	ui.statusBar->addPermanentWidget(m_pStatus);

	ui.actionStop->setEnabled(false);
	ui.actionStart->setEnabled(true);
	m_scaling = ui.actionScale->isChecked();
}

QtCamera::~QtCamera()
{
	if (m_captureThread) {
		stopVideo();
		delete m_captureThread;
		m_captureThread = NULL;
	}

	if (m_camera) {
		delete m_camera;
		m_camera = NULL;
	}

	clearQueue();
}

void QtCamera::toggleScaling()
{
	m_scaling = ui.actionScale->isChecked();
}

bool QtCamera::createCamera()
{
	if (m_captureThread && m_captureThread->isRunning()) {
		stopVideo();
	}

	if (m_camera) {
		delete m_camera;
		m_camera = NULL;
	}

	m_camera = new WebCam();

	return (m_camera != NULL);
}

void QtCamera::startVideo()
{
	if (!m_camera) {
		if (!createCamera())
			return;
	}

	if (!m_camera->open(0)) {
		delete m_camera;
		m_camera = NULL;
		return;
	}

	clearQueue();

	if (!m_captureThread) {
		m_captureThread = new CaptureThread();

		if (!m_captureThread)
			return;
	}

	connect(m_captureThread, SIGNAL(newImage(Mat *)), this, SLOT(newImage(Mat *)), Qt::DirectConnection);

	if (m_captureThread->startCapture(m_camera)) {
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
		disconnect(m_captureThread, SIGNAL(newImage(Mat *)), this, SLOT(newImage(Mat *)));
		m_captureThread->stopCapture();			
	}

	if (m_frameRateTimer) {
		killTimer(m_frameRateTimer);
		m_frameRateTimer = 0;
	}

	if (m_frameRefreshTimer) {
		killTimer(m_frameRefreshTimer);
		m_frameRefreshTimer = 0;
	}

	ui.actionStop->setEnabled(false);
	ui.actionStart->setEnabled(true);
}

void QtCamera::newImage(Mat *grab)
{
	m_frameCount++;

	if (m_frameQMutex.tryLock()) {
		if (m_frameQ.empty())
			m_frameQ.enqueue(new Mat(*grab));

		m_frameQMutex.unlock();
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
		Mat *frame = NULL;

		m_frameQMutex.lock();

		if (!m_frameQ.empty())
			frame = m_frameQ.dequeue();

		m_frameQMutex.unlock();

		if (frame) {
			showImage(frame);
			delete frame;
		}
	}
}

void QtCamera::showImage(Mat *frame)
{	
	if (isMinimized()) 
		return;
	
	QImage img((const uchar*) frame->data, frame->cols, frame->rows, frame->step, QImage::Format_RGB888);		
	QImage swappedImg = img.rgbSwapped();

	if (m_scaling) {
		QImage scaledImg = swappedImg.scaled(m_cameraView->size(), Qt::KeepAspectRatioByExpanding);
#ifdef USING_OPENGL
		m_cameraView->setImage(scaledImg);
		m_cameraView->repaint();
#else
		m_cameraView->setPixmap(QPixmap::fromImage(scaledImg));
#endif
	}
	else {
#ifdef USING_OPENGL
		m_cameraView->setImage(swappedImg);		
		m_cameraView->repaint();
#else
		m_cameraView->setPixmap(QPixmap::fromImage(swappedImg));	
#endif
	}
}

void QtCamera::clearQueue()
{
	Mat *p;

	m_frameQMutex.lock();
	
	while (!m_frameQ.empty()) {
		p = m_frameQ.dequeue();
		delete p;
	}

	m_frameQMutex.unlock();
}
