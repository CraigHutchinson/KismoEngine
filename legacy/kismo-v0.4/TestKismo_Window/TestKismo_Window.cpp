/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\TestKismo_Window\TestKismo_Window.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
// TestKismo_Window.cpp : Defines the entry point for the console application.
//

#include "KismoGFX/Window.h"
#include <crtdbg.h>//Include debug heap modifier to show memory leaks

using namespace kismo;

int _tmain(int argc, _TCHAR* argv[])
{	
	//// Leak checking on PC Platform in debug mode only - needs to be set at app entry
	DEBUG_ONLY( _CrtSetDbgFlag( _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_DELAY_FREE_MEM_DF ); )
	
	using gfx::Window;
	const Count kWindowCount = 5;
	Window window[] =
	{
		Window( Window::ContextSupport_OpenGL ),
		Window( Window::ContextSupport_OpenGL ),
		Window( Window::ContextSupport_OpenGL ),
		Window( Window::ContextSupport_OpenGL ),
		Window( Window::ContextSupport_OpenGL )
	};

	for ( int i = 0; i < kWindowCount; ++i )
	{
		window[i].setRect( Rect2s( i*50 + 50,  i*50 + 50, 300, 400 ) );
		window[i].open();
	}

	bool running = true;
	while ( running )
	{
		running = false;
		for ( int i = 0; i < kWindowCount; ++i )
			running |= window[i].update() && window[i].isActive();

	}

	return 0;
}

