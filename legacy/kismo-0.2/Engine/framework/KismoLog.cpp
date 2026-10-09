/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\framework\KismoLog.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
#include "KismoLog.h"

namespace kismo
{
	namespace frx
	{
		KISMO_IMPLEMENT_RTTI( framework, Log::LogMessage, Message );

		const k_Count kc_MaximumLogQueueSize = 1024; //Maximum number of messages that can be written in one log update

		template<> Log* Singleton< Log >::ms_instance = 0;
		Log& Log::getSingleton( void ) { assert( isValidPointer( ms_instance ) ); return ( *ms_instance ); }
		Log* Log::getSingletonPtr( void ) { return ms_instance;  }

		Log::Log( const k_String& filename, 
			ke_LoggingLevel loggingLevel, //The level at + above which messages are actually logged
			ke_LoggingLevel alertLevel, //Level at which a message will be shown
			k_Bool useFile 
		) :
			Thread( _T("LOG"), Thread::kc_ThreadPriority_Idle ),
			m_file( filename.c_str() ),
			m_loggingLevel( loggingLevel ),
			m_alertLevel( alertLevel ),
			m_silentMode( kc_FALSE )
		{
			LOG_MESSAGE( _T("Log instance created OK") );

			setThreadTerminateTimeout( 60000 );//Thread has 60 seconds to terminate on destruction

			start(); //Start the logging thread
		
		}
		
		Log::~Log()
		{
			assert( !isThreadRunning() );
			pollMessageQueue();
		}

		void Log::write( LogMessage* msg )
		{ 				
			sendMessage( msg, sizeof(LogMessage)  );
			//Orphan the string so it is not deleted when the logMessage is
			msg->m_string.orphan(); 
		}

		void Log::run()
		{
			while ( !shouldThreadTerminate() 
				|| hasMessage() )
			{
				//Clear the message queue
				pollMessageQueue();

				sleep( 10 ); //TODO: Varable wait time? Maybe make the thread loop part of the thread class?
			}
		}

		void Log::processMessage( Message* msg )
		{
			if ( !msg->isExactly( LogMessage::TYPE ) )
			{
				assert( "Invalid Message type!" == false );
			}

			LogMessage* tmsg = dynamic_cast< LogMessage* >( msg );
			m_file << tmsg->type() << _T('\t') << tmsg->string().c_str() << _T('\n');
			DEBUG_ONLY( 
					if ( tmsg->type() >= kc_Message )
					{
						OutputDebugString( tmsg->string().c_str() ); 
						OutputDebugString( _T("\n") ); 
					}
					m_file.flush();	
				);


		}
	}

}