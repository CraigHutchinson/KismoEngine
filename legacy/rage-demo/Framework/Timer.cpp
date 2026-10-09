/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\Framework\Timer.cpp
 * Status: superseded by kismo-core/_SOURCE_/KismoFramework/Timer.cpp (same name, content differs)
 */
#include "Framework/Timer.h"
#include "Framework/Log.h"

#ifdef _MSC_VER
#	include <windows.h>//For the performance counter
//for timeGetTime
#	pragma comment(lib, "winmm.lib")
#	include <mmsystem.h>
#	define SYSTEM_TICKS 1000
#	define PERF_COUNTERS
//#	include <SDL/SDL.h>
#else
#	include <sys/time.h>
#	include <SDL/SDL.h>
#	define SYSTEM_TICKS 1000
#endif


frx::SystemTimer g_systemTimer; ///< The system timer singleton

namespace frx
{	
	template<> const SystemTimer* Singleton< const SystemTimer >::ms_instance = 0;
	const SystemTimer& SystemTimer::getSingleton( void ) 
	{ 
		assert( ms_instance ); return ( *ms_instance ); 
	}
	const SystemTimer* SystemTimer::getSingletonPtr( void ) { return ms_instance;  }

	SystemTimer::SystemTimer() :
		m_tickShift( 0 ),
		m_ticksPerSecond( 0 ),
		m_startTime( 0 ),
		m_usePerformanceCounter( 0 )	
	{
#ifdef PERF_COUNTERS
		Int64 ticksPerSecond;
		//query the system for the high resolution performance counter
		if ( (m_usePerformanceCounter = (QueryPerformanceFrequency( (LARGE_INTEGER *)&ticksPerSecond ) != 0 )) )
		{
			//Due to the insane timer accurace we reduce the ticks per second a bit
			UInt32 tickLimit = 10000;
			while ( (ticksPerSecond >> m_tickShift) > tickLimit )
				++m_tickShift;

			m_ticksPerSecond = (UInt32)(ticksPerSecond >> m_tickShift);
		}
		else
			m_ticksPerSecond = SYSTEM_TICKS; //Set the number of ticks in a second to 1000 for the system timer (TimeGetTime)
#else
		m_usePerformanceCounter = true;
		m_ticksPerSecond = SYSTEM_TICKS;

		if( SDL_Init( SDL_INIT_EVERYTHING /*SDL_INIT_TIMER | SDL_INIT_EVENTTHREAD*/ ) < 0 ) 
		{
			logRecoverable( "Error initialising SDL timer. " << SDL_GetError() );
		}
#endif

		m_startTime = system_ticks();
	}

	UInt32 SystemTimer::system_ticks() const
	{		
#ifdef PERF_COUNTERS
		if ( m_usePerformanceCounter )
		{
			Int64 time;
			QueryPerformanceCounter( (LARGE_INTEGER *)&time );
			return (UInt32)(time >> m_tickShift);
		}
		else
		{			
			return timeGetTime();
		}
#else
		return SDL_GetTicks();
#endif
	}



	FrameTimer::FrameTimer() :
		SimulationTimer(),			
		m_timerStartTimeStamp( 0 ),
		m_lastFrameTimeStamp( 0 ),
		m_lastFpsTimeStamp( 0 ),
		m_frameTime( 0 ),
		m_currentFps( 0 ),
		m_frames( 0 ),
		m_totalFrames( 0 )
	{
		start();
	}

	FrameTimer::~FrameTimer()
	{
		Float32 average = (Float32)m_totalFrames / (Float32)(simulationTime_secs() - m_timerStartTimeStamp);
		if ( _GlobalLogPtr )
		{
			logComment( "Average-Fps = " << average );
		}
	}

	void FrameTimer::start()
	{
		SimulationTimer::start();
		m_timerStartTimeStamp = simulationTime_secs();
		m_lastFpsTimeStamp = m_timerStartTimeStamp;
		m_lastFrameTimeStamp = m_timerStartTimeStamp;
		m_frameTime = 0.0f; ///< The frame time
		m_currentFps = 0;
		m_frames = 0;
		m_totalFrames = 0;
	}

	Real FrameTimer::endFrame()
	{
		++m_frames;

		Real currentTime = simulationTime_secs();
		m_frameTime = currentTime - m_lastFrameTimeStamp;

		m_lastFrameTimeStamp = currentTime;

		Real fpsElapsed = currentTime - m_lastFpsTimeStamp;
		if ( fpsElapsed > (Real)0.25f )
		{
			m_currentFps = m_frames * ((Real)1.0 / fpsElapsed);
			m_totalFrames += m_frames;
			m_frames = 0;
			m_lastFpsTimeStamp = currentTime;
		
			logComment(  _T("\tFps\t") << m_currentFps << '\t' << currentTime );
		}


		return m_frameTime;
	}

} //END: frx
