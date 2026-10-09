/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\KismoCore\KismoParallel\Mutex.h
 * Status: duplicate of kismo-core/_SOURCE_/KismoParallel/Mutex.h
 */
#ifndef KISMOPARALLEL_MUTEX_H
#define KISMOPARALLEL_MUTEX_H

#include "KismoParallel\Common.h"

namespace kismo
{
	class Mutex
	{
	public:
		inline Mutex();
		inline ~Mutex();

		/** Lock indefinitely until the mutex becomes available
			\remarks	Multiple nested lock calls can be performed from an owning thread. 
						For each lock() there should be a coresponding release()
		*/
		inline void lock();
		
		/** Attempt to gain lock without blocking
			\returns Whether the mutex was locked
			\remarks	Multiple nested lock calls can be performed from an owning thread. 
						For each successful lock() there should be a coresponding release()
		*/
		inline bool tryLock();
		
		/** Attempt to gain lock waiting for the specified interval spinning until the lock is obtained or time-out occurs
			\returns Whether the mutex was locked
			\remarks	Multiple nested lock calls can be performed from an owning thread. 
						For each successful lock() there should be a coresponding release()
			\param[in] timeOutMilliseconds Time-out in milliseconds
		*/
		inline bool tryLock( Count timeOutMilliseconds );

		/** Release the mutex		
			\remarks	Multiple nested lock calls can be performed from an owning thread. 
						For each lock() there should be a coresponding release()
		*/
		inline void release();

	

	private:
#ifdef _DEBUG
		Count m_lockCount; //< Counter for number of succesfull locks
#endif
		MutexHandle m_handle;
	};

} //END: kismo

#if KISMO_PLATFORM == KISMO_PLATFORM_WINDOWS
#	include "KismoParallel/Mutex_Win32.inl"
#else
#error Unsupported platform for Threading::Mutex
#endif

#endif