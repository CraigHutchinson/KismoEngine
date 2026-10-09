/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\gui\KismoRenderFrame.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef _KISMO_GUI_RENDERFRAME_H
#define	_KISMO_GUI_RENDERFRAME_H

#include "gui\KismoWindow.h"

namespace kismo
{
	//Forward declarations
	namespace gfx { class RenderWindow; class Renderer; }; 

	namespace gui
	{	
		class _KismoExport RenderFrame : public gui::Window
		{
			KISMO_DECLARE_RTTI;
		public:
			RenderFrame( Widget* parent = 0, gfx::Renderer* gfxRenderer = 0 );
			bool setGfxRenderer( gfx::Renderer* gfxRenderer );
			void create();

			
			void setWindowHandle( void* windowHandle ) { m_windowHandle = windowHandle; }
			void* getWindowHandle() { return m_windowHandle; }

		protected:

				
		private:	

			void* m_windowHandle; ///< The window handle
			gfx::Renderer* m_gfxRenderer; ///< The graphics renderer for this window
			gfx::RenderWindow* m_gfxRenderWindow; ///< The renderer window render target implimentation

		};

	} // END: gui

} // END: kismo

#endif
