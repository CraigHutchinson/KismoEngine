/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\Framework\Log.cpp
 * Status: superseded by kismo-core/_SOURCE_/KismoFramework/Log.cpp (same name, content differs)
 */
#include "Log.h"
#include "Platform/Platform.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

template<> frx::GlobalLog* frx::Singleton< frx::GlobalLog >::ms_instance = 0;
namespace frx
{
	const Count kc_MaximumLogQueueSize = 1024; //Maximum number of messages that can be written in one log update
	
	//IMPL_SINGLETON( GlobalLog );
	
	GlobalLog& GlobalLog::getSingleton( void )
	{ assert( isValidPointer( ms_instance ) ); return ( *ms_instance ); }
	GlobalLog* GlobalLog::getSingletonPtr( void )
	{ return ms_instance;  }
	


	//-----------------------------------------
	// Log - LogListener
	//-----------------------------------------
	Log::LogListener::LogListener()
	{ 
	}

	Log::LogListener::~LogListener()
	{
		for ( LogVector::iterator lIter = m_logs.begin(); lIter != m_logs.end(); ++lIter )
		{
			assert( *lIter );
			(*lIter)->removeListener( this );
		}
		m_logs.clear();
	}

	void Log::LogListener::listenToLog( Log* log )
	{
		assert( log );
		for ( LogVector::iterator lIter = m_logs.begin(); lIter != m_logs.end(); ++lIter )
		{
			assert( *lIter );
			if ( *lIter == log )
				return; ///< Already registered with this log
		}

		if ( log ) //If the new log has been specified
		{
			m_logs.push_back( log ); //Add to the logs list of listeners
			log->addListener( this );
		}
	}

	void Log::LogListener::stopListeningToLog( Log* log )
	{
		log->removeListener( this );
		for ( LogVector::iterator lIter = m_logs.begin(); lIter != m_logs.end(); ++lIter )
		{
			assert( *lIter );
			if ( *lIter == log )
			{
				m_logs.erase( lIter );
				return;
			}
		}
	}

	//-----------------------------------------
	// Log
	//-----------------------------------------
	Log::LogMessage* Log::createMessage( eMessageType messageType, const Char* function, UInt32 lineNumber, Bool isFileOnly)
	{ return new LogMessage( messageType, function, lineNumber, isFileOnly); }

	Log::Log( const String& name, eLoggingLevel loggingLevel, eLoggingLevel alertLevel ) :
		m_file( ( name + _T(".log") ).c_str() ),
		m_loggingLevel( loggingLevel ),
		m_alertLevel( alertLevel ),
		m_silentMode( false )
	{
		m_file << _T("Log instance created OK\n");	
	}
	
	Log::~Log()
	{		
		detatchFromThread();

		//Make sure the message queue is emptied
		pollMessageQueue();

		for ( ListenerVector::iterator lIter = m_listeners.begin(); lIter != m_listeners.end(); lIter = m_listeners.begin() )
			(*lIter)->stopListeningToLog( this );
	}

	Log::eHandleAlert Log::write( LogMessage* msg )
	{ 	
		if ( msg->type() >= m_alertLevel ) ///< Check if we need to halt and act on this message now!
		{
			//TODO: Make sure this is thread safe by 'freezing' other threads to prevent lockup
			pollMessageQueue();

			//Process the message on the current thread
			return processMessage( msg );
		}
		else
		{
			m_messageQueue.push( msg ); 
			return HandleAlert_Default;
		}
	}

	Bool Log::threadPhase()
	{		
		pollMessageQueue();//Clear the message queue

		return true; //Success, don't stop logging!
	}

	Bool Log::pollMessageQueue()
	{
		LogMessageQueue::Node* msgNode;
		while ( (msgNode = m_messageQueue.pop()) ) //get the next message..if one exists
		{
			processMessage( msgNode->value ); //Process the message

			delete msgNode; //Remove the last message
		}

		return true; //Return that there were messages
	}

	Log::eHandleAlert Log::processMessage( const LogMessage* msg )
	{
		eHandleAlert ret = HandleAlert_Default; ///< Return value

		//Log the message to file
		m_file << msg->type() << _T('\t');
		DEBUG_ONLY(
			if ( msg->getFunctionName() ) 
				m_file << msg->getFunctionName() << _T('\t') << msg->getLineNumber() << _T('\t') );
		m_file << msg->str().c_str() << _T('\n');

		//In debug output the messages and flush the stream jsut incase
		DEBUG_ONLY( m_file.flush();	 );
	
		//Post the message to log listeners e.g. Console screen
		for ( ListenerVector::const_iterator lIter = m_listeners.begin(); lIter != m_listeners.end(); ++lIter )
		{
			assert( *lIter );
			(*lIter)->write( msg, this );
		}

		//If the message is above alert level we need to show a message
		if ( msg->type() >= m_alertLevel )
		{		
			String title = _T("Error level - ") + String::number( msg->type() );
			switch ( MessageBox( 0, msg->str().c_str(), title.c_str(), MB_ABORTRETRYIGNORE ) )
			{
			case IDRETRY: ret = HandleAlert_Retry;//< The error location should be attempted again if possible
			case IDIGNORE: ret = HandleAlert_Ignore;//< COntinue execution as normal
			case IDABORT:
			default: ret = HandleAlert_Break;//< Cause a breakpoint
			}
		}

		delete msg; ///< TODO: This is not module safe as the message was created elsewhere

		return ret;
	}

	Bool Log::addListener( LogListener* listener )
	{
		assert( listener );

		for ( ListenerVector::iterator lIter = m_listeners.begin(); lIter != m_listeners.end(); ++lIter )
		{
			if ( *lIter == listener )
			{
				return true; //already rgistered
			}
		}

		m_listeners.push_back( listener );

		return true;
	}

	Bool Log::removeListener( LogListener* listener )
	{		
		for ( ListenerVector::iterator lIter = m_listeners.begin(); lIter != m_listeners.end(); ++lIter )
		{
			if ( *lIter == listener )
			{
				m_listeners.erase( lIter ); //erase this from the logs list of listeners
				return true;
			}
		}
		return false;
	}

} //END: frx