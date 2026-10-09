/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\KismoCore\_SOURCE_\KismoFramework\Timer.cpp
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



#include "KismoFramework/Timer.h"
#include "KismoFramework/Log.h"

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


kismo::SystemTimer g_systemTimer; ///< The system timer singleton

namespace kismo
{	
	//template<> const SystemTimer* Singleton< const SystemTimer >::ms_singleton = 0;
	//const SystemTimer& SystemTimer::getSingleton( void ) 
	//{ 
	//	Assert( ms_singleton ); return ( *ms_singleton ); 
	//}
	//const SystemTimer* SystemTimer::getSingletonPtr( void ) { return ms_singleton;  }

	SystemTimer::SystemTimer() :
		m_tickShift( 0 ),
		m_ticksPerSecond( 0 ),
		m_startTime( 0 ),
		m_usePerformanceCounter( false )	
	{
#ifdef PERF_COUNTERS
		m_usePerformanceCounter = true;
		Int64 ticksPerSecond;
		//query the system for the high resolution performance counter
		if ( (m_usePerformanceCounter = (QueryPerformanceFrequency( (LARGE_INTEGER *)&ticksPerSecond ) != 0 )) )
		{
			//Due to the insane timer accurace we reduce the ticks per second a bit
			//TODO: this seems a bit silly now! Should probably remove this
			UInt32 tickLimit = Int16_Max; ///< This is the number fo ticks per second we aim to maintain any higher is reduced
			while ( (ticksPerSecond >> m_tickShift) > tickLimit )
				++m_tickShift;

			m_ticksPerSecond = (UInt32)(ticksPerSecond >> m_tickShift);
			Assert( m_ticksPerSecond >= 10000 ); //, make sure we have maintained a good accuracy!
		}
		else
			m_ticksPerSecond = SYSTEM_TICKS; //Set the number of ticks in a second to 1000 for the system timer (TimeGetTime)
#else
		m_ticksPerSecond = SYSTEM_TICKS;

		if( SDL_Init( SDL_INIT_EVERYTHING /*SDL_INIT_TIMER | SDL_INIT_EVENTTHREAD*/ ) < 0 ) 
		{
			logRecoverable( "Error initialising SDL timer. " << SDL_GetError() );
		}
#endif

		m_invTicksPerSecond = Real(1.0 / m_ticksPerSecond);
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

	Timer::Timer() :
		m_startTime( _SystemTimer.system_ticks() ), 
		m_running( true )
	{
	}	

	void Timer::start()
	{
		m_startTime = _SystemTimer.system_ticks(); 
		m_running = true;
	}

	void Timer::stop()
	{
		m_startTime = time_ticks();//_SystemTimer.system_ticks() - m_startTime; 
		m_running = false;
	}

	SimulationTimer::SimulationTimer() :
		m_simulationTimeScale( (Real)1.0f ),
		m_lastSimTimeStamp( (Real)0.0f ), m_baseSimTime( (Real)0.0f ), m_running( false )
	{}

	Real SimulationTimer::simulationTime_secs() const 
	{ 
		if ( m_running )					
			return m_baseSimTime + (Real)(_SystemTimer.runningTime_secs() - m_lastSimTimeStamp) * m_simulationTimeScale;
		else
			return m_baseSimTime; //Return the time at which the clock was paused
	}
	
	void SimulationTimer::start()
	{
		if ( m_running ) return; //Do Nothing as to stop start() moving time back to the baseSimTime
		m_lastSimTimeStamp = _SystemTimer.runningTime_secs(); //Start timing from now
		m_running = true;
	}

	void SimulationTimer::pause()
	{
		m_baseSimTime = simulationTime_secs(); //Store the current simulation time
		m_running = false;				
	}

	void SimulationTimer::stop()
	{
		m_running = false;				
		reset(); //Reset the simulation time
	}
	
	void SimulationTimer::setSimulationTimeSecs( Real seconds )
	{
		m_baseSimTime = seconds;
		m_lastSimTimeStamp = _SystemTimer.runningTime_secs(); //Start timing from now
	}

	void SimulationTimer::setSimulationTimeScale( Real scaleFactor )
	{
		m_baseSimTime = simulationTime_secs(); //Store the current simulation time
		m_lastSimTimeStamp = _SystemTimer.runningTime_secs(); //Store the system time stamp
		m_simulationTimeScale = scaleFactor; //set the new simulation scale
	}
	

	FrameTimer::FrameTimer() :	
		m_lastFrameTimeStamp( 0 ),
		m_lastFpsTimeStamp( 0 ),
		m_frameDeltaTime( 0 ),
		m_currentFps( 0 ),
		m_frames( 0 ),
		m_totalFrames( 0 ),
		m_fpsUpdateInterval(0),
		m_minFrameDelta(0),
		m_maxFrameDelta(0),
		m_rollingAverageFpsA(0),
		m_rollingAverageFpsB(0)
	{
		start();
	}

	FrameTimer::~FrameTimer()
	{
		/*Float32 average = (Float32)m_totalFrames / (Float32)(simulationTime_secs() - m_timerStartTimeStamp);
		if ( _GlobalLogPtr )
		{
			logComment( "Average-Fps = " << average );
		}*/
	}

	void FrameTimer::start()
	{
		Timer::start();

		UInt32 timerStartTimeStamp = time_ticks();
		m_lastFpsTimeStamp = timerStartTimeStamp;
		m_lastFrameTimeStamp = timerStartTimeStamp;
		m_frameDeltaTime = 0; ///< The frame time
		m_currentFps = 0;
		m_frames = 0;
		m_totalFrames = 0;
		m_fpsUpdateInterval = UInt32((Real)0.25 * _SystemTimer.ticksPerSecond());
		m_minFrameDelta = UInt16_Max;
		m_maxFrameDelta = 0;
		m_rollingAverageFpsA = 0;
		m_rollingAverageFpsB = 0;
	}

	void FrameTimer::endFrame()
	{
		++m_frames;
		++m_totalFrames;

		UInt32 currentTime = time_ticks();
		m_frameDeltaTime = currentTime - m_lastFrameTimeStamp;

		if ( m_frameDeltaTime < m_minFrameDelta ) m_minFrameDelta = m_frameDeltaTime;
		if ( m_frameDeltaTime > m_maxFrameDelta ) m_maxFrameDelta = m_frameDeltaTime;

		m_lastFrameTimeStamp = currentTime;

		UInt32 fpsElapsed = currentTime - m_lastFpsTimeStamp;

		if ( fpsElapsed > m_fpsUpdateInterval )
		{
			m_currentFps = m_frames / ((Real)fpsElapsed * _SystemTimer.invTicksPerSecond());
			m_frames = 0;
			m_lastFpsTimeStamp = currentTime;

			m_currentMinFrameDelta = m_minFrameDelta; m_minFrameDelta = UInt16_Max;
			m_currentMaxFrameDelta = m_maxFrameDelta; m_maxFrameDelta = 0;		
		
			const Real kRollingIntervalA = Real(1.0/5);
			const Real kRollingIntervalB = Real(1.0/30);
			m_rollingAverageFpsA = m_rollingAverageFpsA * ((Real)1.0-kRollingIntervalA) + m_currentFps * kRollingIntervalA;
			m_rollingAverageFpsB = m_rollingAverageFpsB * ((Real)1.0-kRollingIntervalB) + m_currentFps * kRollingIntervalB;

			//logComment(  _T("\tFps\t") << m_currentFps << '\t' << currentTime );
		}
	}

} //END: kismo
