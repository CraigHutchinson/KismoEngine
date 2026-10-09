/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\KismoCore\_SOURCE_\KismoGL\Window_Win32.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
/*^**************************************************************************
*	
*	Kismo Engine 
*	© 2008-2013 Craig Hutchinson (craig@craighutchinson.co.uk)
*	All right reserved
*
*	This file is subject to the terms and conditions defined in
*	file 'license.txt', which is part of this source code package.
*
***************************************************************************^*/


#include "Window.h"
#include "KismoFramework/Common.h"
#include "KismoFramework/String.h"

namespace kismo { namespace gl
{	
	static const Char* kWindowClass = _T("KismoWindow");

	Window::Window()
	{
		createWindow();
	}

	Window::~Window()
	{
		destroyWindow();
	}
		
	ContextHandle Window::getContext()
	{  return (ContextHandle)0; }


	bool Window::createWindowClass()
	{	
		// Fill in the window class structure with parameters 
		// that describe the main window.
		//http://msdn.microsoft.com/en-us/library/windows/desktop/ms633575(v=vs.85).aspx

		WNDCLASSEX wc;
		wc.cbSize		 = sizeof(WNDCLASSEX);
		wc.style		 = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wc.lpfnWndProc	 = staticProcessWin32Message;
		wc.cbClsExtra	 = 0;
		wc.cbWndExtra	 = 0;
		wc.hInstance	 = GetModuleHandle(0);
		wc.hIcon		 = LoadIcon(NULL, IDI_APPLICATION);
		wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = NULL;
		wc.lpszMenuName  = NULL;
		wc.lpszClassName = kWindowClass;
		wc.hIconSm		 = (HICON)LoadImage(  GetModuleHandle(0), // small class icon 
			IDI_APPLICATION,
			IMAGE_ICON, GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON),  LR_DEFAULTCOLOR); 

		if(!RegisterClassEx(&wc))
		{
			MessageBox(NULL, _T("Window Registration Failed!"), _T("Error!"), MB_ICONEXCLAMATION | MB_OK);
			return false;
		}

		return true;
	}


	void Window::destroyWindowClass()
	{
		if ( !UnregisterClass( _T("KismoWindow"), GetModuleHandle(0) ))			// Are We Able To Unregister Class
		{
			MessageBox(NULL,_T("Could Not Unregister Class."),_T("SHUTDOWN ERROR"),MB_OK | MB_ICONINFORMATION);
			return;
		}

		return;
	}

	bool Window::createWindow()
	{
		if ( !createWindowClass() )
			return Assert(false), false;
		
		String windowTitle;
		windowTitle += _T("Kismo Engine © 2008-");
		windowTitle.append( _T(__DATE__)+7, 4 ); //, Concatenate YEAR
		windowTitle += _T(" | All right reserved {Win32}");

		// Step 2: Creating the Window
		m_handle = CreateWindowEx(
			WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
			_T("KismoWindow"),
			windowTitle.c_str(),
			WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS |	WS_CLIPCHILDREN,				// Required Window Style								
			0, 0, 800, 600,
			NULL, NULL, GetModuleHandle(0), this );

		if( m_handle == NULL)
		{
			MessageBox(NULL, _T("Window Creation Failed!"), _T("Error!"), MB_ICONEXCLAMATION | MB_OK);
			return false;
		}
		
		int ret = ShowWindow( m_handle, SW_SHOWNORMAL );
		return true;
	}

	void Window::destroyWindow()
	{
		if (m_handle && !DestroyWindow(m_handle))					// Are We Able To Destroy The Window?
		{
			MessageBox(NULL,_T("Could Not Release m_handle."),_T("SHUTDOWN ERROR"),MB_OK | MB_ICONINFORMATION);
		}
		m_handle=NULL;

		destroyWindowClass();
	}
		
	
	
	bool Window::update()
	{
		MSG msg;
		Count msgLimit = 1024;
		while( m_handle 
			&& PeekMessage(&msg, m_handle, 0, 0, PM_REMOVE ) 
			&& --msgLimit )
		{
			if (msg.message==WM_QUIT)				// Have We Received A Quit Message?
			{
				//close();
				return false;
			}

			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		return true;
	}
			
	LRESULT CALLBACK Window::staticProcessWin32Message(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		Window* window = 0;

		if( msg == WM_NCCREATE )
		{
			// retrieve Window instance from window creation data and associate
			Window* instance = reinterpret_cast<Window*>(((LPCREATESTRUCT)lParam)->lpCreateParams);
			
			// save window handle
			instance->m_handle = hwnd;

			SetWindowLongPtr(hwnd, GWL_USERDATA, (LONG_PTR)instance );
		}
		else
		{
			// retrieve associated Window instance
			window = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWL_USERDATA));
		}

