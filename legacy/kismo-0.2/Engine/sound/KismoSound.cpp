/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\sound\KismoSound.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
#include "KismoSound.h"

namespace kismo
{
	template<> sound::Sound* Singleton< sound::Sound >::ms_instance = 0;

	namespace sound
	{			
		Sound& Sound::getSingleton( void ) { assert( isValidPointer( ms_instance ) ); return ( *ms_instance ); }
		Sound* Sound::getSingletonPtr( void ) { return ms_instance;  }

		Sound::Sound() :
			Thread( _T("Sound"), Thread::kc_ThreadPriority_Minimum )
		{
			start(); //Start the sound thread
		}

		Sound::~Sound()
		{
			assert( !isThreadRunning() );
		}


		void Sound::run()
		{
			while( !shouldThreadTerminate() || hasMessage() )
			{ 	

				//update(); //Update all the renderers
				Thread::sleep( 5 ); //Release some processing to more important tasks
			}
		}

		void Sound::processMessage( Message::Ptr& msg )
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

	} // END: sound
} // END: kismo