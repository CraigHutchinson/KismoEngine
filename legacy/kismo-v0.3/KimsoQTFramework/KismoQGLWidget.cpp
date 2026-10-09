/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\KimsoQTFramework\KismoQGLWidget.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
#include "KismoQGLWidget.h"
#include <QTCore/QTimer>

QRealRTGLWidget::QRealRTGLWidget(QWidget *parent, Qt::WindowFlags flags) :
	QGLWidget( parent, 0, flags ),
	frx::IWindow( _T("") )
{
	setStatsName( "QLWidget Window" );
}

QRealRTGLWidget::~QRealRTGLWidget()
{}


bool QRealRTGLWidget::setup( const Options& options )
{ 
	m_size = options.size;
	return true;
}

bool QRealRTGLWidget::initialise( const realRT::ConfigMap& config )
{ 
	m_updateTimer = new QTimer(this);
	QObject::connect( m_updateTimer, SIGNAL(timeout()), this, SLOT(timerEvent()) );
	m_updateTimer->setInterval( 0 );
	m_updateTimer->start();

	//initializeGL(); //, This is called automatically 
	
	return IWindow::initialise( config );
}

bool QRealRTGLWidget::open()
{
	QGLWidget::show();
	return true;
}

void QRealRTGLWidget::destroy()
{
}

bool QRealRTGLWidget::update()
{
	//We don't call the display callback here and instead call the Idle callback - this allows the Core to decide whether it wishes to diaply a new image or not
	onIdleCallback();

	return true;
}

bool QRealRTGLWidget::close()
{
	return QGLWidget::close();
}

void QRealRTGLWidget::clear()
{
	// clear screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void QRealRTGLWidget::doDisplay()
{
	m_isActive = QGLWidget::isValid() && QGLWidget::isVisible();
	if ( m_isActive )
		onDisplayCallback();
} 

void QRealRTGLWidget::present()
{
	//glClearColor( 1,1,1,1);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();									// Reset The Current Modelview Matrix
	//glTranslatef(0.0f,0.0f,-1.0f);						// Move One Unit Into The Screen
	
	// Pulsing Colors Based On Text Position		
	glColor3f( 0.75f, 0.0f, 0.0f );
	
	// Position The Text On The Screen
	//glRasterPos2f( -(float)(m_size.w - 12) / m_size.w, -(float)(m_size.h - 12) / m_size.h );
	realRT::Char buff[256];
	sprintf_s( buff, 256, "fps = %f, WxH = %ux%u", m_timer.getFPS(), m_size.w, m_size.h );	// Print GL Text To The Screen	
	glColor3f( 0.0f, 0.0f, 0.0f );
	renderText( 10,10,buff );
	glColor3f( 1.0f, 1.0f, 1.0f );
	renderText( 11,11,buff );

	// swap buffers to display, since we're double buffered.		
	QGLWidget::swapBuffers();
	
	m_timer.startFrame();
}

void QRealRTGLWidget::updateGL()
{
	QGLWidget::updateGL(); 
}

void QRealRTGLWidget::timerEvent()
{ 
	update();
}

void QRealRTGLWidget::initializeGL()
{
	//qglClearColor(trolltechPurple.dark());
	//object = makeObject();
	glShadeModel(GL_FLAT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}

void QRealRTGLWidget::paintGL()
{
	update();
}

void QRealRTGLWidget::keyPressEvent(QKeyEvent *event)
{
	//lastPos = event->pos();
	onKeyboardCallback( event->key(), m_mousePos.x(), m_mousePos.y()  );
	QGLWidget::keyPressEvent(event);
}

void QRealRTGLWidget::mousePressEvent(QMouseEvent *event)
{
	//lastPos = event->pos();
	m_mousePos = event->posF();
	QGLWidget::mousePressEvent(event);
}

void QRealRTGLWidget::mouseMoveEvent(QMouseEvent *event)
{
	//m_mousePos = event->posF();

	QPointF rel = event->posF() - m_mousePos;
	/*int dx = event->x() - lastPos.x()();
	int dy = event->y() - lastPos.y()();

	if (event->buttons() & Qt::LeftButton) {
		setXRotation(xRot + 8 * dy);
		setYRotation(yRot + 8 * dx);
	} else if (event->buttons() & Qt::RightButton) {
		setXRotation(xRot + 8 * dy);
		setZRotation(zRot + 8 * dx);
	}
	lastPos = event->pos();*/
	onMotionCallback( event->buttons(),event->x(), event->y(), rel.x(), rel.y() );
		
	m_mousePos = event->posF();

	QGLWidget::mouseMoveEvent(event);
}

void QRealRTGLWidget::resizeGL(int width, int height)
{		
	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	glViewport( 0, 0, width - 1, height - 1 );	

	// Calculate The Aspect Ratio Of The IWindow
	//gluOrtho2D( 0, width - 1, 0, height - 1 );//45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix

	IWindow::onResizeCallback( width, height  );		
	
}