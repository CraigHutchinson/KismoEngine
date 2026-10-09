/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\Win32Platform\Win32Thread.cpp
 * Status: current (newest copy of this file name in the archive)
 */
#include "Win32Thread.h"
#include "Framework/Common.h"
#include "Framework/Log.h"

frx::Thread::Ptr frx::Thread::create( const Char* name, frx::Thread::eThreadPriority priority )
{ return frx::Thread::Ptr(  new win32::Win32Thread( name ) ); }


//Get the win32 thread prority from the enum
Int32 getWin32ThreadPriority( frx::Thread::eThreadPriority prority )
{
	switch ( prority )
	{
	case frx::Thread::ThreadPriority_Idle:
		return THREAD_PRIORITY_IDLE;
	case frx::Thread::ThreadPriority_Minimum:
		return THREAD_PRIORITY_LOWEST;	
	case frx::Thread::ThreadPriority_Low:
		return THREAD_PRIORITY_BELOW_NORMAL;		
	case frx::Thread::ThreadPriority_Normal:
		return THREAD_PRIORITY_NORMAL;
	case frx::Thread::ThreadPriority_Medium:
		return THREAD_PRIORITY_ABOVE_NORMAL;
	case frx::Thread::ThreadPriority_High:
		return THREAD_PRIORITY_HIGHEST;				
	case frx::Thread::ThreadPriority_Maximum:
		return THREAD_PRIORITY_TIME_CRITICAL;
	}
	return THREAD_PRIORITY_NORMAL;
}

#ifdef _DEBUG
	//http://blogs.msdn.com/stevejs/archive/2005/12/19/505815.aspx
	typedef struct tagTHREADNAME_INFO
	{
	   DWORD dwType; // must be 0x1000
	   LPCSTR szName; // pointer to name (in user addr space)
	   DWORD dwThreadID; // thread ID (-1=caller thread)
	   DWORD dwFlags; // reserved for future use, must be zero
	} THREADNAME_INFO;

	//http://blogs.msdn.com/stevejs/archive/2005/12/19/505815.aspx
	// This function calls an exception which visual studio uses to set the name of the thread
	void SetThreadName( DWORD dwThreadID, LPCSTR szThreadName)
	{
	   THREADNAME_INFO info;
	   info.dwType = 0x1000;
	   info.szName = szThreadName;
	   info.dwThreadID = dwThreadID;
	   info.dwFlags = 0;

	   __try
	   {
		  RaiseException( 0x406D1388, 0, sizeof(info)/sizeof(DWORD), (ULONG_PTR*)&info );
	   }
	   __except(EXCEPTION_CONTINUE_EXECUTION)
	   {
	   }
	}
#endif

namespace win32
{		
	Win32Thread::Win32Thread( const String& name, eThreadPriority priority ) : 
		frx::Thread( name, priority ) 
	{
	}

	Win32Thread::~Win32Thread()
	{
		assert( !isThreadRunning() );
		//Failsafe stop the thread if possible
		stopThread();
	}

	Bool Win32Thread::setThreadPriorityImpl( eThreadPriority priority )
	{
		assert( frx::isValidHandle( m_threadHandle ) );

		//set the threads priority
		return SetThreadPriority( m_threadHandle, getWin32ThreadPriority( priority ) ) != 0;
	}

	Bool Win32Thread::sleepImpl( UInt32 milliseconds )
	{
		Sleep( milliseconds );
		return true;
	}

	Bool Win32Thread::startThreadImpl()
	{
		//Create the thread
		m_threadHandle = CreateThread( 0, 0, 
			(LPTHREAD_START_ROUTINE)staticThreadProcess, 
			this, 0, &m_threadID );

		if ( !frx::isValidHandle( m_threadHandle ) )
		{
			logError( "Failed to start win32 thread" );
			return false;
		}
		else
		{		
#ifdef _DEBUG
#	ifndef UNICODE
			SetThreadName( m_threadID, getName().c_str() );
#	else
			String8 str8 = getName().asString8();
			SetThreadName( m_threadID, str8.c_str() );
#	endif
#endif
			return setThreadPriorityImpl( getThreadPriority() ); //Set the thread priority
		}

	}

	Int32 Win32Thread::staticThreadProcess( void* data )
	{
		Win32Thread* thread = ((Win32Thread*)data);
		assert( isValidPointer( thread ) );

		if ( _GlobalLogPtr )
		{
			logMessage( _T("Started '") << thread->getName() << _T("' thread.") );
		}
	
		//Call the thread entry point
		((Win32Thread*)data)->threadEntry();

		if ( _GlobalLogPtr )
		{
			logMessage( _T("Finished '") << thread->getName() << _T("' thread.") );
		}
		
		return 0;
	}

	Bool Win32Thread::closeThreadImpl()
	{
		return CloseHandle( m_threadHandle ) != 0;
	}

	Bool Win32Thread::pauseThreadImpl()
	{
		return false; ///< Not implemented yet. We jsut use a sleep wihtn the thread class for now!
	}

	Bool Win32Thread::waitThreadImpl()
	{
		DWORD ret = WaitForSingleObject( m_threadHandle, INFINITE );
		return ret != WAIT_FAILED;
	}

} //END: win32

