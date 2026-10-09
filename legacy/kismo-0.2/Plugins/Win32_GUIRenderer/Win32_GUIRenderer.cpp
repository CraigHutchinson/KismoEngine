/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Plugins\Win32_GUIRenderer\Win32_GUIRenderer.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
#include "Win32_GUIRenderer.h"

#include "Win32Widget.h"
namespace kismo
{
	namespace gui
	{	

				
		LRESULT CALLBACK staticWin32Process(	
			HWND hWnd, // Handle For This Window
			UINT uMsg, // Message For This Window
			WPARAM wParam, // Additional Message Information
			LPARAM lParam) // Additional Message Information
		{	

			//Pointer to an instance for the microsoft window class
			Win32Widget* pWin32Widget = 0 ;
			
			if (uMsg == WM_NCCREATE) //Window creating event
			{
				//When we create the window we passed in a pointer to the Window class instance
				// Note: compiler warning about this line is wrong!
				pWin32Widget = (Win32Widget*) ((LPCREATESTRUCT) lParam)->lpCreateParams ;

				//Store this pointer in the windows user memory space
				SetWindowLong ( hWnd, GWL_USERDATA, (long) pWin32Widget ) ;

				if ( pWin32Widget ) //Check if the parent instance was set correctly
				{
					//Store the handle to the window now so next windows procedure call succesds
					pWin32Widget->setHWnd( hWnd );
				}
			}
			else
			{
				//Read the userdata to retrieve the pointer to the window clas sinstance again
				// Note: compiler warning about this line is wrong!
				pWin32Widget = (Win32Widget*) GetWindowLong(hWnd, GWL_USERDATA) ;
			}	
		   
			if ( isValidPointer( pWin32Widget ) ) 
			{
				//If the pointer is valid then call the actual window process	
				return pWin32Widget->win32Proc( uMsg, wParam, lParam );
			}
			else 
			{
				//If not then use the default process for the window
				return DefWindowProc(hWnd, uMsg, wParam, lParam );
			}

		}
		
		Win32_GUIRenderer::Win32_GUIRenderer() : 
			GUIRenderer( NAME_SWITCH( Win32 ) )
		{
		}

		Win32_GUIRenderer::~Win32_GUIRenderer()
		{
			destroy();
		}

		//Initialise the renderer
		k_Bool Win32_GUIRenderer::initialise()
		{
			InitCommonControls(); 

			WNDCLASSEX wc;
			wc.cbSize = sizeof( WNDCLASSEX );
			wc.cbClsExtra = 0;
			wc.cbWndExtra = 0;
			wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
			wc.hCursor = LoadCursor(0, IDC_ARROW);
			wc.hIcon = LoadIcon( 0, IDI_QUESTION  );
			wc.hInstance = GetModuleHandle( NULL );
			wc.lpfnWndProc = staticWin32Process;
			wc.lpszClassName = _T("WINDOW");
			wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
			wc.hIconSm = 0;
			wc.lpszMenuName = 0;
			RegisterClassEx( &wc );
		
			//wc.hbrBackground = 0;	
			wc.hbrBackground = 0;		
			wc.lpszClassName = _T("RENDERBOX");
			RegisterClassEx( &wc );

			//WNDCLASS wcn;
			//GetClassInfo( GetModuleHandle( NULL ), _T("BUTTON"), &wcn );
			//wcn.lpfnWndProc = staticWin32Process;		

			//RegisterClass( &wcn );

			m_initialised = true;
			return kc_TRUE;
		}

		//Destroy tyhe renderer
		void Win32_GUIRenderer::destroy()
		{
			if ( !_Gui.isCurrentThread() ) //If not the GUI thread
			{
				_Gui.sendMessage( new GUIMsg_DestroyRenderer( this ) ); //Post a destroy message to the GUI thread
				return;
			}

			for ( k_Map< Widget*, Win32Widget* >::iterator i = m_widgets.begin(); i != m_widgets.end(); ++i )
			{
				safeDelete( i->second ); //delete the widget instance	
			}
			m_widgets.clear(); //Clear the widget map
		}	

		//Update the GUI and poll the message queues
		k_Bool Win32_GUIRenderer::update()
		{
			//-------------------------------------------------------------
			// loop through all win32 messages in the queue
			//-------------------------------------------------------------
			pollWin32MessageQueue();
			
			//-------------------------------------------------------------
			// Poll the renderers message queue for messages
			//-------------------------------------------------------------
			pollMessageQueue();

			return kc_TRUE;
		}

