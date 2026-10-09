/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\Win32Platform\Win32Window.cpp
 * Status: superseded by kismo-v0.3/Win32Platform/Win32Window.cpp (same name, content differs)
 */
#include "Win32Window.h"
#include "Framework/Log.h"
#include "Framework/Common.h"

frx::Window* frx::Window::create( const Char* name, frx::Rect2s rect )
{ return new win32::Win32Window( name, rect ); }

namespace win32
{
	LRESULT CALLBACK Win32Window::staticWin32Process(	
		HWND hWnd, // Handle For This Window
		UINT uMsg, // Message For This Window
		WPARAM wParam, // Additional Message Information
		LPARAM lParam) // Additional Message Information
	{	

		//Pointer to an instance for the microsoft window class
		Win32Window* pWindow = 0 ;
		
		if (uMsg == WM_NCCREATE) //Window creating event
		{
			//When we create the window we passed in a pointer to the Window class instance
			// Note: compiler warning about this line is wrong!
			pWindow = (Win32Window*) ((LPCREATESTRUCT) lParam)->lpCreateParams ;

#pragma warning( disable: 4311) //Disable pointer to long warning
			//Store this pointer in the windows user memory space
			SetWindowLong ( hWnd, GWL_USERDATA, (long) pWindow ) ;
#pragma warning( default: 4311 )

			if ( pWindow ) //Check if the parent instance was set correctly
			{
				//Store the handle to the window now so next windows procedure call succesds
				pWindow->setHWnd( hWnd );
			}
		}
		else
		{
#pragma warning( disable: 4312) //Disable pointer to long warning
			//Read the userdata to retrieve the pointer to the window clas sinstance again
			// Note: compiler warning about this line is wrong!
			pWindow = (Win32Window*) GetWindowLong(hWnd, GWL_USERDATA) ;
#pragma warning( default: 4312 )
		}	
	   
		if ( isValidPointer( pWindow ) ) 
		{
			//If the pointer is valid then call the actual window process	
			return pWindow->win32Proc( uMsg, wParam, lParam );
		}
		else 
		{
			//If not then use the default process for the window
			return DefWindowProc(hWnd, uMsg, wParam, lParam );
		}

	}

	Win32Window::Win32Window( const String& name,frx:: Rect2s rect ) :
		 Window( name, rect ),
		 m_hWnd( 0 )
	{
		m_wc.cbSize = sizeof( WNDCLASSEX );
		m_wc.cbClsExtra = 0;
		m_wc.cbWndExtra = 0;
		m_wc.hbrBackground = 0;//(HBRUSH)(COLOR_BACKGROUND);
		m_wc.hCursor = LoadCursor(0, IDC_ARROW);
		m_wc.hIcon = LoadIcon( 0, IDI_APPLICATION  );
		m_wc.hInstance = GetModuleHandle( NULL );
		m_wc.lpfnWndProc = staticWin32Process;
		m_wc.lpszClassName = _T("RAGE_WINDOW");
		m_wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		m_wc.hIconSm = 0;
		m_wc.lpszMenuName = 0;
		RegisterClassEx( &m_wc );
	}

	Win32Window::~Win32Window()
	{
		DestroyWindow( m_hWnd );
	}
	
	Bool Win32Window::create()
	{
		if ( m_hWnd != 0 )
			return true;
	
		DWORD dwExStyle = 0;//WS_EX_COMPOSITED;
		m_windowStyle = WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

		// Create the window
		m_hWnd = CreateWindowEx( dwExStyle,
			_T( "RAGE_WINDOW" ), 
			getName().c_str(), 
			m_windowStyle, 
			(Int32)getRect().left(), (Int32)getRect().top(), // Window Position
			(Int32)getRect().width(), (Int32)getRect().height(), //window width & height
			0, //parent window handle
			0,  m_wc.hInstance, 
			this ); //Pass a pointer to the instance of the window class on the windows construction


		if ( m_hWnd != 0  ) //Return if the window was created successfully
		{	
			logSuccess( _T( "The window '") << getName().c_str() << _T("' create OK") );

			return true;//return success
		}
		else
		{
			logError( _T( "The window '") << getName().c_str() << _T("' create FAILED") );
			return false; //Return failure
		}	

	}

