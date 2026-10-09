/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Plugins\Win32_GUIRenderer\Win32Widget.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
#include "Win32Widget.h"
#include "gui\KismoButton.h"
#include "gui\KismoRenderFrame.h"

namespace kismo
{
	namespace gui
	{	
		const k_String win32StyleName( Widget::ke_WidgetStyle style )
		{
			switch ( style )
			{
				case Widget::kc_WidgetStyle_Window: //The class for a window.
					return _T("WINDOW");
				case Widget::kc_WidgetStyle_RenderBox: //The class for a render target area/window.
					return _T("RENDERBOX");
				case Widget::kc_WidgetStyle_Button: //The class for a button. 
					return _T("BUTTON");
				case Widget::kc_WidgetStyle_ComboBox: //The class for a combo box. 
					return _T("COMBOBOX");
				case Widget::kc_WidgetStyle_Edit: //The class for an edit control.
					return _T("EDIT"); 
				case Widget::kc_WidgetStyle_ListBox: //The class for a list box. 
					return _T("LISTBOX");
				case Widget::kc_WidgetStyle_HScrollBar: //The class for a scroll bar. 
					return _T("HSCROLL");
				case Widget::kc_WidgetStyle_VScrollBar: //The class for a scroll bar. 
					return _T("VSCROLL");
				case Widget::kc_WidgetStyle_Label: //The class for a static control.
					return _T("LABEL");
				case Widget::kc_WidgetStyle_Frame: //The class for a static control.
					return _T("FRAME");					
				case Widget::kc_WidgetStyle_Group: //The class for a static control.
					return _T("GROUP");

				default:
					return _T("WINDOW");
			};
		}

		Win32Widget::Win32Widget( Win32_GUIRenderer* renderer, Widget* widget  ) :
			m_renderer( renderer ),
			m_impl( widget ),
			m_windowStyle(0 ),
			m_hWnd( 0 ),
			m_isRender( false )
		{
			assert( isValidPointer( m_impl ) );

		}

		Win32Widget::~Win32Widget()
		{
			DestroyWindow( m_hWnd );
		}

		//Create the widget and show it
		k_Bool Win32Widget::create()
		{
			if ( m_hWnd != 0 )
			{
				LOG_ERROR( _T("Win32 widget '") + m_impl->getDefinition().name + _T("' is already executed!") );
				return kc_FALSE;
			}

			m_windowStyle = 0;
			if ( isValidPointer( m_impl->getParent() ) )
				m_windowStyle |= WS_CHILDWINDOW;// | WS_OVERLAPPED;
			else
				m_windowStyle |= WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
			
			switch ( m_impl->getDefinition().style )
			{
				case Widget::kc_WidgetStyle_RenderBox: //The class for a window.
					if ( isValidPointer( m_impl->getParent() ) )	
					{
						m_windowStyle |= WS_BORDER | WS_CLIPSIBLINGS;
						break;
					}

				case Widget::kc_WidgetStyle_Window: //The class for a window.					
					m_windowStyle |= WS_SYSMENU | WS_CAPTION ;//| WS_CLIPSIBLINGS;
										
					if ( m_impl->childCount() )
						m_windowStyle |= WS_CLIPCHILDREN;

					break;

				default:

					break;
			}

			if ( m_impl->getDefinition().resizeable )
				m_windowStyle |= WS_THICKFRAME;




			//Get the widgets definition
			Widget::WidgetDefinition& def = m_impl->getDefinition();

			//Get the parent windo handle
			HWND parent = 0;
			Win32Widget* parentWidget = m_renderer->getParent( this );			
			if ( isValidPointer( parentWidget ) )
				parent = parentWidget->m_hWnd;

			k_String className( win32StyleName( def.style ) );


			DWORD dwExStyle = 0;//WS_EX_COMPOSITED;

			// Create the window
			m_hWnd = CreateWindowEx( dwExStyle,
				className.c_str(), 
				def.text.c_str(), 
				m_windowStyle, 
				(k_Int32)def.position.x, (k_Int32)def.position.y, // Window Position
				(k_Int32)def.size.x, (k_Int32)def.size.y, //window width & height
				parent, //parent window handle
				0, GetModuleHandle( NULL ), 
				this ); //Pass a pointer to the instance of the window class on the windows construction

			LOG_MESSAGE( _T(" CreateWindowEx Module - ") << GetModuleHandle( NULL ) );

			if ( m_hWnd != 0  ) //Return if the window was created successfully
			{	
				LOG_SUCCESS( className << _T(":") + def.name + _T(" Win32 widget created") );

				if ( def.visible )
					show();

				m_isRender = m_impl->isDerived( RenderFrame::TYPE );
				if ( m_isRender )
				{
					((RenderFrame*)m_impl)->setWindowHandle( &m_hWnd );
				}

				return true;//return success
			}
			else
			{
				LOG_ERROR( className << _T(":") + def.name + _T(" Win32 widget failed to create") );
				return false; //Return failure
			}			
		}

