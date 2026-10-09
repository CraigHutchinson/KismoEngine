/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\framework\KismoProfiler.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
#include "KismoProfiler.h"

namespace kismo
{
	namespace frx
	{			
		template<> Profiler* Singleton< Profiler >::ms_instance = 0;
		Profiler& Profiler::getSingleton( void ) { assert( isValidPointer( ms_instance ) ); return ( *ms_instance ); }
		Profiler* Profiler::getSingletonPtr( void ) { return ms_instance;  }

		Profiler::Profiler() 
		{
			//Add a root 'application' element to the stack which defines the profilers whole lifetime
			m_runningTimers.push( new STimer( getSystemTime(), _T("App"), 0 ) );
		}

		//Pushes a new timer onto the stack
		void Profiler::pushTimer( k_String sectionName )
		{
			STimer* newTimer = new STimer( getSystemTime(), sectionName, m_runningTimers.top() );
			m_runningTimers.push( newTimer );
			
			m_timingResults.push_back( newTimer ); //Also add it to the timing results so the starting is logged

		}

		//pops the last 
		void Profiler::popTimer()
		{
			assert( m_runningTimers.size() >= 1 ); //Make sure the 1 base ('app') timer always stays on the stack

			STimer* timer = m_runningTimers.top(); //Get the last timer on the stack
			
			timer->time = getSystemTime() - timer->time; //Calculate the timers elapsed time

			m_timingResults.push_back( timer ); //Store the timing in the results list

			m_runningTimers.pop(); //Remove the timer from the stack
		}

	}
}
