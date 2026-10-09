/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\Framework\Log.h
 * Status: superseded by kismo-core/_SOURCE_/KismoFramework/Log.h (same name, content differs)
 */
#ifndef FRX_LOG_H
#define	FRX_LOG_H

#include "Platform/Platform.h"
#include "Framework/Singleton.h"
#include "framework/DateTime.h"
#include "framework/Thread.h"
#include "framework/LockFreeQueue.h"

#include <sstream>
#include <fstream>

#define _GlobalLog frx::GlobalLog::getSingleton()
#define _GlobalLogPtr frx::GlobalLog::getSingletonPtr()

/** \brief Logging helper macro 
*/
#define LOG( lvl, msg ) _GlobalLog.write( (frx::Log::LogMessage*)&(*(frx::Log::createMessage( frx::Log::lvl, _T(__FUNCTION__), __LINE__ )) << msg) );

/** \brief Logging helper macros for specific log types 
*/
#define logSuccess( msg ) LOG( Success, msg );
#define logWarning( msg ) LOG( Warning, msg );
#define logRecoverable( msg ) LOG( RecoverableError, msg );
#define logError( msg ) LOG( Error, msg );
#define logCritical( msg ) LOG( CriticalError, msg );

//Don't log messages in release
#ifdef _DEBUG
#	define logMessage( msg ) LOG( Message, msg )
#else
#	define logMessage( msg )
#endif

//Only log comments in metrics builds
#ifdef METRICS
#	define logComment( msg ) LOG( Comment, msg )
#else
#	define logComment( msg )
#endif

namespace frx
{		
	/** \brief Logging & error class
	*/
	class _DllExport Log : public Threaded
	{
		friend class LogListener; //Log listener needs access to register itself as a listener

	public:

		enum eMessageType ///< The type of log message
		{
			ProfilerMessage = -2, ///< A profiler output - e.g. function timings
			Comment, ///< A non important message - e.g. debugging output
			Message, ///< Status messages for normal program output, GENERAL
			Success, ///< Success message for completion of a task etc.. 
			Warning, ///< Something isn't exactly correct such as an invalid value/paramater		
			RecoverableError, ///< Non critical warning which will maintain normal program flow
			Error, ///< Error which will cause the current function to terminate early with recuring effects later
			CriticalError, ///< Critical error which will cause the application to close
		};

		/** \brief Enumerator for specifying the level of errors to log to file or display
			This is also used for specifying the alert level
		*/
		enum eLoggingLevel
		{
			Lvl_VeryLow = CriticalError,
			Lvl_Low = Error,
			Lvl_Medium = Warning,
			Lvl_Normal = Message,
			Lvl_All = Comment,
		};

		/** \brief Specifies the retyrn value of an alert log message used
			to define how execution should continue
		*/
		enum eHandleAlert
		{
			HandleAlert_Default, //< COntinue execution as normal
			HandleAlert_Retry, //< The error location should be attempted again
			HandleAlert_Ignore, //< Continue ignoring the eror
			HandleAlert_Break, //< Cause a breakpoint
		};

		/** \brief Defines the structure of a log message which is basically a stream object
		*/
		class _DllExport LogMessage : public std::basic_stringstream< Char >
		{
			friend class Log;
		protected:
			
			/** \brief Message constructor. protected as can only be created via 'Log::createMessage'
			\param[in] messageType The type of log message ie. comment, error, etc
			\param[in] fileOnly Whether this message should not be displayed to the user in any form
			*/
			LogMessage( eMessageType messageType = Message, const Char* function = 0, UInt32 lineNumber = 0, Bool isFileOnly = false ) :
				m_dateTime( /* Defaults to now */ ),
				m_function( function ),
				m_lineNumber( lineNumber ),
				m_isFileOnly( isFileOnly ),
				m_messageType( messageType )
			{
			} 

		public:
			virtual ~LogMessage(){}

			/** \brief get the type of message 
			*/
			eMessageType type() const 
			{ return m_messageType; }

			/** \brief get the date and time of the message
			*/
			const DateTime& dateTime() const 
			{ return m_dateTime; }

			/** \brief Returns whether the message is for file only
			*/
			Bool isFileOnly() const 
			{ return m_isFileOnly; }

			/** \brief Get the function name where the message was created
			*/
			const Char* getFunctionName() const
			{ return m_function; }

			/** \brief Get the line number the message was created
			*/
			UInt32 getLineNumber() const
			{ return m_lineNumber; }

		private:
			const Char* m_function; //< Const pointer to the function name where the message was created
			Bool m_isFileOnly; ///< Whether the message shgould not be displayed to the user in the console or otherwise
			UInt32 m_lineNumber; //< The l;ine number the message was created
			DateTime m_dateTime; ///< The date and time the message was created
			eMessageType m_messageType; ///< The type of message ie. error, comment etc
		};
		
