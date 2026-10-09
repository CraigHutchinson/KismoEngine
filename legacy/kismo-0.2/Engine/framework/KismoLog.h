/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\framework\KismoLog.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef _KISMO_LOG_H
#define	_KISMO_LOG_H

#include "framework/KismoPlatform.h"
#include "framework/KismoSingleton.h"
#include "framework/KismoDateTime.h"
#include "framework/KismoThread.h"
#include "framework/KismoString.h"

#include <sstream>
#include <fstream>

#define LOG_HEAD _T(__FILE__) << _T('\t') << k_String::number(__LINE__) << _T('\t')
#define LOG( _lvl, _msg ) frx::Log::getSingleton().write( (frx::Log::LogMessage*)&(*(new frx::Log::LogMessage( frx::Log::_lvl )) << LOG_HEAD << _msg) );
#define LOGWAIT( _lvl, _msg ) frx::Log::getSingleton().write( (frx::Log::LogMessage*)frx::Thread::wait( (frx::Log::LogMessage*)&( *(new frx::Log::LogMessage( frx::Log::_lvl )) << LOG_HEAD << _msg) ) );

#define LOG_COMMENT( msg ) LOG( kc_Comment, msg );
#define LOG_MESSAGE( msg ) LOG( kc_Message, msg );
#define LOG_SUCCESS( msg ) LOG( kc_Success, msg );
#define LOG_WARNING( msg ) LOG( kc_Warning, msg );
#define LOG_RECOVERABLE( msg ) LOG( kc_RecoverableError, msg );
#define LOG_ERROR( msg ) LOGWAIT( kc_Error, msg );
#define LOG_CRITICAL( msg ) LOGWAIT( kc_CriticalError, msg );

namespace kismo
{
	namespace frx
	{		
		//-----------------------------------------------------
		// Logging & error class
		//-----------------------------------------------------
		class _KismoExport Log : public Thread, public Singleton< Log >
		{
			//friend class LogListener; //Log listener needs access to register itself as a listener

		public:

			enum ke_MessageType
			{
				//(Highlighted grey - when colours used ;-) )
				kc_Profiler = -2, //A profiler output - e.g. function timings
				kc_Comment, //A non important message - e.g. debugging output

				//(Highlighted white - when colours used ;-) )
				kc_Message, //Status messages for normal program output, GENERAL
				
				//(Highlighted green - when colours used ;-) )
				kc_Success, //Success message for completion of a task etc.. 

				//(Highlighted amber - when colours used ;-) )
				kc_Warning, //Something isn't exactly correct such as an invalid value/paramater		
				kc_RecoverableError, //Non critical warning which will maintain normal program flow

				//(Highlighted red - when colours used ;-) )
				kc_Error, //Error which will cause the current function to terminate early with recuring effects later
				kc_CriticalError, //Critical error which will cause the application to close
			};

			enum ke_LoggingLevel
			{
				kc_VeryLow = kc_CriticalError,
				kc_Low = kc_Error,
				kc_Medium = kc_Warning,
				kc_Normal = kc_Message,
				kc_All = kc_Comment,
			};

			
			//class _KismoExport LogListener //Base class for any class that wants to also recieve log messages
			//{
			//	friend class Log; //Log needs access to the write method
			//public:
			//	LogListener( ke_LoggingLevel loggingLevel ) : m_loggingLevel( loggingLevel )
			//	{ Log::registerListener( this ); }
			//protected:
			//	//virtual write function called by Log
			//	virtual void write( const k_String& message, Log::ke_messageType type ) = 0;
			//	//The level of logging to use, can ignore all messages below this level
			//	ke_LoggingLevel m_loggingLevel; 
			//};
			
			class _KismoExport LogMessage : public Message
			{
				friend class Log; ///< Give Log access to the internal string
				KISMO_DECLARE_RTTI;
			public:
				
				LogMessage( const ke_MessageType messageType = kc_Message, k_Bool fileOnly = kc_FALSE ) : 
					Message(),
					m_time(),
					m_fileOnly( fileOnly ),
					m_messageType( messageType )
				{} 
				virtual ~LogMessage(){}

				ke_MessageType type() const 
				{ return m_messageType; }

				const k_String& string() const
				{ return m_string; }

				//template < class _T >
				//LogMessage& operator << ( const _T& d )
				//{ m_string << d; return *this; }		

				//LogMessage& operator << ( const k_Char* d )
				//{ m_string << d; return *this; }	

				template < class _T >
				friend LogMessage& operator << ( LogMessage& o, const _T& d )
				{ o.m_string << d; return o; }

			protected:
				DateTime m_time;
				k_Bool m_fileOnly;
				ke_MessageType m_messageType;
				k_String m_string;
			};

		public:

			static Log& getSingleton( void );
			static Log* getSingletonPtr( void );	

			Log( const k_String& filename, 
				ke_LoggingLevel loggingLevel = kc_Normal, //The level at + above which messages are actually logged
				ke_LoggingLevel alertLevel = kc_VeryLow, //Level at which a message will be shown
				k_Bool useFile = true 
			);

			virtual ~Log();

			//Get the logging level
			ke_LoggingLevel loggingLevel() { return m_loggingLevel; }
			ke_LoggingLevel alertLevel() { return m_alertLevel; }

			//Silent mode
			void activateSilentMode() { m_silentMode = kc_TRUE; }
			void deactivateSilentMode() { m_silentMode = kc_FALSE; }

			void write( LogMessage* msg );

		protected:


			virtual void run();

			//process a message
			virtual void processMessage( Message* msg );

			//virtual	void onClosed(){ run(); } //clear the message queue on destruction

   		protected: 

			k_OFStream m_file; //file output stream

			ke_LoggingLevel m_loggingLevel; //The level of logging to use, will ignore all messages below this level
			ke_LoggingLevel m_alertLevel; //The message level at which a message box or such is displayed.
			
			k_Bool m_silentMode; //In silent mode alert level messages aren't shown

			k_Vector< k_OStream* > m_outStreams;
		};

	} //END: frx

} //END: kismo

#endif