		k_Bool Win32Widget::update()
		{
			return kc_TRUE;
		}

		k_Bool Win32Widget::show()
		{
			//AnimateWindow( m_hWnd, 200, AW_CENTER );
			ShowWindow( m_hWnd, SW_SHOWNORMAL ); //Show the window
			return kc_TRUE;
		}

		k_Bool Win32Widget::hide()
		{
			ShowWindow( m_hWnd, SW_HIDE ); //Show the window
			return kc_TRUE;
		}

		k_Bool Win32Widget::setSize( const Vector2r& size )
		{
			assert( m_impl );
			Widget::WidgetDefinition& def = m_impl->getDefinition();
			def.size = size;
			MoveWindow( m_hWnd, 
				(int)def.position.x, (int)def.position.y, 
				(int)def.size.x, (int)def.size.y, true );
			return kc_TRUE;
		}

		k_Bool Win32Widget::setPosition( const Vector2r& position )
		{
			assert( m_impl );
			Widget::WidgetDefinition& def = m_impl->getDefinition();
			def.position = position;
			MoveWindow( m_hWnd, 
				(k_Int32)def.position.x, (k_Int32)def.position.y, 
				(k_Int32)def.size.x, (k_Int32)def.size.y, true );
			return kc_TRUE;
		}

		k_Bool Win32Widget::setEnabled( k_Bool enabled )
		{
			return kc_TRUE;
		}
		
		k_Bool Win32Widget::setText( const k_String& text )
		{
			SetWindowText( m_hWnd, text.c_str() );
			return kc_TRUE;
		}

				//Each widget has its own message handler
		LRESULT Win32Widget::win32Proc( UINT uMsg, WPARAM wParam, LPARAM lParam )
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
					PostQuitMessage( 0 );
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
						//ValidateRect( m_hWnd, NULL ); //Remove the window rect from being updated
					if ( m_isRender )
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

				case WM_KEYDOWN:							// Is A Key Being Held Down?
				{
					return 0;								// Jump Back
				}

				case WM_KEYUP:								// Has A Key Been Released?
				{
					return 0;								// Jump Back
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
					if ( (HWND)lParam != m_hWnd )
					{
						Win32Widget* pWin32Widget = m_renderer->getWidget( (HWND)lParam );//(Win32Widget*)GetWindowLong( (HWND)lParam, GWL_USERDATA);
						assert( isValidPointer( pWin32Widget ) );
						pWin32Widget->win32Proc( uMsg, wParam, lParam );												
					}
					else//Find the widget this command is for
					if ( isValidPointer( m_impl ) )
					{
						switch ( HIWORD(wParam) )
						{
						case BN_CLICKED:		m_impl->doEvent( Button::kc_Slot_clicked ); break;
						case BN_DOUBLECLICKED:	m_impl->doEvent( Button::kc_Slot_doubleClicked ); break;
						//case BN_DOUBLECLICKED:	m_impl->doEvent( pushed ); break;

						}
					}

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
						//( LOWORD( lParam ), HIWORD( lParam ));
					}
					return 0;
				}

				case WM_MOVE:
					break;

				case WM_MOUSEWHEEL:	//Scroll wheel message
				{          
					//GET_WHEEL_DELTA_WPARAM(wParam) );
					return 0;
				}

				// Trap mouse messages
				case WM_LBUTTONDOWN:


					break;
				case WM_LBUTTONUP:
				case WM_LBUTTONDBLCLK:
				{
					return 0;
				}
				case WM_RBUTTONUP:
				case WM_RBUTTONDOWN:
				case WM_RBUTTONDBLCLK:
				{
					return 0;
				}
				case WM_MBUTTONDOWN:
				case WM_MBUTTONUP:
				case WM_MBUTTONDBLCLK:
				{
					return 0;
				}
				case WM_XBUTTONDOWN:
				case WM_XBUTTONUP:
				case WM_XBUTTONDBLCLK:
				{
					return 0;
				}
				case WM_MOUSEMOVE:
				{
					//LOWORD( lParam ), HIWORD( lParam );
					return 0;
				}		

			}

			// Pass All Unhandled Messages To DefWindowProc
			return DefWindowProc(m_hWnd,uMsg,wParam,lParam);
		}
	}
}