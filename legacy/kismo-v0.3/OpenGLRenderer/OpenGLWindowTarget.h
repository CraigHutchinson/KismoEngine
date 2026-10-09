/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\OpenGLRenderer\OpenGLWindowTarget.h
 * Status: current (newest copy of this file name in the archive)
 */
#ifndef OPENGL_WINDOWTARGET_H
#define OPENGL_WINDOWTARGET_H

#include "Framework/Window.h"
#include "Gfx/WindowTarget.h"

namespace gfx
{
	/** \brief OpenGL IWindow render target implementation
	*/
	class OpenGLWindowTarget : public gfx::IWindowTarget
	{
	public:
		OpenGLWindowTarget( const String& name, Priority priority = 0  ) :
			gfx::IWindowTarget( name, priority )
		{}

		virtual Bool update();

	};


} //END: gfx

#endif