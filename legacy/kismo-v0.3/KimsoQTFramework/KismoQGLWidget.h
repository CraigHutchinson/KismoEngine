/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\KimsoQTFramework\KismoQGLWidget.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef KISMOQGLWIDGET_H
#define KISMOQGLWIDGET_H

#include <QtOpenGL/QGL.h>
#include <QTGui/QEvent.h>
#include "Framework/Window.h"

class QRealRTGLWidget : public QGLWidget, public frx::IWindow
{
	Q_OBJECT

public:
	QRealRTGLWidget(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~QRealRTGLWidget();

public:

	/*virtual bool setup( const Options& options );
	virtual bool initialise( const realRT::ConfigMap& config );
	virtual bool open();*/

	virtual void destroy();
	/**
		\brief Main render/update loop for the window
	*/
	virtual bool update();
	virtual bool close();
	virtual void clear();
	virtual void doDisplay();
	
	///< TEMP: Tels glut to update the window
	virtual void present();

protected slots:

	void updateGL();

    void timerEvent();
protected:
	
	void initializeGL();
	void paintGL();

	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);


    virtual void resizeGL(int width, int height);

private:
	QTimer* m_updateTimer;
	QPointF m_mousePos;

};

#endif
