/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\Win32Platform\Win32Thread.h
 * Status: duplicate of kismo-v0.3/Win32Platform/Win32Thread.h
 */
#ifndef WIN32THREAD_H
#define WIN32THREAD_H

#include "Framework/Thread.h"

#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <windows.h>

namespace win32
{
	/** \brief Win32 implementation of frx::Thread
	*/
	class Win32Thread : public frx::Thread
	{
	public:
		/** \brief Thread constructor
			\param[in] name Unique name used to identity this thread
			\param[in] priority Specifies the threads priority enumerator
		*/
		Win32Thread( const String& name, eThreadPriority priority = ThreadPriority_Normal );

		/** \brief Destructor
			\remarks Will assert if the thread process has not been terminated before the destructr is called 
		*/
		~Win32Thread();

		virtual Bool setThreadPriorityImpl( eThreadPriority priority );

		/** \brief Virtual function should cause the callign thread to sleep for the respective 
			number of milliiseconds
			\remarks Used by Threaded objects attached to this thread so that they can cause execution to stall if required
		*/
		virtual Bool sleepImpl( UInt32 milliseconds );

		/** \brief Virtual function should start the thread
		*/
		virtual Bool startThreadImpl();

		/** \brief Virtual function should terminate the thread
		*/
		virtual Bool closeThreadImpl();

		/** \brief  Virtual function should Pause execution of the thread so stopping any execution at this time
		*/
		virtual Bool pauseThreadImpl();

		/** \brief  Virtual function should Wait for the thread to finish executing.
		*/
		virtual Bool waitThreadImpl();

	protected:
		
		/** \brief Static thread process distrubutes thread process to the class instance
		*/
		static Int32 staticThreadProcess( void* data );

	private:
		HANDLE m_threadHandle; ///< The win32 thread handle
		DWORD m_threadID; //Thread ID
	};

} //END: frx

#endif