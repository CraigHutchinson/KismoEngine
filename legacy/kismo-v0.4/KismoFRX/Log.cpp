/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoFRX\Log.cpp
 * Status: superseded by kismo-core/_SOURCE_/KismoFramework/Log.cpp (same name, content differs)
 */
#include "KismoFRX/Log.h"
#include "KismoFRX/Timer.h"
#include <cstdlib>
#include <iostream>
#include <windows.h>

namespace kismo
{
	//template <> Log* Singleton<Log>::ms_singleton = 0;
	//template Singleton<Log>; //< Explicit instantiation	

	
	bool logType( const Char* msg, Log::eMessageType type ) 
	{ return _Log.write( msg, type); }
	
	/** helper functions for writing different log messages from const Char* */
	bool logProfile( const Char* format, ... )
	{
		if ( _Log.loggingLevel() > Log::Profile ) return true;
		Char msg[1024];
		va_list args;
		va_start(args, format);
		_vstprintf_s(msg, 512, format, args);
		va_end(args);
		return _Log.write( msg, Log::Profile ); 
	}
	bool logComment( const Char* format, ... )
	{
		if ( _Log.loggingLevel() > Log::Comment ) return true;
		Char msg[1024];
		va_list args;
		va_start(args, format);
		_vstprintf_s(msg, 512, format, args);
		va_end(args);
		return _Log.write( msg, Log::Comment ); 
	}
	bool logMessage( const Char* format, ... )
	{
		if ( _Log.loggingLevel() > Log::Message ) return true;
		Char msg[1024];
		va_list args;
		va_start(args, format);
		_vstprintf_s(msg, 512, format, args);
		va_end(args);
		return _Log.write( msg, Log::Message ); 
	}
	bool logSuccess( const Char* format, ... )
	{
		if ( _Log.loggingLevel() > Log::Success ) return true;
		Char msg[1024];
		va_list args;
		va_start(args, format);
		_vstprintf_s(msg, 512, format, args);
		va_end(args);
		return _Log.write( msg, Log::Success ); 
	}
	bool logWarning( const Char* format, ... )
	{
		if ( _Log.loggingLevel() > Log::Warning ) return true;
		Char msg[1024];
		va_list args;
		va_start(args, format);
		_vstprintf_s(msg, 512, format, args);
		va_end(args);
		return _Log.write( msg, Log::Warning ); 
	}
	bool logRecoverable( const Char* format, ... )
	{
		if ( _Log.loggingLevel() > Log::RecoverableError ) return true;
		Char msg[1024];
		va_list args;
		va_start(args, format);
		_vstprintf_s(msg, 512, format, args);
		va_end(args);
		return _Log.write( msg, Log::RecoverableError ); 
	}
	bool logError( const Char* format, ... )
	{
		if ( _Log.loggingLevel() > Log::Error ) return true;
		Char msg[1024];
		va_list args;
		va_start(args, format);
		_vstprintf_s(msg, 512, format, args);
		va_end(args);
		return _Log.write( msg, Log::Error ); 
	}
	bool logCritical( const Char* format, ... )
	{
		if ( _Log.loggingLevel() > Log::CriticalError ) return true;
		Char msg[1024];
		va_list args;
		va_start(args, format);
		_vstprintf_s(msg, 512, format, args);
		va_end(args);
		return _Log.write( msg, Log::CriticalError ); 
	}
	bool logFatal( const Char* format, ... )
	{
		if ( _Log.loggingLevel() > Log::FatalError ) return true;
		Char msg[1024];
		va_list args;
		va_start(args, format);
		_vstprintf_s(msg, 512, format, args);
		va_end(args);
		return _Log.write( msg, Log::FatalError ); 
	}

	Log::Log( const String& filename, Bool useFile, eLoggingLevel loggingLevel, eLoggingLevel haltExecLevel ) :
		m_logFile( filename.c_str() ),
		m_useLogFile( useFile ),
		m_loggingLevel( loggingLevel ),
		m_haltExecLevel( haltExecLevel )
		//m_silentMode( false )
	{
		write( _T("Log instance constructed.") );
	}

	bool Log::write( StreamMessage& streamMessage )
	{
		if( !streamMessage.isOpen )
		{
			assert(false);
			return false;
		}
		streamMessage.isOpen = false; //, Must set this to prevent recursion
		return write( streamMessage.buffer.str().c_str(), streamMessage.type, streamMessage.FileOnly );	
	}