		if(window)
		{
			if ( window->processWin32Message( msg, wParam, lParam ) )
				return 0;
		}

		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	bool Window::processWin32Message( UInt32 msg, UInt32 wParam, UInt32 lParam)
	{
		switch(msg)
		{
			case WM_SIZE:			/* User resized video mode */
			{		
				//onResizeCallbackEx( LOWORD(lParam), HIWORD(lParam) );
				break;
			}

			case WM_SHOWWINDOW:
				//m_isActive = (wParam==TRUE);
				break;

			case WM_CREATE:
				//m_isActive = true;
				break;

			case WM_CLOSE:
				PostMessage( m_handle, WM_QUIT, 0, 0 );
				return true;

			case WM_DESTROY:
			//	m_isActive = false;
				return true;

			case WM_KEYDOWN:
			{
				//eKeyModifier modifiers = (eKeyModifier)0; 
				//onKeyboardCallback( true,  ms_keys.map[wParam], modifiers, 0, 0 );
				return true;
			}

			case WM_KEYUP:
			{
				//eKeyModifier modifiers = (eKeyModifier)0; 
				//onKeyboardCallback( false, ms_keys.map[wParam], modifiers, 0, 0 );
				return true;
			}
			
			case WM_SYSCOMMAND:							// Intercept System Commands
			{
				switch (wParam)							// Check System Calls
				{
					case SC_SCREENSAVE:					// Screensaver Trying To Start?
					case SC_MONITORPOWER:				// Monitor Trying To Enter Powersave?
					return 0;							// Prevent From Happening
				}
				break;									// Exit
			}
			case WM_LBUTTONDBLCLK:
			case WM_RBUTTONDBLCLK:
			case WM_MBUTTONDBLCLK:

			case WM_MOUSEWHEEL:
				break;

			case WM_MOUSEMOVE:			/* Mouse moved */
				{
					/*UInt32 buttonState = 
						(wParam & MK_LBUTTON ? Button_Left : 0) |
						(wParam & MK_RBUTTON ? Button_Right : 0) |
						(wParam & MK_MBUTTON ? Button_Middle : 0) |
						(wParam & MK_XBUTTON1 ? Button_X1 : 0) |
						(wParam & MK_XBUTTON2 ? Button_X2 : 0);

					Vector2i newMousePos( LOWORD(lParam), HIWORD(lParam) );
					if ( m_mousePos == Vector2i(Int32_Max,Int32_Max) ) //< When we first update we don't want a large relative motion
						m_mousePos = newMousePos;

					onMotionCallback( buttonState, newMousePos.x, newMousePos.y, newMousePos.x-m_mousePos.x, newMousePos.y-m_mousePos.y );
					m_mousePos = newMousePos;
					*/
					break;
				}

			case WM_LBUTTONDOWN: /* Mouse button pressed */
				//onMouseCallback( true, Button_Left, LOWORD(lParam), HIWORD(lParam) ); break;
			case WM_RBUTTONDOWN: /* Mouse button pressed */
				//onMouseCallback( true, Button_Right, LOWORD(lParam), HIWORD(lParam) ); break;
			case WM_MBUTTONDOWN: /* Mouse button pressed */
				//onMouseCallback( true, Button_Middle, LOWORD(lParam), HIWORD(lParam) ); break;

			case WM_LBUTTONUP: 	/* Mouse button released */
				//onMouseCallback( false, Button_Left, LOWORD(lParam), HIWORD(lParam) ); break;
			case WM_RBUTTONUP:/* Mouse button released */
				//onMouseCallback( false, Button_Right, LOWORD(lParam), HIWORD(lParam) ); break;
			case WM_MBUTTONUP:	/* Mouse button released */
				//onMouseCallback( false, Button_Middle, LOWORD(lParam), HIWORD(lParam) ); 
				break;
		}

		return false;
	}

} /*END: gl */ } /*END: kismo */
