/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\framework\KismoThread.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
#include "KismoThread.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "kismoLog.h"

namespace kismo
{
	namespace frx
	{		
		KISMO_IMPLEMENT_RTTI( thread, WorkerMessage, frx::Message );

		//typedef for thread handles
		typedef void* THREADHANDLE; 

		//The default terminate timeout (Half a second)
		const k_UInt32 kc_DefaultTerminateTimeout = 500;

		//Thread implimentation data hidden from the interface
		struct Thread::Impl
		{
			k_String name; //Name of the thread
			THREADHANDLE handle; //The win32 thread handle		
			Thread::ke_ThreadPriority priority; //The threads priority
			k_Bool shouldTerminate; //Whether the running thread should terminate at the next chance
			k_Bool isThreadRunning; //Whether the thread is running
			k_UInt32 terminateTimeout; //Milliseconds to wait before automatically forcing a terminate
			DWORD threadID; //Thread ID
		};
	

		//Get the win32 thread prority from the enum
		k_Int32 getWin32ThreadPriority( Thread::ke_ThreadPriority prority )
		{
			switch ( prority )
			{
			case Thread::kc_ThreadPriority_Idle:
				return THREAD_PRIORITY_IDLE;
			case Thread::kc_ThreadPriority_Minimum:
				return THREAD_PRIORITY_LOWEST;	
			case Thread::kc_ThreadPriority_Low:
				return THREAD_PRIORITY_BELOW_NORMAL;		
			case Thread::kc_ThreadPriority_Normal:
				return THREAD_PRIORITY_NORMAL;
			case Thread::kc_ThreadPriority_Medium:
				return THREAD_PRIORITY_ABOVE_NORMAL;
			case Thread::kc_ThreadPriority_High:
				return THREAD_PRIORITY_HIGHEST;				
			case Thread::kc_ThreadPriority_Maximum:
				return THREAD_PRIORITY_TIME_CRITICAL;
			}
			
			return THREAD_PRIORITY_NORMAL;
		}

		//Static thread process distrubutes thread process to the class instances run method
		k_Int32 Thread::staticThreadProcess( void* data )
		{
			Impl* impl = ((Thread*)data)->m_impl;
			assert( isValidPointer( data ) );
			assert( impl->isThreadRunning );

			((Thread*)data)->onstart();

			LOG_MESSAGE( _T("Started '") << impl->name << _T("' thread.") );
			((Thread*)data)->run();
			LOG_MESSAGE( _T("Finished '") << impl->name << _T("' thread.") );
			
			((Thread*)data)->onTerminate();

			impl->isThreadRunning = kc_FALSE;
			CloseHandle( impl->handle );
			impl->handle = 0;

			return 0;
		}

		Thread::Thread( const k_String& name, ke_ThreadPriority priority )
		{
			m_impl = new Impl;
			m_impl->name = name;
			m_impl->handle = 0;
			m_impl->priority = priority;
			m_impl->shouldTerminate = kc_FALSE;
			m_impl->isThreadRunning = kc_FALSE;	
			m_impl->terminateTimeout = kc_DefaultTerminateTimeout; //Set the default terminate timeout
			
			//LOG_MESSAGE( "Created '" << m_impl->name << "' thread." );
		}
		
		Thread::~Thread()
		{
			terminateThread( true );

			assert( !m_impl->isThreadRunning );
			//TODO: Can't assure thread has stopped as the virtual function table 
			//will be destroid and therefore running must be stopped before this point!!!!
			
			safeDelete( m_impl ); //free memory if the thread didn't delete itself
		}

		//Causes the current thread to sleep for a set number of milliseconds 
		void Thread::sleep( k_UInt32 milliseconds )
		{
			Sleep( milliseconds );
		}


		//Starts the thread with a specified priority
		k_Bool Thread::start()
		{
			if ( isThreadRunning() )
			{
				LOG_WARNING( _T("Thread already running!!") );
				return kc_FALSE;
			}

			//Store the threads infomation			
			m_impl->handle = 0;
			m_impl->shouldTerminate = kc_FALSE;
			m_impl->isThreadRunning = kc_TRUE;

			//Create the thread
			HANDLE threadHandle = CreateThread( 0, 0, 
				(LPTHREAD_START_ROUTINE)staticThreadProcess, 
				this, 0, &m_impl->threadID );

			if ( !isValidHandle( threadHandle ) )
				return kc_FALSE;
			else
			{
				m_impl->handle = threadHandle;//Update the thread information
				setThreadPriority( m_impl->priority ); //Set the thread priority
			}

			return kc_TRUE;		
		}
		
		//Stops the thread
		void Thread::stop()
		{
			terminateThread();

			if ( !isCurrentThread() )
				if ( isThreadRunning() )
					waitForThreadTerminate();
				else
					pollMessageQueue(); //make sure all messages are processed
		}

		//Set the thread priority onces its running
		void Thread::setThreadPriority( ke_ThreadPriority priority )
		{
			//Store the priority
			m_impl->priority = priority; 

			if ( isValidHandle( m_impl->handle ) )
			{
				//set the threads priority
				SetThreadPriority( m_impl->handle, getWin32ThreadPriority( priority ) );
			}
		}

		//Wait for the thread to complete
		void Thread::waitForThreadTerminate( k_UInt32 time )
		{
			if ( m_impl->isThreadRunning && isValidHandle( m_impl->handle ) )
				DWORD ret = WaitForSingleObject( m_impl->handle, time );

		}

		//Inform the thread to terminate prematurely
		// NOTE: Will only work if the run() function checks for shouldTerminate()
		// - if the teminateTimeout is reached the thread will then be 'forced' to terminate
		// forced = Closes the thread stopping execution wherever it was in the code! NOT RECOMMENDED
		void Thread::terminateThread( k_Bool forced )
		{ 
			m_impl->shouldTerminate = kc_TRUE;

			if ( forced )
			{
				CloseHandle( m_impl->handle );
				m_impl->handle = 0;
			}
		}

		///Check if the thread has finished running its process
		k_Bool Thread::isThreadFinished() const
		{
			return !isThreadRunning();
		}

		//Check if the thread is running
		k_Bool Thread::isThreadRunning() const
		{
			return m_impl->isThreadRunning;
		} 

		//Checks if the thread should terminate
		k_Bool Thread::shouldThreadTerminate() 
		{
			return m_impl->shouldTerminate; 
		}

		//Set the amount of time to wait before terminate is forced
		// This should be a time in milliseconds larger than the running loops execution time
		void Thread::setThreadTerminateTimeout( k_UInt32 milliseconds ) 
		{  
			m_impl->terminateTimeout = milliseconds; 
		}

		//Check whether this thread is the current one we are running in
		k_Bool Thread::isCurrentThread() const
		{
			if ( !isThreadRunning() )
				return true; //If this thread isn't running then we can/could just say this is now current!
			else
				return m_impl->threadID == GetCurrentThreadId();
		}

		
		bool Thread::defaultMessageHandler( Message::Ptr& msg )
		{
			//TODO: Terminate message

			if ( msg->isDerived( WorkerMessage::TYPE ) )
				return ((WorkerMessage*)msg.getPtr())->run();

			return false;
		}

	} //END: frx
} //END: kismo