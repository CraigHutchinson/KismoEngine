/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoFRX\ThreadManager.h
 * Status: superseded by kismo-core/_SOURCE_/KismoFramework/ThreadManager.h (same name, content differs)
 */
#ifndef KISMOFRX_THREADMANAGER_H
#define KISMOFRX_THREADMANAGER_H

#include "KismoFRX/Singleton.h"
#include "KismoFRX/SmallSet.h"

namespace kismo
{
	//forward delcarations
	class Thread;
	class ThreadPool;

	/** \brief Manages created threads
		\remarks Can be used to stop all threads or pause some of them etc
	*/		
	class KISMOFRX_EXPORT ThreadManager : public Singleton< ThreadManager >
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