	bool Log::write( const Char* msg, eMessageType type, Bool fileOnly )
	{
		if ( m_streamMessage.isOpen ) //< close any open stream if it has not been flushed already
			write( m_streamMessage );

		if ( m_useLogFile )
		{			
			//TODO: we could do this better? Use the Clock ticks instead of the Time in Float32 seconds?
			float secs = _SystemTimer.runningTime_secs();
			UInt32 hours = UInt32(secs/(60*60));
			secs -= hours*(60*60);
			UInt32 mins = UInt32(secs/60);
			secs -= mins*60;

			char timeBuff[256];
			sprintf_s( timeBuff,256,"%02u:%02u:%09.6f", hours, mins, secs );

			m_logFile << '<' << timeBuff << ">\t" << type << '\t' << msg << '\n';	
		}

		if ( !fileOnly )
		{
			for ( LogListenerArray::iterator iter = m_listeners.begin(); iter != m_listeners.end(); ++iter )
			{
				assert( *iter );
				if ( (*iter)->getLoggingLevel() <= type )
					(*iter)->write( this, msg, type ); //Pass on the message to the listener
			}
		}
		return true;
	}

	Log& operator << ( Log& outLog, const Log::eMessageType& type )
	{
		if ( outLog.m_streamMessage.isOpen ) //, Write existing log message
			outLog.write( outLog.m_streamMessage );

		outLog.m_streamMessage.type = type;
		if ( outLog.m_streamMessage.type >= outLog.m_loggingLevel )
		{
			outLog.m_streamMessage.isOpen = true;
			outLog.m_streamMessage.FileOnly = false;
			outLog.m_streamMessage.buffer.str(String());
		}
		return outLog;
	}
	
	Log& operator << ( Log& outLog, const Log::eLogModifiers& mod )
	{
		switch( mod )
		{
		case Log::Flush:
			if ( outLog.m_streamMessage.isOpen ) 
				outLog.write( outLog.m_streamMessage ); 
			break;

		case Log::FileOnly: outLog.m_streamMessage.FileOnly = true; break;
		default:
			assert( "Invalid log modifier" == 0 );
			break;
		}
		return outLog;
	}		

	bool Log::registerListener( LogListener* listener )
	{
		m_listeners.push_back( listener ); //add the listerner to the vector
		return true;
	}

	
	LogCOut::LogCOut( Log::eLoggingLevel loggingLevel ) : 
		LogListener( loggingLevel )
	{}

	//This will set the forground color for printing in a console window.
	//void SetColor(int ForgC)
	//{
	//	WORD wColor;

	//	//We will need this handle to get the current background attribute
	//	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	//	CONSOLE_SCREEN_BUFFER_INFO csbi;	    

	//	//We use csbi for the wAttributes word.
	//	if(GetConsoleScreenBufferInfo(hStdOut, &csbi))
	//	{
	//	   //Mask out all but the background attribute, and add in the forgournd color
	//		 wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
	//		 SetConsoleTextAttribute(hStdOut, wColor);     
	//	}
	//}

	//This will set the forground and background color for printing in a console window.
	void SetColorAndBackground(int ForgC, int BackC)
	{
		 WORD wColor = ((BackC & 0x0F) << 4) + (ForgC & 0x0F);
		 SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), wColor);
	}

	bool LogCOut::write( Log* sourceLog, const Char* msg, Log::eMessageType type )
	{
		int consolveTextColour = 0;
		int consolveBackgroundColour = 0;
		switch(type)
		{
		case Log::Profile: consolveTextColour = FOREGROUND_BLUE|FOREGROUND_INTENSITY; break;
		case Log::Comment: consolveTextColour = FOREGROUND_GREEN|FOREGROUND_BLUE; break;

			//(Highlighted Black/White - when colours used ;-) )
		case Log::Message: consolveTextColour = FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE; break;

			//(Highlighted green - when colours used ;-) )
		case Log::Success: consolveTextColour = FOREGROUND_GREEN; break;

			//(Highlighted amber - when colours used ;-) )
		case Log::Warning: consolveTextColour = FOREGROUND_RED|FOREGROUND_GREEN; break;
		case Log::RecoverableError: consolveTextColour = FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_INTENSITY; break;

			//(Highlighted red - when colours used ;-) )
		case Log::Error: consolveTextColour = FOREGROUND_RED; break;
		case Log::CriticalError:
			consolveTextColour = FOREGROUND_RED | FOREGROUND_INTENSITY; break;
		case Log::FatalError: 	
			consolveTextColour = FOREGROUND_RED | FOREGROUND_INTENSITY;
			consolveBackgroundColour = FOREGROUND_RED; break;
		}
		SetColorAndBackground( consolveTextColour, consolveBackgroundColour ); //, Set the console text consolveTextColour

		//TODO: we could do this better? Use the Clok ticks instead of the Time in Float32 seconds?
		float secs = _SystemTimer.runningTime_secs();
		UInt32 hours = UInt32(secs/(60*60));
		secs -= hours*(60*60);
		UInt32 mins = UInt32(secs/60);
		secs -= mins*60;

		char timeBuff[256];
		sprintf_s( timeBuff,256,"%02u:%02u:%09.6f", hours, mins, secs );

		std::cout << '<' << timeBuff << ">\t" << type << '\t' << msg << '\n';

		//revert the console consolveTextColour back!
		SetColorAndBackground( FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE, 0 ); //, Set the console text consolveTextColour
		return true;
	}

} //END: kismo