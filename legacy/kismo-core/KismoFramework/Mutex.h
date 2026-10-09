/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\KismoCore\KismoFramework\Mutex.h
 * Status: superseded by kismo-core/_SOURCE_/KismoParallel/Mutex.h (same name, content differs)
 */
/*^**************************************************************************
*	
*	Kismo Engine 
*	© 2008-2013 Craig Hutchinson (craig@craighutchinson.co.uk)
*	All right reserved
*
*	This file is subject to the terms and conditions defined in
*	file 'license.txt', which is part of this source code package.
*
***************************************************************************^*/

#ifndef KISMOFRAMEWORK_MUTEX_H
#define KISMOFRAMEWORK_MUTEX_H

#include "KismoFramework/Types.h"
#include "KismoFramework/Named.h"
#include "KismoFramework/SmartPointer.h"

namespace kismo
{
	/** \brief Mutually-exclusive handle for inter-thread commuynication
		\remarks Can be locked in one thrread which will cause another thread
		that calls wait to be suspended until the first thread calls Release
	*/
	class Mutex : public Named, public SmartReferenced
	{
	public:

		typedef SmartPointer< Mutex > Ptr;

		static const UInt32 WaitInfinite = UInt32_Max;

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
			True if the mutex was locked. If the mutex was not already locked an Assert
			will occur and False will be returned **/
		virtual Bool release() = 0;

	protected:
		Mutex( const String& name ) : Named( name ) {}
		~Mutex() {}

	private:
	};
} //END: kismo

#endif
