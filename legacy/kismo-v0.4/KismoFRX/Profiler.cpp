/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoFRX\Profiler.cpp
 * Status: superseded by kismo-core/_SOURCE_/KismoFramework/Profiler.cpp (same name, content differs)
 */
#include "KismoFRX/Profiler.h"
#include "KismoFRX/Log.h"

namespace kismo
{
	IMPL_SINGLETON( Profiler );

	Profiler::Timer* Profiler::m_currentTimer = 0;

	void Profiler::writeTimings( Log& log ) const
	{
		_Log << Log::Profile << 
			_T("\n Calls \t Time(Sec) \t Average(sec) \t Total-In-Child(Sec) \t Total(Sec) \t Total-Average(sec) \t Name/Function\n"); 
			
		 for( StatMap::const_iterator i = m_timingResults.begin(); i != m_timingResults.end(); ++i )
		 {
			 assert( i->second.ticks >= i->second.childTicks );
			 _Log << i->second.calls << _T('\t') 
				 << (Float32)(i->second.ticks - i->second.childTicks)  / _SystemTimer.ticksPerSecond() << _T('\t') 
				 << (Float32)(i->second.ticks - i->second.childTicks) / i->second.calls / _SystemTimer.ticksPerSecond() << _T('\t') 
				 << (Float32)i->second.childTicks  / _SystemTimer.ticksPerSecond() << _T('\t') 
				 << (Float32)i->second.ticks / _SystemTimer.ticksPerSecond() << _T('\t') 
				 << (Float32)i->second.ticks / i->second.calls / _SystemTimer.ticksPerSecond() << _T('\t') 
				 << i->first << _T('\n');
		 }
		_Log << Log::Flush; //< /write the message
	}

} //END: kismo
