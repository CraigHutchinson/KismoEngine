/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\KismoCore\_SOURCE_\KismoParallel\Mutex_Win32.inl
 * Status: unique (no other copy of this file name in the archive)
 */
//#include "Mutex.h"
#include "KismoFramework\Common.h"

namespace kismo 
{
	//http://en.wikipedia.org/wiki/Critical_section
	Mutex::Mutex()  
#ifdef _DEBUG
		:
		m_lockCount(0)
#endif
	{
		/* Initialize the critical section before entering multi-threaded context. */
		InitializeCriticalSection(&m_handle);
	}

	Mutex::~Mutex()
	{
#ifdef _DEBUG
		Assert( m_lockCount==0 );
#endif
		DeleteCriticalSection(&m_handle);
	}

	void Mutex::lock()
	{
		/* Enter the critical section -- other threads are locked out */
		EnterCriticalSection(&m_handle);
#ifdef _DEBUG
		++m_lockCount;
#endif
	}

	bool Mutex::tryLock()
	{
		/* Enter the critical section -- other threads are locked out */
		bool ret = TryEnterCriticalSection(&m_handle) != 0;
		
#ifdef _DEBUG
		if ( ret )
			++m_lockCount;
#endif
		return ret;
	}

	void Mutex::release()
	{   
		/* Leave the critical section -- other threads can now EnterCriticalSection() */
#ifdef _DEBUG
		AssertEx( m_lockCount != 0, "Attempt to release mutex that has not been locked!" );
		--m_lockCount;
#endif
		LeaveCriticalSection(&m_handle);
	}

} //END: kismo