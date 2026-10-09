/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\gfx\KismoGfx.cpp
 * Status: superseded by kismo-v0.4/KismoGFX/KismoGFX.cpp (same name, content differs)
 */
#include "KismoGfx.h"

#include "KismoRenderer.h"

namespace kismo
{
	template<> gfx::Gfx* Singleton< gfx::Gfx >::ms_instance = 0;

	namespace gfx
	{			
		Gfx& Gfx::getSingleton( void ) { assert( isValidPointer( ms_instance ) ); return ( *ms_instance ); }
		Gfx* Gfx::getSingletonPtr( void ) { return ms_instance;  }

		Gfx::Gfx() :
			Thread( _T("Gfx"), Thread::kc_ThreadPriority_Normal )
		{
			start(); //Start the gfx thread
		}

		Gfx::~Gfx()
		{
			assert( !isThreadRunning() );
			m_renderers.clear();
		}

		//Add a renderer to the map of available gui renderers
		bool Gfx::addRenderer(  const k_String& name, Renderer* renderer )
		{
			if ( getRenderer( name ) )
			{
				return kc_FALSE;
			}

			m_renderers.insert( RENDERER_MAP::value_type( name, renderer ) );
			return true;
		}

		//get a GUi renderer from its name
		Renderer* Gfx::getRenderer( const k_String& name )
		{
			RENDERER_MAP::const_iterator i = m_renderers.find( name );
			
			if ( i == m_renderers.end() )
			{
				return 0;
			}

			return i->second;
		}

		//remove a renderer
		bool Gfx::removeRenderer( const k_String& name )
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

		bool Gfx::removeRenderer( Renderer* renderer )
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

		k_Bool Gfx::update()
		{
			k_Bool success = true; //If any renderer is updated Ok then we return true

			pollMessageQueue();	

			for ( RENDERER_MAP::const_iterator i = m_renderers.begin(); i !=  m_renderers.end(); ++i )
			{
				Renderer* renderer = i->second;
				if ( isValidPointer( renderer ) && renderer->initialised() )
				{
					///TODO: Better stuff here
					success |= renderer->beginFrame(); 
					renderer->endFrame(); 
				}
			}
			return success;
		}
		

		void Gfx::run()
		{
			while( !shouldThreadTerminate() || hasMessage() )
			{ 	

				update(); //Update all the renderers
				//Thread::sleep( 0 ); //Release some processing to more important tasks
			}
		}

		void Gfx::processMessage( Message::Ptr& msg )
		{
			/*if ( msg->isExactly( GUIMsg_InitialiseRenderer::TYPE ) )
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
			}*/
			//else
			{
				defaultMessageHandler( msg ); //Pass the message to the default message handler
			}

		}

	} // END: gfx
} // END: kismo