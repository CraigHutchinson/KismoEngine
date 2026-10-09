/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\framework\KismoTimer.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
#include "KismoTimer.h"

//For the performance counter
#include <windows.h>

//for timeGetTime
#pragma comment(lib, "winmm.lib")
#include <mmsystem.h>

namespace kismo
{
	namespace frx
	{	
		template<> const SystemTimer* Singleton< const SystemTimer >::ms_instance = 0;
		const SystemTimer& SystemTimer::getSingleton( void ) 
		{ 
			assert( isValidPointer( ms_instance ) ); return ( *ms_instance ); 
		}
		const SystemTimer* SystemTimer::getSingletonPtr( void ) { return ms_instance;  }

		SystemTimer::SystemTimer()
		{
			k_Int64 ticksPerSecond;
			//query the system for the high resolution performance counter
			if ( (m_usePerformanceCounter = (QueryPerformanceFrequency( (LARGE_INTEGER *)&ticksPerSecond ) != 0 )) )
				m_ticksPerSecond = (k_UInt32)ticksPerSecond;
			else
				m_ticksPerSecond = 1000; //Set the number of ticks in a second to 1000 for the system timer (TimeGetTime)
			m_startTime = system_ticks();
		}

		k_Int64 SystemTimer::system_ticks() const
		{		
			if ( m_usePerformanceCounter )
			{
				k_Int64 time;
				QueryPerformanceCounter( (LARGE_INTEGER *)&time );
				return time;
			}
			else
			{			
				return (k_Int64)timeGetTime();
			}
		}





		Timer::Timer() :
			m_isPaused( kc_TRUE ), //Start the timer in the paused state
			m_timerStartTime( 0 ),
			m_frameStartTime( 0 ),
			m_simulationTimeScale( 1.0f ),
			m_simulationTime( 0 )
		{
			k_UInt64 frequency;

			//query the system for the high resolution performance counter
			m_usePerformanceCounter = (QueryPerformanceFrequency( (LARGE_INTEGER *) &frequency) != 0);
			
			if ( m_usePerformanceCounter )
				m_ticksPerSecond = frequency; //Set the number of ticks in a second to the timer frequency
			else
				m_ticksPerSecond = 1000; //Set the number of ticks in a second to 1000 for the system timer

			m_timerStartTime = getSystemTime();
			m_lastGetElapsedTime = m_timerStartTime;
		}

		void Timer::startFrame()
		{	
			k_UInt32 systemTime = getSystemTime();//get the new system time
			m_frameTimeElapsed = systemTime - m_frameStartTime;//get the elapsed system time
			m_frameStartTime = systemTime; //Update the new frame start time
			
			if ( !m_isPaused )
			{
				m_frameSimulationTimeElapsed = (k_Float32)m_frameTimeElapsed * m_simulationTimeScale;
				m_simulationTime += m_frameSimulationTimeElapsed; //Incement simulation time
			}
		}

		void Timer::reset()
		{
			m_simulationTime = 0;
		}

		void Timer::resetAll()
		{
			reset(); //Reset the simulation timer
			m_timerStartTime = getSystemTime();
		}

		k_UInt32 Timer::getSystemTime() const
		{
			if ( m_usePerformanceCounter )
			{
				k_UInt64 time;

				QueryPerformanceCounter((LARGE_INTEGER *) &time);

				return (k_UInt32)( ((k_Float32)time * 1000.0f) / m_ticksPerSecond);
			}
			else
			{
				return (k_UInt32)timeGetTime();
			}
		}

		k_UInt32 Timer::getElapsedTime()
		{
			k_UInt32 systemTime = getSystemTime();
			k_UInt32 elapsedTime = systemTime - m_lastGetElapsedTime;
			m_lastGetElapsedTime = systemTime;

			return elapsedTime;
		}
	}
}