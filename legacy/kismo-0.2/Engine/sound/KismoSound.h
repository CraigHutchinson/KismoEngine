/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\sound\KismoSound.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef _KISMO_SOUND_H
#define	_KISMO_SOUND_H

#include "framework/KismoCommon.h"
#include "framework/KismoThread.h"
#include "framework/KismoVector.h"

//#include "sound\KismoGfxMessage.h"

#define _Sound kismo::sound::Sound::getSingleton()

namespace kismo
{
	//Include framework utilities
	using frx::Thread;
	using frx::Singleton;
	using frx::Message;

	namespace sound
	{	
		//The core Sound
		// Is Singleton and runs in its own thread
		class _KismoExport Sound : public Thread, public Singleton< Sound >
		{
		public:
			
			static Sound& getSingleton( void );
			static Sound* getSingletonPtr( void );

		public:

			Sound();
			~Sound();


			/** \brief Post a message to this thread and process it imediately if the thread is current */
			virtual void postMessage( Message::Ptr msg ) 
			{ 				
				if ( shouldThreadTerminate() ) return;
				if ( isCurrentThread() )
					processMessage( msg ); //Process the message now
				else
					sendMessage( msg ); //Add to the message queue
			}
		protected:
			/** \brief Thread process entry point */
			virtual void run();			
			/** \brief Process a message sent to the thread */
			virtual void processMessage( Message::Ptr& msg );
		protected:
		};

	} // END: sound
} // END: kismo

#endif
