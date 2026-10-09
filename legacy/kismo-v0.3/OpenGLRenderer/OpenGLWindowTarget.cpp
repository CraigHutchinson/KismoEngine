/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\OpenGLRenderer\OpenGLWindowTarget.cpp
 * Status: current (newest copy of this file name in the archive)
 */
#include "OpenGLWindowTarget.h"

namespace gfx
{

	Bool OpenGLWindowTarget::update()
	{
		getWindow()->update();
		return true;
	}



} //END: gfx