	Bool Win32Window::update()
	{
		MSG msg; 
		while ( PeekMessage( &msg, 0, 0, 0, PM_NOREMOVE) )
		{ 
			//Get a message from the callback queue
			//Returns -1 = error, 0 = quit message, 1 = normal message
			int bRet = GetMessage(&msg, 0, 0, 0);

			switch ( bRet ) 
			{ 
			case -1: //Check if getMessage failed
				{
					logError( _T("Win32 Message queue failed!") );
					return false; 
				}
			case 0:			
				{
					logMessage( getName() << " recieved a WM_QUIT message." );
					return false;
				}
			default:
				{					
					bool forDialog = false;

					//Check that the message isn't for any modeless dialogs
					for( ContextArray::const_iterator ptr = m_childContexts.begin(); ptr != m_childContexts.end(); ++ptr )
					{
						HWND hDlg = (HWND)(*ptr);
						if( hDlg != 0 && IsDialogMessage( hDlg, &msg )  )
						{
							forDialog = true;
							break;
						}

					}

					//If the message is not for any dialogs then send it to the window handle
					if ( !forDialog )
					{
						TranslateMessage(&msg); 
						DispatchMessage(&msg); 
					}
				}
			}
		}
		return true;

	}

	Bool Win32Window::setVisible( Bool visible )
	{
		if ( !frx::isValidHandle( m_hWnd ) )
			if ( !create() )
				return false;
		
		return (m_visible = visible && (ShowWindow( m_hWnd, (visible? SW_SHOW : SW_HIDE ) ) == 0)); //Show the window
	}
	
	Bool Win32Window::setRect( frx::Rect2s rect )
	{
		m_rect = rect;

		MoveWindow( m_hWnd, 
				(Int32)getRect().left(), (Int32)getRect().top(), // Window Position
				(Int32)getRect().width(), (Int32)getRect().height(), true );

		return true;
	}	

