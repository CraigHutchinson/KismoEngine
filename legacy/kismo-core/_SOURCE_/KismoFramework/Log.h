/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\KismoCore\_SOURCE_\KismoFramework\Log.h
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

#ifndef KISMOFRAMEWORK_LOG_H
#define	KISMOFRAMEWORK_LOG_H

#include "KismoFramework/Singleton.h"
#include "KismoFramework/String.h"
#include "KismoFramework/Types.h"
#include "KismoFramework/Vector.h"

#include <sstream>
#include <fstream>

#define _Log Log::getSingleton()

namespace kismo
{
	//<forward declaration
	class Log;

	/** Helper functions for writing different log messages from const Char* */
	bool logProfile( Log* log, const Char* format, ... );
	bool logComment( Log* log, const Char* format, ... );
	bool logMessage( Log* log, const Char* format, ... );
	bool logSuccess( Log* log, const Char* format, ... );
	bool logWarning( Log* log, const Char* format, ... );
	bool logRecoverable( Log* log, const Char* format, ... );
	bool logError( Log* log, const Char* format, ... );
	bool logCritical( Log* log, const Char* format, ... );
	bool logFatal( Log* log, const Char* format, ... );

	/** Helper functions for writing different log messages from const String */
	inline bool logProfile( Log* log, const String& msg );
	inline bool logComment( Log* log, const String& msg );
	inline bool logMessage( Log* log, const String& msg );
	inline bool logSuccess( Log* log, const String& msg );
	inline bool logWarning( Log* log, const String& msg );
	inline bool logRecoverable( Log* log, const String& msg );
	inline bool logError( Log* log, const String& msg );
	inline bool logCritical( Log* log, const String& msg );
	inline bool logFatal( Log* log, const String& msg );

	/** Logging and error class
		\remarks LogListeners can be attached to send log data to a console, window, etc
		\remarks messages can be writen through the write() method or via a stream method
		\warning The stream interface (and most parts) are currently not thread safe and there should be thread local m_streamMessage per thread or an array of streamMessage objects!? TODO: <<< log thrading
		\todo Is this sensible to be a multi-instance singleton? Maybe just remove singleton all together */
	class Log : public Singleton< Log, true >
	{
	public:

		/** LogModifiers to identify end of log message etc when writing using the stream method */
		enum eLogModifiers
		{
            Flush = -1000, //<End the message and Flush the streamMessage buffer - without this the current message is not writen until the next message is started! or write() is called
			FileOnly, //< This message should be logged to file only
		};

		enum eMessageType
		{
			//<(Highlighted grey - when colours used ;-) )
			Profile = -2, //<A profiler output - e.g. function timings
			Comment, //<A non important message - e.g. debugging output

			//<(Highlighted Black/White - when colours used ;-) )
			Message, //<Status messages for normal program output, GENERAL
			
			//<(Highlighted green - when colours used ;-) )
			Success, //<Success message for completion of a task etc.. 

			//<(Highlighted amber - when colours used ;-) )
			Warning, //<Something isn't exactly correct such as an invalid value/parameter		
			RecoverableError, //<Non critical warning which will maintain normal program flow

			//<(Highlighted red - when colours used ;-) )
			Error, //<Error which will cause the current function to terminate early with recuring effects later - use RecoverableError if this is not the case
            CriticalError, //<Critical error which will cause the application to close in a clean manner
            FatalError, //<Fatal error which will cause the application to be unable to continue to close or function

			_MessageType_Start = Profile,
			_MessageType_End = FatalError,
		};

		enum eLoggingLevel
		{
			LoggingLevel_VeryLow = CriticalError,
            LoggingLevel_Low = Error,
            LoggingLevel_MediumLow = RecoverableError,
			LoggingLevel_Medium = Warning,
			LoggingLevel_Normal = Success,
			LoggingLevel_AboveNormal = Message,
			LoggingLevel_High = Comment,
			LoggingLevel_All = Profile,
		};

		/** Base class for any class that wants to also recieve log messages
			\remarks The LogListener does nto keep track of what logs it is attached to and only receives callbacks
			\todo if a log is destroyed it could send a message etc on attach/destroy etc */
		class LogListener 
		{
			friend class Log; //<Log needs access to the write method
		public:

			LogListener( eLoggingLevel loggingLevel = LoggingLevel_Normal ) : 
				m_loggingLevel( loggingLevel )
			{}

			eLoggingLevel getLoggingLevel() const
			{ return m_loggingLevel; }

			void setLoggingLevel( eLoggingLevel level )
			{ m_loggingLevel = level; }

		protected:

			/** Virtual write function called by Log
				\param[in] sourceLog Poinetr top the log instance that sent the message. Usefull if there are m ultiple logs being listened too! */
			virtual bool write( Log* sourceLog, const Char* message, eMessageType type ) = 0;

			virtual void onLogAttached( Log* sourceLog ){}
			virtual void onLogDetached( Log* sourceLog ){}
		protected:

			/** The level of logging to use, messages with a type below this will not be sent to this listener */
			eLoggingLevel m_loggingLevel; 
		};

		typedef std::vector< LogListener*> LogListenerArray;

