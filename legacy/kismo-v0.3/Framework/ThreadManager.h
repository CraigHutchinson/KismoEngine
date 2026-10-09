/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\Framework\ThreadManager.h
 * Status: superseded by kismo-core/_SOURCE_/KismoFramework/ThreadManager.h (same name, content differs)
 */
#ifndef THREADMANAGER_H
#define THREADMANAGER_H

#include "Framework/Singleton.h"
#include "Platform/Platform.h"
#include "Framework/SmallSet.h"

namespace frx
{
	//forward delcarations
	class Thread;

	/** \brief Manages created threads
		\remarks Can be used to stop all threads or pause some of them etc
	*/		
	class _DllExport ThreadManager : public Singleton< ThreadManager >
	{
		friend class Thread;
	public:
		SINGLETON( ThreadManager );


	protected:
		/** \brief Called by the Thread constructor whenever a thread is created 
		\param[in] thread Pointer to the thread object */
		bool registerThread( Thread* thread );

	private:
		

	};

} //END: frx

#endif