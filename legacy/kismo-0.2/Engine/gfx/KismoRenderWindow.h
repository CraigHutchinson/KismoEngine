/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\gfx\KismoRenderWindow.h
 * Status: unique (no other copy of this file name in the archive)
 */
// is a render target but creates a window etc...
#ifndef _KISMO_RENDER_WINDOW_H
#define _KISMO_RENDER_WINDOW_H

#include "gfx\KismoRenderTarget.h"

namespace kismo
{
	namespace gfx
	{
		//forward declaration
		class Renderer;

		//-----------------------------------------------------
		// RenderTarget type
		//-----------------------------------------------------
		class _KismoExport RenderWindow : public RenderTarget
		{
		public:
			RenderWindow( const k_String& name ) : RenderTarget( name ) {}

			void writeContentsToFile(const kismo::k_String& filename )
			{
			}

			/** \brief Sets the handle for the window that this render target uses */
			virtual void setWindowHandle( void* windowHandle ) = 0;
			/** \brief Creates the render context */
			virtual bool createContext() = 0;
		
			virtual void drawText( k_Float32 x, k_Float32 y, const k_String8& text ) = 0; //TEMP: Draw text

		private:

		};

	} // END: gfx

} // END: kismo

#endif