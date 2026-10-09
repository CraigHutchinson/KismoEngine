/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\KismoCore\_TEST_\TestGL\TestGL.cpp
 * Status: unique (no other copy of this file name in the archive)
 */


#include "KismoGL/Context.h"

using kismo::gl::Context;
using kismo::gl::IWindow;
using kismo::gl::VBO;

int _tmain(int argc, _TCHAR* argv[])
{
	/* Create default context
		- creates an internal window
	*/
	Context context( 0 ); 

	IWindow* window = context.window();

	VBO vbo = context.createVBO();

	while ( window->update() )
	{


		context.present();
	}

	return 0;
}

