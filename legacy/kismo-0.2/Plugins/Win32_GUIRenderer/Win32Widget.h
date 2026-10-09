/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Plugins\Win32_GUIRenderer\Win32Widget.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef _KISMO_GUI_WIN32WIDGET_H
#define	_KISMO_GUI_WIN32WIDGET_H

#include "Win32_GUIRenderer.h"

#include "gui\KismoWidget.h"

namespace kismo
{
	namespace gui
	{	
		class Win32Widget
		{
		public:
			Win32Widget( Win32_GUIRenderer* renderer, Widget* widget);
			~Win32Widget();

			virtual k_Bool create();
			virtual k_Bool update();
			virtual k_Bool show();
			virtual k_Bool hide();
			virtual k_Bool setSize( const Vector2r& size );
			virtual k_Bool setPosition( const Vector2r& position );
			virtual k_Bool setEnabled( k_Bool enabled );
			virtual k_Bool setText( const k_String& text );

			//Get the implimentation widget
			Widget* impl() { return m_impl; }

			Win32_GUIRenderer* renderer() { return m_renderer; }

			//the handle needs to be set in the static win32 procedure so this set function has been added
			void setHWnd( HWND h ) { m_hWnd = h; }
			HWND hWnd() const { return m_hWnd; }

			//Each widget has its own message handler
			virtual LRESULT win32Proc( UINT uMsg, // Message For This Window
								WPARAM wParam, // Additional Message Information
								LPARAM lParam );

		protected:

			Win32_GUIRenderer* m_renderer; //pointer to the gui renderer
			
			Widget* m_impl; //Pointer to the implimentation widget

			HWND m_hWnd;//The window handle
			bool m_isRender; //Is the window a render target and shouldn't re-paint
			DWORD m_windowStyle; //Custom window style e.g. WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN

		};

	} // END: gui

} // END: kismo

#endif
