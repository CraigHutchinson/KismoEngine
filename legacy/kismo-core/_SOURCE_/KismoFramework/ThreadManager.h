/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\KismoCore\_SOURCE_\KismoFramework\ThreadManager.h
 * Status: current (newest copy of this file name in the archive)
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

#ifndef KISMOFRAMEWORK_THREADMANAGER_H
#define KISMOFRAMEWORK_THREADMANAGER_H

#include "KismoFramework/Singleton.h"
#include "KismoFramework/SmallSet.h"

namespace kismo
{
	//forward delcarations
	class Thread;
	class ThreadPool;

	/** \brief Manages created threads
		\remarks Can be used to stop all threads or pause some of them etc
	*/		
	class ThreadManager : public Singleton< ThreadManager >
	{
		friend class Thread;
	public:
		SINGLETON( ThreadManager );


	protected:
		/** \brief Called by the Thread constructor whenever a thread is created 
		\param[in] thread Pointer to the thread object */
		bool registerThread( Thread* thread );

		/** \brief Called by the ThreadPool constructor whenever a pool is created 
		\param[in] pool Pointer to the pool object */
		bool registerPool( ThreadPool* pool );
	private:
		

	};

} //END: kismo

#endif