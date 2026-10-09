/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Plugins\OGL_Renderer\OGL_RenderWindow.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef _KISMO_OGL_RENDERWINDOW_H
#define _KISMO_OGL_RENDERWINDOW_H

#include "gfx\KismoRenderWindow.h"
#include "framework/KismoTimer.h"

namespace kismo
{
	namespace gfx
	{

		class OGL_RenderWindow : public RenderWindow
		{
		public:
			OGL_RenderWindow( const k_String& name );
			
			/** \brief Sets the handle for the window that this render target uses */
			void setWindowHandle( void* windowHandle );
			/** \brief Creates the render context */
			bool createContext();

			virtual void update();

			
			virtual void drawText( k_Float32 x, k_Float32 y, const k_String8& text ); //TEMP: Draw text

		private:


		void buildFont( k_UInt32 fontHeight );
		void killFont();

		UINT m_fontListID;
		int m_fontHeight;



			bool setupPixelFormat();

		private:

			frx::Timer m_timer;

			HWND m_hWnd; ///< The window handle
			
			HDC m_hDC; ///< A handle to the device context
			HGLRC m_hRC; ///< A handle to the renering context
		};
	
	} //END: gfx

} //END: kismo

#endif