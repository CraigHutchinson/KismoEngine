/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\Framework\Mutex.h
 * Status: superseded by kismo-core/_SOURCE_/KismoParallel/Mutex.h (same name, content differs)
 */
#ifndef MUTEX_H
#define MUTEX_H

#include "Framework/Types.h"
#include "Framework/Named.h"
#include "Framework/SmartPointer.h"

namespace frx
{
	/** \brief Mutually-exclusive handle for inter-thread commuynication
		\remarks Can be locked in one thrread which will cause another thread
		that calls wait to be suspended until the first thread calls Release
	*/
	class _DllExport Mutex : public Named, public SmartReferenced
	{
	public:

		typedef SmartPointer< Mutex > Ptr;

		static const UInt32 WaitInfinite = UInt32_MAX;

		/** \brief Styatic create function created the platform specific implementation fo the mutex
		*/
		static Ptr create( const String& name = String() );

	public:
		
		/** \brief Wait for the excusive mutex to become signalled or the timeout interval elapses.
		\param[in] waitTimeout The number of milliseconds to wait for the lock to become available.
		\remarks Setting timeout to WaitInfinite will cause a wait until the mutex is detroyed or released 
		\return
			True if the wait succeeded before the timeout occured. False if the mutex has been closed
			of the timeout period is reached and the mutex could not be made exlclusive
		*/
		virtual Bool lock( UInt32 waitTimeout = WaitInfinite ) = 0;

		/** \brief Releases the mutex
		\return
			True if the mutex was locked. If the mutex was not already locked an assert
			will occur and False will be returned **/
		virtual Bool release() = 0;

	protected:
		Mutex( const String& name ) : Named( name ) {}
		~Mutex() {}

	private:
	};
} //END: frx

#endif
