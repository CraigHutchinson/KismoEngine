/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\Win32Platform\Win32Window.h
 * Status: superseded by kismo-v0.3/Win32Platform/Win32Window.h (same name, content differs)
 */
#ifndef WIN32WINDOW_H
#define WIN32WINDOW_H

#include "Framework/Window.h"


//Define Window 2000 as the minimum windows version so we hav scroll wheel support & the extra buttons
#ifndef WINVER 
	#define _WIN32_WINNT 0x0500
	#define WINVER 0x0500
	#define _WIN32_WINDOWS 0x0500 // Change this to the appropriate value to target Windows Me or later.
	#define _WIN32_IE 0x0500
#endif

#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <windows.h>


/** \brief Contains Win32 implementations of Platform classes
*/
namespace win32
{
	/** \brief Window defies the base interface for a window in the operating system */
	class Win32Window : public frx::Window
	{
	public:
		
		Win32Window( const String& name, frx::Rect2s rect = frx::Rect2s( 0, 0, 400, 300 ) );

		virtual ~Win32Window();
		
		/** \brief Create the window if necessary
		*/
		virtual Bool create();

		/** \brief \copydoc frx::Window::update()
		*/
		virtual Bool update();

		/** \brief \copydoc frx::Window::setVisible()
		*/
		virtual Bool setVisible( Bool visible );
		
		/** \brief \copydoc frx::Window::setRect()
		*/		
		virtual Bool setRect( frx::Rect2s rect );	

		/* \copydoc frx::Window::getContextHandle()
		*/		
		virtual void* getContextHandle();

	private:

		//the handle needs to be set in the static win32 procedure so this set function has been added
		void setHWnd( HWND h ) 
		{ m_hWnd = h; }

		HWND hWnd() const 
		{ return m_hWnd; }
		
		/** \brief handle windows messages for this window
		\remarks in windows callback format!
		*/
		LRESULT win32Proc( UINT uMsg, WPARAM wParam, LPARAM lParam );

		/** \brief Window create callback
		*/
		static LRESULT CALLBACK staticWin32Process( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	private:

		WNDCLASSEX m_wc; ///< The window class structure	
		DWORD m_windowStyle; ///< Custom window style e.g. WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN
		HWND m_hWnd; ///< The window handle		

	};


} //END: win32

#endif
