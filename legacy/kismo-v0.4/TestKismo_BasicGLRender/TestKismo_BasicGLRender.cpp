/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\TestKismo_BasicGLRender\TestKismo_BasicGLRender.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
// TestKismo_Window.cpp : Defines the entry point for the console application.
//

#include "KismoGFX/Window.h"

using namespace kismo;

int _tmain(int argc, _TCHAR* argv[])
{
	using gfx::Window;
	Window window( Window::ContextSupport_OpenGL );
	window.open();

	//using gfx::Renderer;
	//Renderer* renderer = Renderer::create

	bool running = true;
	while (  window.update() && window.isActive() )
	{
	}

	return 0;
}

