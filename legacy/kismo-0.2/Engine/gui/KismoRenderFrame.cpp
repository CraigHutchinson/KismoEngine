/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\gui\KismoRenderFrame.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
#include "gui\KismoRenderFrame.h"
#include "gui\KismoGUIRenderer.h"
#include "gfx\KismoRenderWindow.h"
#include "gfx\KismoRenderer.h"
#include "gfx\KismoGfx.h"

namespace kismo
{
	namespace gui
	{
	
		class CreateContextMsg : public frx::WorkerMessage
		{
		public:
			CreateContextMsg( gfx::RenderWindow* win ) : gfxRenderWindow( win ) {}
			bool run() { return gfxRenderWindow->createContext(); }
			gfx::RenderWindow* gfxRenderWindow;
		};



		KISMO_IMPLEMENT_RTTI( gui, RenderFrame, Window );

		RenderFrame::RenderFrame( Widget* parent, gfx::Renderer* gfxRenderer ) :
			Window( parent, 0 ),//(isValidPointer(parent)? parent->guiRenderer() : 0 )
			m_gfxRenderWindow( 0 ), m_gfxRenderer( 0 )
		{
			WidgetDefinition& def = getDefinition(); 			
			def.style = kc_WidgetStyle_RenderBox;
			def.text = _T("RenderFrame");
			def.resizeable = false;
			
			setGfxRenderer( gfxRenderer );
		};

		bool RenderFrame::setGfxRenderer( gfx::Renderer* gfxRenderer )
		{
			assert( !(m_gfxRenderer && m_gfxRenderWindow && m_gfxRenderWindow->isPrimary()) ); //TODO: Can't change the primary context for a renderer

			//TODO: Check for change of current renderer
			m_gfxRenderer = gfxRenderer;
		
			return true;
		}

		void RenderFrame::create()
		{
			assert( m_gfxRenderer );
			if ( isValidPointer( m_gfxRenderer ) )
			{
				//Send a waiting create message so that the window will be created when the send returns
				gui::GUIMsg_CreateWidget msg( this );
				msg.setWait(); //Set that we want to wait for this message to be prcessed by the recieveing thread
				guiRenderer()->sendMessage( msg ); 	
				
				m_gfxRenderWindow = m_gfxRenderer->createRenderWindow( getName() );
				assert( m_gfxRenderWindow );

				//Add the window to the renderer
				m_gfxRenderer->addRenderTarget( *m_gfxRenderWindow );


				m_gfxRenderWindow->setWindowHandle( getWindowHandle() );
				
				_Gfx.sendMessage( Thread::wait( new CreateContextMsg( m_gfxRenderWindow ) ) ); 	
				
				//if ( !m_gfxRenderWindow->createContext() )
				//	LOG_ERROR( _T("Failed to create render context window") );


				m_gfxRenderer->setInitialised();
				
			}
			else
			{
				gui::Window::create();
			}
		}
		
	}
}
