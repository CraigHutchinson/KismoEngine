/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\Framework\Profiler.cpp
 * Status: superseded by kismo-core/_SOURCE_/KismoFramework/Profiler.cpp (same name, content differs)
 */
#include "Framework/Profiler.h"
#include "Framework/Log.h"

namespace frx
{
	IMPL_SINGLETON( Profiler );

	Profiler::Timer* Profiler::m_currentTimer = 0;

	void Profiler::writeTimings( Log& log ) const
	{
		Log::LogMessage* msg = Log::createMessage( Log::ProfilerMessage );
		 (*msg) << "\n Calls \t Time(Sec) \t Average(sec) \t Total-In-Child(Sec) \t Total(Sec) \t Total-Average(sec) \t Name/Function\n";
		 for( StatMap::const_iterator i = m_timingResults.begin(); i != m_timingResults.end(); ++i )
		 {
			 assert( i->second.ticks >= i->second.childTicks );
			 (*msg) << i->second.calls << '\t' 
				 << (Float32)(i->second.ticks - i->second.childTicks)  / _SystemTimer.ticksPerSecond() << '\t'
				 << (Float32)(i->second.ticks - i->second.childTicks) / i->second.calls / _SystemTimer.ticksPerSecond() << '\t'
				 << (Float32)i->second.childTicks  / _SystemTimer.ticksPerSecond() << '\t'
				 << (Float32)i->second.ticks / _SystemTimer.ticksPerSecond() << '\t'
				 << (Float32)i->second.ticks / i->second.calls / _SystemTimer.ticksPerSecond() << '\t'
				 << i->first << '\n';
		 }
		 log.write( msg ); //write the message
	}

} //END: frx
