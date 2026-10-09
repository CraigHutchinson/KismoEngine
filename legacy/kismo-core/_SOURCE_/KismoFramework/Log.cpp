/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\KismoCore\_SOURCE_\KismoFramework\Log.cpp
 * Status: current (newest copy of this file name in the archive)
 */
/*^**************************************************************************
*	
*	Kismo Engine 
*	© 2008-2013 Craig Hutchinson (craig@craighutchinson.co.uk)
*	All right reserved
*
*	This file is subject to the terms and conditions defined in
*	file 'license.txt', which is part of this source code package.
*
***************************************************************************^*/


#include "KismoFramework/Log.h"
#include "KismoFramework/Timer.h"
#include <cstdlib>
#include <iostream>
#include <windows.h>

namespace kismo
{
	/** Html log file headers */
	static const Char* kHtmlHeaderStart =
		_T("<html> <head>")
		_T("<style type=\"text/css\">")
		_T(" .Profile{color:blue;}")
		_T(" .Comment{color:yellow;}")
			//(Highlighted Black/White - when colours used ;-) )
		_T(" .Message{color:black;}")
			//(Highlighted green - when colours used ;-) )
		_T(" .Success{color:green;}")
			//(Highlighted amber - when colours used ;-) )
		_T(" .Warning{color:orange;}")
		_T(" .RecoverableError{color:orange;}")
			//(Highlighted red - when colours used ;-) )
		_T(" .Error{color:darkred;}")
		_T(" .CriticalError{color:red;}")
		_T(" .FatalError{color:red; bacground:darkred;}")
		_T("</style>")
		_T("<title>");
	static const Char* kHtmlHeaderEnd = _T("</title> </head> <body>");
	static const Char* kHtmlFooter = _T("</body> </html>");

	//template <> Log* Singleton<Log>::ms_singleton = 0;
	//template Singleton<Log>; //< Explicit instantiation	

	
	bool logType( const Char* msg, Log::eMessageType type ) 
	{ return _Log.write( msg, type); }

	bool logVarArg( Log* log,  const Char* format, va_list args, Log::eMessageType messageType )
	{
		if ( !log || log->loggingLevel() > messageType )
		{
			va_end(args);
			return true; //< Silently ignore this message
		}
		
		Char msg[1024];
		_vstprintf_s(msg, 1024, format, args);
		va_end(args);
		
		return log->write( msg, messageType );
	}
	
	/** helper functions for writing different log messages from const Char* */
	bool logProfile( Log* log, const Char* format, ... )
	{
		va_list args;
		va_start(args, format);
		return logVarArg( log, format, args, Log::Profile ); 
	}
	bool logComment( Log* log, const Char* format, ... )
	{
		va_list args;
		va_start(args, format);
		return logVarArg( log, format, args, Log::Comment ); 
	}
	bool logMessage( Log* log, const Char* format, ... )
	{
		va_list args;
		va_start(args, format);
		return logVarArg( log, format, args, Log::Message ); 
	}
	bool logSuccess( Log* log, const Char* format, ... )
	{
		va_list args;
		va_start(args, format);
		return logVarArg( log, format, args, Log::Success ); 
	}
	bool logWarning( Log* log, const Char* format, ... )
	{
		va_list args;
		va_start(args, format);
		return logVarArg( log, format, args, Log::Warning ); 
	}
	bool logRecoverable( Log* log, const Char* format, ... )
	{
		va_list args;
		va_start(args, format);
		return logVarArg( log, format, args, Log::RecoverableError ); 
	}
	bool logError( Log* log, const Char* format, ... )
	{
		va_list args;
		va_start(args, format);
		return logVarArg( log, format, args, Log::Error ); 
	}
	bool logCritical( Log* log, const Char* format, ... )
	{
		va_list args;
		va_start(args, format);
		return logVarArg( log, format, args, Log::CriticalError ); 
	}
	bool logFatal( Log* log, const Char* format, ... )
	{
		va_list args;
		va_start(args, format);
		return logVarArg( log, format, args, Log::FatalError ); 
	}

