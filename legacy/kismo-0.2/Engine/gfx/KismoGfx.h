/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\gfx\KismoGfx.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef _KISMO_GFX_H
#define	_KISMO_GFX_H

#include "framework/KismoCommon.h"
#include "framework/KismoThread.h"
#include "framework/KismoVector.h"

#include "gfx\KismoGfxMessage.h"

#define _Gfx kismo::gfx::Gfx::getSingleton()

namespace kismo
{
	//Include framework utilities
	using frx::Thread;
	using frx::Singleton;
	using frx::Vector2r;
	using frx::Message;

	namespace gfx
	{	
		//Forward declaration
		class Renderer;

		//The core Gfx
		// Is Singleton and runs in its own thread
		class _KismoExport Gfx : public Thread, public Singleton< Gfx >
		{
		public:
			
			static Gfx& getSingleton( void );
			static Gfx* getSingletonPtr( void );

		public:

			Gfx();
			~Gfx();

			//Add a renderer to the map of available renderers
			bool addRenderer( const k_String& name, Renderer* renderer );

			/** \brief Get a renderer by name
			\param name The name of the renderer
			\return Pointer to the renderer*/
			Renderer* getRenderer( const k_String& name );
			
			/** \brief Remove a renderer by name
			\param name The name of the renderer
			\return Whether the renderer was removed*/
			bool removeRenderer( const k_String& name );
			/** \brief Remove a renderer with a pointer to its instance
			\param renderer The renderer
			\return Whether the renderer was removed*/
			bool removeRenderer( Renderer* renderer );

			/** \brief Update all the renderers (D not use if the Gfx thread is running) */
			k_Bool update();

			///** \brief Post a message to this thread and process it imediately if the thread is current */
			//virtual void postMessage( Message::Ptr msg ) 
			//{ 				
			//	if ( shouldThreadTerminate() ) return;
			//	if ( isCurrentThread() )
			//		processMessage( msg ); //Process the message now
			//	else
			//		sendMessage( msg ); //Add to the message queue
			//}
		protected:
			/** \brief Thread process entry point */
			virtual void run();			
			/** \brief Process a message sent to the thread */
			virtual void processMessage( Message::Ptr& msg );
		protected:
			/** \brief Type definition for the renderer map with a string anme as the key */			
			typedef k_Map< k_String, Renderer* > RENDERER_MAP;
			/** \brief Map of available Graphics renderers */
			RENDERER_MAP m_renderers;
		};

	} // END: gfx
} // END: kismo

#endif
