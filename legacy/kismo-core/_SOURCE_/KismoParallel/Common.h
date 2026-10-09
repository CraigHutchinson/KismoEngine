/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\KismoCore\_SOURCE_\KismoParallel\Common.h
 * Status: superseded by kismo-core/_SOURCE_/KismoGL/Common.h (same name, content differs)
 */
#ifndef KISMOPARALLEL_COMMON_H
#define KISMOPARALLEL_COMMON_H

#include "KismoFramework/Platform.h"

namespace kismo 
{
	
#if KISMO_PLATFORM == KISMO_PLATFORM_WINDOWS
	#define WIN32_LEAN_AND_MEAN  //Thin windows headers a bit
	#include <windows.h>
	#undef WIN32_LEAN_AND_MEAN //< Undefine as other code may want full windows
	typedef CRITICAL_SECTION MutexHandle;
	typedef HANDLE ThreadHandle;
#elif KISMO_PLATFORM == KISMO_PLATFORM_LINUX
	#include <pthread.h> //, Unix/Linux pthreads
	typedef pthread_mutex_t MutexHandle;
#endif

} //END: kismo


#endif