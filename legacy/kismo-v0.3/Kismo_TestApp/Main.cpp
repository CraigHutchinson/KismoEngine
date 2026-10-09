/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\Kismo_TestApp\Main.cpp
 * Status: superseded by kismo-v0.4/KismoAI/main.cpp (same name, content differs)
 */
#include "Core/Core.h"
#include "Framework/Window.h"
#include "Framework/Common.h"
#include "Framework/Profiler.h"
#include "Framework/Log.h"
#include "Framework/Plugin.h"
#include "Math/Vector.h"
#include "Framework/Timer.h"
#include "Framework/Thread.h"
#include "Gfx/WindowTarget.h"
#include "Gfx/Renderer.h"
#include "Gfx/MeshManager.h"
#include "Gfx/Camera.h"
#include "Phys/Physics.h"
#include "Gfx/GeometryQuery.h"

#include "CameraControllerAction.h"
#include "DeformerControllerAction.h"


#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <windows.h>
#include <crtdbg.h>//Include debug heap modifier to show memory leaks

INIT_PROFILER;

int _tmain()
{
	//// Leak checking on PC Platform in debug mode only - needs to be set at app entry
	DEBUG_ONLY( _CrtSetDbgFlag( _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_DELAY_FREE_MEM_DF ); )
	
	PROFILE;

	frx::GlobalLog globalLog( _T("RageGlobal"), frx::Log::Lvl_All , frx::Log::Lvl_Low ); 
	if ( !globalLog.isOpen() )
	{
		logCritical( "Failed to open the global log" );
		return -1;
	}

	//Put loggin on a seperate thread so deadlock elsewhere will still allow the logging to 
	// perform its required task of outputting messages to file and other parts of the program
	frx::Thread::Ptr logThread = frx::Thread::create( _T("logging"), frx::Thread::ThreadPriority_Idle );
	globalLog.attachToThread( logThread );
	logThread->startThread();

	core::Core core;
	if ( !core.initialise() )
	{
		logError( "Failed to intialise the core engine" );
		return -1;
	}

	// Load plugins
	frx::Plugin::Ptr glRenderPlugin( frx::Plugin::create( _T("OpenGLRenderer.dll") ) );
	if ( glRenderPlugin.isNull() || !glRenderPlugin->start() ) // Load and register the plugin
	{
		logCritical( "Render plugin failed to load! Cannot continue" );
		return -1;
	}

	frx::Plugin::Ptr glRageTranslatorPlugin( frx::Plugin::create( _T("KismoMeshTranslator.dll") ) );
	if ( glRageTranslatorPlugin.isNull() || !glRageTranslatorPlugin->start() ) // Load and register the plugin
	{
		logCritical( "Rage mesh translator failed to load! Cannot continue" );
		return -1;
	}

	_Core.directoryManager().addDirectoryMap( gfx::Mesh::TYPE, _T("../Models/") );
	_Core.directoryManager().addDirectoryMap( gfx::Scene::TYPE, _T("../Models/") );

	// Get the renderer
	gfx::Renderer* renderer = _Core.renderer;//_Core.getRenderer( _T("OpenGLRenderer") );
	
	//Create the app window
	frx::IWindow::Ptr win( frx::IWindow::create( _T("mooo" ) ) );
	win->setRect( math::Rect2s( 0,0,800,600 ) );

	// Create the render window
	gfx::IWindowTarget* renderWindow = (gfx::IWindowTarget*)renderer->createTarget( gfx::IWindowTarget::TYPE, _T("OpenGlRenderWindow") );
	renderWindow->setWindow( win );

	//-----------------------------------------
	// Create the scene graph
	//-----------------------------------------
	gfx::Scene::Ptr sceneRoot = _Core.sceneManager().create( _T("cube.rmf"), renderer ); 

	if ( sceneRoot.isNull() )
	{
		return -1;
	}

	gfx::Camera::Ptr mainCam( new gfx::Camera( _T("main_camera") ) );
	gfx::Camera::Ptr altCam(  new gfx::Camera( _T("alt_camera") ) );
	gfx::Viewport* viewport1 = renderWindow->addViewport( _T("main_viewport"), mainCam );
	//gfx::Viewport* viewport2 = renderWindow->addViewport( _T("main_alt"), altCam, 1, math::Rect2r( 0.75, 0.75, 1, 1 ) );
	//viewport2->setEnabled( false );

	//-----------------------------------------------
	//Initialise and run
	//-----------------------------------------------	
	renderer->initialise();// Initialise the renderer	
	renderer->setVSync( 0 ); //< Disable vertical sync so we get the maximum frame rate
	win->show();// Show the window

	mainCam->localTransform().setTranslate( math::Vector3( 0, 0, 0) );
	//mainCam->localTransform().rotateY( 87 );
	//mainCam->localTransform().rotateX( 28 );

	altCam->localTransform().setTranslate( math::Vector3( 0, 5, 35.0 ) );
	altCam->localTransform().rotateX( 15 );

	
	//Create out custom window to camera action controller
	CameraControllerAction cameraController( win, mainCam );

	gfx::Geometry* geometry = (gfx::Geometry*)frx::Object::findObject( _T("pCubeShape1") );
	DeformerControllerAction deformerController( win, mainCam, geometry );

	sceneRoot->addChild( mainCam );
	sceneRoot->addChild( altCam );

	//Render some frames
	Real tDelta = 0;
	Real tApp = 0;
	frx::FrameTimer frameTimer;
	
	sceneRoot->updateGeometryState( tDelta, tApp );
	while ( win->isVisible() )
	{
		//TODO: FrameListener stylce interface so these can occur automaticlly
		_Core.eventManager().update( tDelta, tApp );
		sceneRoot->updateGeometryState( tDelta, tApp );
		sceneRoot->updateRenderState();
		
		deformerController.update( tDelta, tApp );
		renderer->drawScene( sceneRoot );

		tDelta = frameTimer.endFrame();
		tApp = frameTimer.getFrameTime();
	}

	//Shut down thedeformer thread and remove all the deformers etc
	deformerController.terminate();

	core.terminate();

	STOP_PROFILE; //< Stop the current profile block
	END_PROFILER; //< Finish profiling
	return 0;
}