		/** \brief Message create function whihc will use the libraries new implimentation reducing cross 
		module boundary memory problems
		*/
		static LogMessage* createMessage( eMessageType messageType = Message, const Char* function = 0, UInt32 lineNumber = 0, Bool isFileOnly = false );
		
		/** \brief Base class used to attach an object to a log and receive it messages
			\remarks usefull fo a debug console etc
		*/
		class _DllExport LogListener
		{
			friend class Log;
		public:

			/** \brief Constructor optionally takes a sending log object
			*/
			LogListener();

			/** \brief Destructor automatically removes the listener from the attached log 
			*/
			virtual ~LogListener();			

			/** \brief Sets the log this listener recieves messages from
			\param[in] log The log to register with and recieve messages from
			*/
			void listenToLog( Log* log );

			/** \brief Stops recieveing messages form a log
			\param[in] log The log to unregister from
			*/
			void stopListeningToLog( Log* log );

			/** \brief Pure virtual write function must be overridden in the listener
			\remarks 
				Will be called from the thread of the log is running on. The message object will
				be removed directly after the write.
			*/
			virtual void write( const LogMessage* msg, Log* origin ) = 0;

		private:
			typedef Vector< Log* > LogVector;
			LogVector m_logs; ///< pointers to the logs this listener is connected too
		};
		

	public:

		/** \brief Constructor for a log 
		\param[in] name The name for the log which wil be used for the filename with ".log" appended
		\param[in] loggingLevel The level at + above which messages sent to log message listeners
		\param[in] alertLevel Level at which a message will be shown, also causes the write call to 
			wait until the message is written to file
		*/
		Log( const String& name, 
			eLoggingLevel loggingLevel = Lvl_All, 
			eLoggingLevel alertLevel = Lvl_Low
		);

		virtual ~Log();

		/** \brief Gets the Level at which messages are to be sent to log message listeners 
		*/
		eLoggingLevel loggingLevel() 
		{ return m_loggingLevel; }

		/** \brief Gets the level at which a message box will be displayed when a message is inserted
		\remarks
			A message that is above this level will cause write() to become a blocking function 
		*/
		eLoggingLevel alertLevel() 
		{ return m_alertLevel; }

		/** \brief Activates silent mode in which no fatal error messages will be displyed 
		*/
		void activateSilentMode() 
		{ m_silentMode = true; }

		/** \brief Re-activates showing fatal error messages
		*/
		void deactivateSilentMode() 
		{ m_silentMode = false; }

		/** \brief Adds a message to the logs message queue for writting to file 
			\return If the message level is above alert level a messagebox will be displayed
			and a return value will be given based on the users choice of action.
		*/
		eHandleAlert write( LogMessage* msg );
		
		/** \brief Polls the message queue outputting any log messages that exist 
		*/
		Bool pollMessageQueue();

		/** \brief Gets whether the log is open or not
		*/
		Bool isOpen() const
		{ return m_file.is_open(); }

		/** \brief Forces the file stream to close
		*/
		void close()
		{ m_file.close(); }

	protected:

		/** \brief \copydoc frx::Thread::threadIteration() 
		*/
		virtual Bool threadIteration();

		/** \brief Processes a message and writs its contents out to file and to any log listeners
		*/
		eHandleAlert processMessage( const LogMessage* msg );

		/** \brief Registers a listener who will be sent any log messages
		*/
		Bool addListener( LogListener* listener );

		/** \brief Removes a listener from recieveing log messages
		*/
		Bool removeListener( LogListener* listener );

	private: 

		OutFStream m_file; ///< file output stream

		typedef Vector< LogListener* > ListenerVector;
		ListenerVector m_listeners; ///< Vector of message listeners 0 only ever be a few so using a vector

		eLoggingLevel m_loggingLevel; ///< The level of logging to use, will ignore all messages below this level
		eLoggingLevel m_alertLevel; ///< The message level at which a message box or such is displayed.
		
		Bool m_silentMode; ///< In silent mode alert level messages aren't shown
		
		typedef LockFreeQueue< LogMessage* > LogMessageQueue;
		LogMessageQueue m_messageQueue; ///< Messages posted to the thread are stored in a ring buffer
	};

	/** \brief Singleton log class which will therefore be application wide and used as the default log
	*/
	class _DllExport GlobalLog : public Log, public Singleton< GlobalLog >
	{
	public:
		SINGLETON( GlobalLog );

		GlobalLog( const String& name, eLoggingLevel loggingLevel, eLoggingLevel alertLevel) :
			Log( name, loggingLevel, alertLevel )
		{}
	};

} //END: frx

#endif