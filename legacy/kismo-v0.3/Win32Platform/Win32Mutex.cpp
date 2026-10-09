/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\Win32Platform\Win32Mutex.cpp
 * Status: current (newest copy of this file name in the archive)
 */
#include "Win32Mutex.h"

frx::Mutex::Ptr frx::Mutex::create( const String& name ) 
{ return frx::Mutex::Ptr( new win32::Win32Mutex( name ) ); }

namespace win32
{
	Win32Mutex::Win32Mutex( const String& name ) :
		frx::Mutex( name ),
		m_mutexHandle( 0 )
	{
		m_mutexHandle = CreateMutex( NULL, FALSE, ( getName().length() ? getName().c_str() : 0 ) );
	}

	Win32Mutex::~Win32Mutex()
	{
		release();
		CloseHandle( m_mutexHandle );
	}

	Bool Win32Mutex::lock( UInt32 waitTimeout )
	{
		DWORD ret = WaitForSingleObject( m_mutexHandle, waitTimeout );
		switch ( ret )
		{
		case WAIT_OBJECT_0: //The state of the specified object is signaled
			return true;
		case WAIT_ABANDONED: //The thread which has the mutex locked has terminated. Must abandon!
			return false;
		case WAIT_TIMEOUT: //Timeout period is reached
		case WAIT_FAILED:
		default:
			//GetLastError()
			return false;
		};

	}

	Bool Win32Mutex::release()
	{
		if ( ReleaseMutex( m_mutexHandle ) == 0 )
		{
			//GetLastError();
			return false;
		}
		return true;
	}

}; //END: win32