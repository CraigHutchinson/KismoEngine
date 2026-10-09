/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\KismoCore\_SOURCE_\KismoParallel\Mutex.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
#include "KismoParallel/Mutex.h"
#include "KismoFramework/Timer.h"

namespace kismo 
{
	bool Mutex::tryLock( Count timeOutMilliseconds )
	{
		const SystemTimer& timer = _SystemTimer;
		timeOutMilliseconds *= timer.ticksPerSecond() / 1000; //< Convert timeout to clock ticks
		UInt32 start = timer.system_ticks();
		while ( !tryLock() )
		{
			if ( timer.system_ticks()-start > timeOutMilliseconds )
				return false;
		}
		return true;
	}

} //END: kismo