/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\KismoCore\_SOURCE_\KismoFramework\Profiler.cpp
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



#include "KismoFramework/Profiler.h"
#include "KismoFramework/Log.h"

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
			 Assert( i->second.ticks >= i->second.childTicks );
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