		bool Win32_GUIRenderer::pollWin32MessageQueue()
		{
			assert( !_Gui.isThreadRunning() || _Gui.isCurrentThread() );
			MSG msg; 
			while ( PeekMessage(&msg, 0, 0, 0, PM_NOREMOVE) )
			{ 
				//Get a message from the callback queue
				//Returns -1 = error, 0 = quit message, 1 = normal message
				int bRet = GetMessage(&msg, 0, 0, 0);

				switch ( bRet ) 
				{ 
					case -1: //Check if getMessage failed
						{
							LOG_ERROR( _T("Win32 Message queue failed!") );
							return false; 
						}
					case 0:			
						{
							//LOG_MESSAGE( m_name + " recieved a WM_QUIT message." );
							return false;
						}
					default:
						{
							TranslateMessage(&msg); 
							DispatchMessage(&msg); 
						}
				}
			}
			return true;
		}

		//Process a message sent to the renderer
		void Win32_GUIRenderer::processMessage( Message::Ptr& msg )
		{
			if ( msg->isExactly( GUIMsg_CreateWidget::TYPE ) )
				createWidget( ((GUIMsg_CreateWidget*)msg.getPtr())->widget );
			else			
			if ( msg->isExactly( GUIMsg_ShowWidget::TYPE ) )
				showWidget( ((GUIMsg_ShowWidget*)msg.getPtr())->widget );
			else			
			if ( msg->isExactly( GUIMsg_HideWidget::TYPE ) )
				hideWidget( ((GUIMsg_HideWidget*)msg.getPtr())->widget );
			else			
			if ( msg->isExactly( GUIMsg_SizeWidget::TYPE ) )
				sizeWidget( ((GUIMsg_SizeWidget*)msg.getPtr())->widget, ((GUIMsg_SizeWidget*)msg.getPtr())->size );
			else			
			if ( msg->isExactly( GUIMsg_PositionWidget::TYPE ) )
				positionWidget( ((GUIMsg_PositionWidget*)msg.getPtr())->widget, ((GUIMsg_PositionWidget*)msg.getPtr())->position );
			else			
			if ( msg->isExactly( GUIMsg_SetWidgetText::TYPE ) )
				setWidgetText( ((GUIMsg_SetWidgetText*)msg.getPtr())->widget, ((GUIMsg_SetWidgetText*)msg.getPtr())->text );

		}
		
		//Create a widget
		k_Bool Win32_GUIRenderer::createWidget( Widget* widget )
		{
			Win32Widget* newWidget = new Win32Widget( this, widget );
			m_widgets[ widget ] = newWidget;
			k_Bool bRet = newWidget->create();
			m_hwidgets[ newWidget->hWnd() ] = newWidget;
			return bRet;
		}

		k_Bool Win32_GUIRenderer::hideWidget( Widget* widget ) //hide a widget
		{
			Win32Widget* widget32 = getWidget( widget );
			if ( isValidPointer( widget32 ) )
				return widget32->hide();
			
			widget->getDefinition().visible = false;
			return kc_FALSE;
		}

		k_Bool Win32_GUIRenderer::showWidget( Widget* widget ) //show a widget
		{
			Win32Widget* widget32 = getWidget( widget );
			if ( isValidPointer( widget32 ) )
				return widget32->show();
			
			widget->getDefinition().visible = true;
			return kc_FALSE;
		}

		k_Bool Win32_GUIRenderer::sizeWidget( Widget* widget, const Vector2r& size ) //set the size of the widget
		{
			if ( Win32Widget* widget32 = getWidget( widget ) )
				return widget32->setSize( size );
			
			widget->getDefinition().size = size;
			return kc_FALSE;
		}
		k_Bool Win32_GUIRenderer::positionWidget( Widget* widget, const Vector2r& position )//set the position of the widget
		{
			if ( Win32Widget* widget32 = getWidget( widget ) )
				return widget32->setPosition( position );
			
			widget->getDefinition().position = position;
			return kc_FALSE;
		}

		k_Bool Win32_GUIRenderer::setWidgetText( Widget* widget, const k_String& text )//set the text on the widget
		{
			if ( Win32Widget* widget32 = getWidget( widget ) )
				return widget32->setText( text );
			
			widget->getDefinition().text = text;
			return kc_FALSE;
		}


		//Ge the parent handle for a widget
		Win32Widget* Win32_GUIRenderer::getParent( Win32Widget* widget )
		{
			if ( isValidPointer( widget->impl() ) ) //Check that implimentation exists
			{
				//get the parent implimentation
				Widget* parentWidget = widget->impl()->getParent();
				if ( isValidPointer( parentWidget ) )
					return m_widgets[ parentWidget ];//look the parent implimentation up using the widget map
			}
			return 0;
		}
		
		Win32Widget* Win32_GUIRenderer::getWidget( HWND hWnd ) //Get the widget from its handle
		{
			assert( m_hwidgets.find( hWnd ) != m_hwidgets.end() );
			return m_hwidgets[ hWnd ];//TODO: Check the handle exists
		}

		Win32Widget* Win32_GUIRenderer::getWidget( Widget* impl ) //get a win32 widget from a interface widget
		{
			if ( isValidPointer( impl ) ) //Check that implimentation exists
				return m_widgets[ impl ];//look the parent implimentation up using the widget map
			return 0;
		}



	}
}