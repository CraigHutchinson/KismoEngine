/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\OpenGLRenderer\OpenGLWindowTarget.cpp
 * Status: duplicate of kismo-v0.3/OpenGLRenderer/OpenGLWindowTarget.cpp
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