	Log::Log( const String& filename, Bool useFile, eLoggingLevel loggingLevel, eLoggingLevel haltExecLevel ) :
		m_logFile( filename.c_str() ),
		m_useLogFile( useFile ),
		m_loggingLevel( loggingLevel ),
		m_haltExecLevel( haltExecLevel )
		//m_silentMode( false )
	{
		m_logFile << kHtmlHeaderStart << _T("LogTitleHere") << kHtmlHeaderEnd << _T('\n');
		//TODO: log entities could be in a table which we add a javascript here so they can be sorted/filtered by type etc
		write( _T("Log instance constructed.") );
		//TODO: output current time to log
	}

	Log::~Log()
	{
		m_logFile << kHtmlFooter << _T('\n');
	}

	const Char* Log::getTypeName( eMessageType type )
	{
		switch(type)
		{
		case Log::Profile:			return _T("Profile");
		case Log::Comment:			return _T("Comment");
		case Log::Message:			return _T("Message");
		case Log::Success:			return _T("Success");
		case Log::Warning:			return _T("Warning");
		case Log::RecoverableError: return _T("Recoverable"); 
		case Log::Error:			return _T("Error");
		case Log::CriticalError:	return _T("CriticalError");
		case Log::FatalError:		return _T("FatalError");;
		}
		return 0; //< Invalid!
	}

	bool Log::write( StreamMessage& streamMessage )
	{
		if( !streamMessage.isOpen )
		{
			Assert(false);
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

			Char timeBuff[256];
			_stprintf_s( timeBuff,256,_T("%02u:%02u:%09.6f"), hours, mins, secs );

			const Char* typeName = getTypeName( type );		

			m_logFile << _T("<div class=\"")
				<< typeName << _T("\">") << timeBuff << _T("\t") << typeName << _T('\t') << msg 
				<< _T("</div>\n");	
		}

		if ( !fileOnly )
		{
			for ( LogListenerArray::iterator iter = m_listeners.begin(); iter != m_listeners.end(); ++iter )
			{
				Assert( *iter );
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
			outLog.m_streamMessage.buffer.str(_T(""));
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
			Assert( "Invalid log modifier" == 0 );
			break;
		}
		return outLog;
	}		

	bool Log::registerListener( LogListener* listener )
	{
		m_listeners.push_back( listener ); //add the listerner to the vector
		return true;
	}

	
	LogStream::LogStream( tostream& stream, Log::eLoggingLevel loggingLevel ) : 
		LogListener( loggingLevel ),
		m_stream(stream)
	{}
	
	bool LogStream::write( Log* sourceLog, const Char* msg, Log::eMessageType type )
	{
		//TODO: we could do this better? Use the Clok ticks instead of the Time in Float32 seconds?
		float secs = _SystemTimer.runningTime_secs();
		UInt32 hours = UInt32(secs/(60*60));
		secs -= hours*(60*60);
		UInt32 mins = UInt32(secs/60);
		secs -= mins*60;

		char timeBuff[256];
		sprintf_s( timeBuff,256,"%02u:%02u:%09.6f", hours, mins, secs );

		tcout << '<' << timeBuff << ">\t" << type << '\t' << msg << '\n';
		return true;
	}

	//This will set the forground and background color for printing in a console window.
	static void SetColorAndBackground(int ForgC, int BackC)
	{
		 WORD wColor = ((BackC & 0x0F) << 4) + (ForgC & 0x0F);
		 SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), wColor);
	}

	LogConsoleStream::LogConsoleStream( Log::eLoggingLevel loggingLevel ) : 
		LogStream( tcout, loggingLevel )
	{}

	bool LogConsoleStream::write( Log* sourceLog, const Char* msg, Log::eMessageType type )
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

		LogStream::write( sourceLog, msg, type );

		//revert the console consolveTextColour back!
		SetColorAndBackground( FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE, 0 ); //, Set the console text consolveTextColour
		return true;
	}
} //END: kismo