		/** Stores a message with s string stream buffer for writign using the std stream overloads */
		struct StreamMessage
		{
			Bool isOpen; //< Is a valid log message stored in this container
			Bool FileOnly; //< Whether message is only for output to the log file
			eMessageType type; //< Stores the type of the curren oen message
			std::basic_ostringstream<Char> buffer; //< Buffer to store the current message

			StreamMessage() : isOpen(false), FileOnly(false), type(Message) {}
		};

	public:

		/** returns the type-name from a type enum. i.e Log::Message returns "Message" */
		static const Char* Log::getTypeName( eMessageType type );

	public:
		/** Constructor requires a filename which output will be written
			\todo the useFile flag is confusing
			\param[in] loggingLevel The level at + above which messages are actually logged
			\param[in] haltExecLevel Level at which a message will be shown and 'write()' will nto exit until user intervention occurs
		*/
		Log( const String& filename, Bool useFile = true, eLoggingLevel loggingLevel = LoggingLevel_Normal, eLoggingLevel haltExecLevel = LoggingLevel_MediumLow );
		~Log();

		/**  Get the logging level which is the level at which messages will be processed */
		eLoggingLevel loggingLevel() const 
		{ return m_loggingLevel; }

		/** Get the halt level at which user interaction is required before the write() call is completed
			\remarks This is for fatal/critical errors */
		eLoggingLevel haltLevel() const 
		{ return m_haltExecLevel; }

		//<Silent mode
	//<	void activateSilentMode() 
		//<{ m_silentMode = kc_TRUE; }

		//<void deactivateSilentMode() 
		//<{ m_silentMode = false; }

		/**
			\brief Write a message to the log file
			\brief FileOnly will hide the message from other LogListeners - may be used for extra information to be placed in log but hidden from user interface
		*/
		bool write( const String& msg, eMessageType type = Message, Bool FileOnly = false )
		{ return write( msg.c_str(), type, FileOnly ); }

		bool write( StreamMessage& streamMessage );

		bool write( const Char* msg, eMessageType type = Message, Bool FileOnly = false );

		friend Log& operator << ( Log& outLog, const eMessageType& type );

		//<Stream operator
		template < class T >
		friend Log& operator << ( Log& outLog, const T& msg )
        {
			//<TODO: if the log wasn't opened we could open it by default as a message??
			Assert( (outLog.m_streamMessage.type < outLog.m_loggingLevel) || outLog.m_streamMessage.isOpen == true );

			if ( outLog.m_streamMessage.isOpen )
				outLog.m_streamMessage.buffer << msg; //<Write the message body to the buffer
            return outLog;
        }
		
		friend Log& operator << ( Log& outLog, const eLogModifiers& mod );	

		/** Register a listener to send write messages onto */
		bool registerListener( LogListener* listener );

   	protected: 

		eLoggingLevel m_loggingLevel; //< The level of logging to use, will ignore all messages below this level
		eLoggingLevel m_haltExecLevel; //< The message level at which a stop will occur and force user to see the message in a message box or such.
		//<Bool m_silentMode; //<In silent mode halt level messages aren't shown

		tofstream m_logFile; //<The output filestream for the log if m_useLogFile==true
		Bool m_useLogFile; //< Whether the log file should be used or not - can be used to turn ooff file loggin etc!
		
		StreamMessage m_streamMessage; //< Stream buffer with message settings for use in c++ stream operator

#pragma warning(disable:4251)
		LogListenerArray m_listeners; //< Array fo log listener objects which get sent any messages we receive
#pragma warning(default:4251)
	};
	

	/**
		\brief Stream redirector class for listening to log messages and writing them all to the std::stream
	*/
	class LogStream : public Log::LogListener
	{
	public:
		LogStream( tostream& stream, Log::eLoggingLevel loggingLevel = Log::LoggingLevel_All );
	protected:	
		virtual bool write( Log* sourceLog, const Char* message, Log::eMessageType type ); //<virtual write function called by Log
	protected:
		tostream& m_stream;
	};

	/**
		\brief Stream redirector specialised for console output in which text colour is set based on 
		\todo Colour changes are win32 specific
	*/
	class LogConsoleStream : public LogStream
	{
	public:
		LogConsoleStream( Log::eLoggingLevel loggingLevel = Log::LoggingLevel_All );
	protected:	
		virtual bool write( Log* sourceLog, const Char* message, Log::eMessageType type ); //<virtual write function called by Log
	};

	/** helper functions for writing different log messages from const String */
	bool logProfile( Log* log, const String& msg )		{ return log->write( msg, Log::Profile ); }
	bool logComment( Log* log, const String& msg )		{ return log->write( msg, Log::Comment ); }
	bool logMessage( Log* log, const String& msg )		{ return log->write( msg, Log::Message ); }
	bool logSuccess( Log* log, const String& msg )		{ return log->write( msg, Log::Success ); }
	bool logWarning( Log* log, const String& msg )		{ return log->write( msg, Log::Warning ); }
	bool logRecoverable( Log* log, const String& msg )	{ return log->write( msg, Log::RecoverableError ); }
	bool logError( Log* log, const String& msg )		{ return log->write( msg, Log::Error ); }
	bool logCritical( Log* log, const String& msg )		{ return log->write( msg, Log::CriticalError ); }
	bool logFatal( Log* log, const String& msg )		{ return log->write( msg, Log::FatalError ); }


} //<END: kismo

#endif