#ifndef QGLCANVAS_H
#define QGLCANVAS_H

#include <QtGui>
#include <QtOpenGL>
#include <QGLWidget>

class QGLCanvas : public QGLWidget
{
public:
	QGLCanvas(QWidget* parent = NULL);
	void setImage(const QImage& image);

protected:
	void paintEvent(QPaintEvent*);

private:
	QImage m_img;
};

#endif // QGLCANVAS_H