	void* Win32Window::getContextHandle()
	{
		return (void*)m_hWnd;
	}
	LRESULT Win32Window::win32Proc( UINT uMsg, WPARAM wParam, LPARAM lParam )
	{
		switch( uMsg )
		{
			case WM_NULL:
				break;

			case WM_CREATE:
				return 0;

			case WM_ACTIVATE:
				//WA_INACTIVE     0
				//WA_ACTIVE       1
				//WA_CLICKACTIVE  2
				break;

			case WM_DESTROY:
				m_visible = false;
				return 0;

			case WM_SETFOCUS:
			case WM_KILLFOCUS:
			case WM_ENABLE:
			case WM_SETREDRAW:
			case WM_SETTEXT:
			case WM_GETTEXT:
			case WM_GETTEXTLENGTH:
				break;

			case WM_PAINT:
			{
				//if (!m_noValidate)
				//	ValidateRect( m_hWnd, NULL ); //Remove the window rect from being updated
				//if ( m_isRender )
				{
					//ValidateRect( m_hWnd, NULL );
					//return 0;
				}

				break;
			}

			case WM_CLOSE:	//Window close message
			{
				//return 0;								// Jump Back
				break;
			}
			
			case WM_QUIT:
				m_visible = false;
				break;
			case WM_ERASEBKGND:
			case WM_SYSCOLORCHANGE:
			case WM_SHOWWINDOW:
				break;

			case WM_SETTINGCHANGE:
				break;

			case WM_DEVMODECHANGE:
			case WM_ACTIVATEAPP:
			case WM_FONTCHANGE:
			case WM_TIMECHANGE:
			case WM_CANCELMODE:
			case WM_SETCURSOR:
			case WM_MOUSEACTIVATE:
			case WM_CHILDACTIVATE:
			case WM_QUEUESYNC:
				break;

			case WM_GETMINMAXINFO:
				break;

			case WM_KEYDOWN: // Is A Key Being Held Down?
			{
				m_keyEvent.keyCode = (UInt8)wParam;
				m_keyEvent.pressed = true;
				m_keyEvent.trigger(); ///< send the key event
				return 0;
			}

			case WM_KEYUP: // Has A Key Been Released?
			{
				m_keyEvent.keyCode = (UInt8)wParam;
				m_keyEvent.pressed = false;
				m_keyEvent.trigger(); ///< send the key event
				return 0;
			}

			case WM_SYSCOMMAND: //Intercept System Commands
			{
				switch (wParam)	// Check System Calls
				{
					case SC_SCREENSAVE:	// Screensaver Trying To Start?
					case SC_MONITORPOWER:	// Monitor Trying To Enter Powersave?
					return 0; // Prevent From Happening
				}
				break;			
			}
			case WM_COMMAND: 
			{
				

				break;
			}

			case WM_SIZE: // Resize The OpenGL Window
			{
				if( SIZE_MINIMIZED == wParam ) //if window has been minimized
				{
					// pause while minimised
				}
				else
				{
					m_resizeEvent.size.width = LOWORD( lParam );
					m_resizeEvent.size.height = HIWORD( lParam );
					m_resizeEvent.trigger(); ///< Send the resize event
					m_rect.setSize( m_resizeEvent.size ); //Store the new size
				}
				//return 0;
				break;
			}

			case WM_MOVE:
				{					
					m_moveEvent.pos.x = LOWORD( lParam );
					m_moveEvent.pos.y = HIWORD( lParam );
					m_moveEvent.trigger(); ///< Send the resize event
					m_rect.setTopLeft( m_moveEvent.pos ); //Store the new size
					break;
				}

			case WM_MOUSEWHEEL:	//Scroll wheel message
			{          
				//GET_WHEEL_DELTA_WPARAM(wParam) );
				return 0;
			}

			// Trap mouse messages
			case WM_LBUTTONDOWN:
			case WM_LBUTTONUP:
			case WM_LBUTTONDBLCLK:
			case WM_RBUTTONUP:
			case WM_RBUTTONDOWN:
			case WM_RBUTTONDBLCLK:
			case WM_MBUTTONDOWN:
			case WM_MBUTTONUP:
			case WM_MBUTTONDBLCLK:
			case WM_XBUTTONDOWN:
			case WM_XBUTTONUP:
			case WM_XBUTTONDBLCLK:
			case WM_MOUSEMOVE:
			{
				if ( wParam )
					SetCapture( m_hWnd );
				else
					ReleaseCapture();

				m_mouseEvent.button[0] = (wParam & MK_LBUTTON) != 0;
				m_mouseEvent.button[1] = (wParam & MK_MBUTTON) != 0;
				m_mouseEvent.button[2] = (wParam & MK_RBUTTON) != 0;
				
				//Get the relative mouse motion
				if ( m_mouseEvent.absolutePos.x != -1 )
				{
					m_mouseEvent.relativePos.x = (short)LOWORD( lParam ) - m_mouseEvent.absolutePos.x;
					m_mouseEvent.relativePos.y = (short)HIWORD( lParam ) - m_mouseEvent.absolutePos.y;
				}
				
				m_mouseEvent.absolutePos.x = (short)LOWORD( lParam );
				m_mouseEvent.absolutePos.y = (short)HIWORD( lParam ) ;

				m_mouseEvent.trigger(); ///< send the mouse event

				return 0;
			}		

		}

		// Pass All Unhandled Messages To DefWindowProc
		return DefWindowProc( m_hWnd, uMsg, wParam, lParam );
	}

} //END: win32