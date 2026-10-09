/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\Win32Platform\Win32Mutex.h
 * Status: duplicate of kismo-v0.3/Win32Platform/Win32Mutex.h
 */
#ifndef WIN32MUTEX_H
#define WIN32MUTEX_H

#include "Framework/Mutex.h"

#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <Windows.h>

namespace win32
{
	/** \brief Win32 implementation of frx::Mutex
	*/
	class Win32Mutex : public frx::Mutex
	{
	public:

		Win32Mutex( const String& name );
		~Win32Mutex();
		
		/** \brief \copydoc frx::Mutex::waitOne() */
		virtual Bool lock( UInt32 waitTimeout = WaitInfinite );

		/** \brief \copydoc frx::Mutex::release() */
		virtual Bool release();

	private:
		HANDLE m_mutexHandle; ///< The win32 handle to the mutex
	};

} //END: win32

#endif
