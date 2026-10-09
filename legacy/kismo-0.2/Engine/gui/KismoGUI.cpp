/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\gui\KismoGUI.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
#include "KismoGUI.h"

#include "KismoGUIRenderer.h"
#include "KismoWidget.h"



namespace kismo
{
	template<> gui::GUI* Singleton< gui::GUI >::ms_instance = 0;

	namespace gui
	{			
		GUI& GUI::getSingleton( void ) { assert( isValidPointer( ms_instance ) ); return ( *ms_instance ); }
		GUI* GUI::getSingletonPtr( void ) { return ms_instance;  }

		GUI::GUI() :
			Thread( _T("GUI"), Thread::kc_ThreadPriority_Minimum )
		{
			start(); //Start the gui thread
		}

		GUI::~GUI()
		{
			assert( !isThreadRunning() );
			m_renderers.clear();
		}

		//Add a renderer to the map of available gui renderers
		bool GUI::addRenderer(  const k_String& name, GUIRenderer* renderer )
		{
			if ( getRenderer( name ) )
			{
				return kc_FALSE;
			}

			m_renderers.insert( RENDERER_MAP::value_type( name, renderer ) );
			return true;
		}

		//get a GUi renderer from its name
		GUIRenderer* GUI::getRenderer( const k_String& name )
		{
			RENDERER_MAP::const_iterator i = m_renderers.find( name );
			
			if ( i == m_renderers.end() )
			{
				return 0;
			}

			if ( !i->second->initialised() ) //if not initialised yet...
				sendMessage( new GUIMsg_InitialiseRenderer( i->second ) );

			return i->second;
		}

		//remove a renderer
		bool GUI::removeRenderer( const k_String& name )
		{ 
			RENDERER_MAP::iterator i = m_renderers.find( name );
			
			if ( i != m_renderers.end() )
			{
				i->second->destroy();
				safeDelete( i->second );
				m_renderers.erase( i );
				return kc_TRUE;;
			}

			return kc_FALSE;
		}

		bool GUI::removeRenderer( GUIRenderer* renderer )
		{
			assert( isCurrentThread() ); //TODO: Post message if not current thread

			if ( renderer == 0 )
			{
				return kc_FALSE;
			}
			
			for ( RENDERER_MAP::iterator i = m_renderers.begin(); i !=  m_renderers.end(); ++i )
			{
				if ( i->second == renderer )
				{
					m_renderers.erase( i );
					return kc_TRUE;
				}				
			}

			return kc_FALSE;
		}

		k_Bool GUI::update()
		{
			k_Bool success = true; //If any renderer is updated Ok then we return true

			pollMessageQueue();	

			for ( RENDERER_MAP::const_iterator i = m_renderers.begin(); i !=  m_renderers.end(); ++i )
			{
				if ( isValidPointer( i->second ) && i->second->initialised() )
				{
					success |= i->second->update(); 
				}
			}
			return success;
		}
		

		void GUI::run()
		{
			while( !shouldThreadTerminate() || hasMessage() )
			{ 	
				update(); //Update all the renderers
				Thread::sleep( 5 ); //Release some processing to more important tasks
			}
		}

		void GUI::processMessage( Message::Ptr& msg )
		{
			if ( msg->isExactly( GUIMsg_InitialiseRenderer::TYPE ) )
				((GUIMsg_InitialiseRenderer*)msg.getPtr())->renderer->initialise();
			else
			if ( msg->isExactly( GUIMsg_DestroyRenderer::TYPE ) )
			{
				((GUIMsg_DestroyRenderer*)msg.getPtr())->renderer->destroy();
			}
			else
			if ( msg->isExactly( GUIMsg_RemoveRenderer::TYPE ) )
			{
				((GUIMsg_RemoveRenderer*)msg.getPtr())->renderer->destroy();
				removeRenderer( ((GUIMsg_RemoveRenderer*)msg.getPtr())->renderer );
				safeDelete( ((GUIMsg_RemoveRenderer*)msg.getPtr())->renderer );
			}
			else
			{
				defaultMessageHandler( msg ); //Pass the message to the default message handler
			}

		}

	} // END: gui
} // END: kismo