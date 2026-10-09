/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\framework\KismoThread.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef _KISMO_THREAD_H
#define _KISMO_THREAD_H

#include "Framework\KismoMessageQueue.h"

namespace kismo
{
	namespace frx
	{	
		class _KismoExport WorkerMessage : public Message
		{
			KISMO_DECLARE_RTTI;
		public:
			virtual bool run() = 0; ///< The worker process method
		};

		class _KismoExport Thread : public MessageQueue
		{
		public:

			//Thread priorities
			enum ke_ThreadPriority
			{
				kc_ThreadPriority_Idle,
				kc_ThreadPriority_Minimum,
				kc_ThreadPriority_Low,		
				kc_ThreadPriority_Normal,
				kc_ThreadPriority_Medium,
				kc_ThreadPriority_High,				
				kc_ThreadPriority_Maximum,
			};

		public:
			
			//Causes the current thread to sleep for a set number of milliseconds 
			static void sleep( k_UInt32 milliseconds ); 

		public:

			Thread( const k_String& name = _T("name_undefined"), ke_ThreadPriority priority = kc_ThreadPriority_Normal );
			virtual ~Thread();

			//Starts the thread
			k_Bool start();

			//Stops the thread
			void stop();

			//Set the thread priority onces its running
			void setThreadPriority( ke_ThreadPriority priority = kc_ThreadPriority_Normal );

			//Wait for the thread to complete
			void waitForThreadTerminate( k_UInt32 time = kc_UInt32_MAX );

			//Inform the thread to terminate prematurely
			// NOTE: Will only work if the run() function checks for shouldTerminate()
			// - if the teminateTimeout is reached the thread will then be 'forced' to terminate
			// forced = Closes the thread stopping execution wherever it was in the code! NOT RECOMMENDED
			void terminateThread( k_Bool forced = false );

			///Check if the thread has finished running its process
			k_Bool isThreadFinished() const;

			//Check if the thread is running
			k_Bool isThreadRunning() const; 

			//Check whether this thread is the current one we are running in
			k_Bool isCurrentThread() const;

		protected:

			//process a message
			virtual void processMessage( Message* msg ) {};
			
			//Does the calculations of the thread
			// NOTE: Should check for shouldTerminate() in order to accept clean termination
			// - use setTerminateTimeout( TIME ) to change how long to wait before a terminate is forced
			virtual	void run() = 0;

			virtual	void onstart(){};
			virtual	void onTerminate(){};

			//Checks if the thread should terminate
			k_Bool shouldThreadTerminate();

			//Set the amount of time to wait before terminate is forced
			// This should be a time in milliseconds larger than the running loops execution time
			void setThreadTerminateTimeout( k_UInt32 milliseconds );

			/** \brief The default message handler */
			bool defaultMessageHandler( Message::Ptr& msg );

		private:

			//forward declaration
			struct Impl;
			Impl* m_impl;

			k_Handle m_threadHandle; //The threads handle (NOTE: For internal use and is not the win32 handle)

			//Static thread process distrubutes thread process to the class instance
			static k_Int32 staticThreadProcess( void* data );

		};
	} //END: frx
} //END: kismo

#endif