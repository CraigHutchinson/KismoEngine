/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\OpenGLRenderer\OpenGLWindowTarget.h
 * Status: superseded by kismo-v0.3/OpenGLRenderer/OpenGLWindowTarget.h (same name, content differs)
 */
#ifndef OPENGL_WINDOWTARGET_H
#define OPENGL_WINDOWTARGET_H

#include "Framework/Window.h"
#include "Gfx/WindowTarget.h"

namespace gfx
{
	/** \brief OpenGL Window render target implementation
	*/
	class OpenGLWindowTarget : public gfx::WindowTarget
	{
	public:
		OpenGLWindowTarget( const String& name, Priority priority = 0  ) :
			gfx::WindowTarget( name, priority )
		{}

		virtual Bool update();

	};


} //END: gfx

#endif