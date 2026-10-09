/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\KismoCore\_SOURCE_\KismoFramework\Timer.h
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

#ifndef KISMOFRAMEWORK_TIMER_H
#define KISMOFRAMEWORK_TIMER_H

#include "KismoFramework/Types.h"
#include "KismoFramework/Singleton.h"

#define _SystemTimer kismo::SystemTimer::getSingleton()

namespace kismo
{	
	/** 
		\brief SystemTimer is a timer class which interacts directly with the computers internal high
			reolution clock maintaining the highest possible clock resolution.

		\remarks Only one system timer is ever created due to the singleton type
		\remarks The system timer becomes the base for most other timers classes
		\remarks All functions in this class return the most upto date time value available (i.e. at time of function call)
		\remarks Runtime values use 32bit floats/integers as higher accuracy is not required and will conform
			with the data size used in most calculations with time.
		\remarks On a multiprocessor computer, it should not matter which processor is called.
			However, you can get different results on different processors due to bugs in the basic 
			input/output system (BIOS) or the hardware abstraction layer (HAL). Specify a processor
			affinity for the current thread if issues occur. 
			http://msdn2.microsoft.com/en-us/library/ms644904.aspx
	*/
	class SystemTimer : public Singleton< const SystemTimer >
	{
	public:
		//SINGLETON( const SystemTimer );

		/** 
			\brief Default constructor checks system capabilities and gets clock ticks pers second 
		*/
		SystemTimer();

		/** 
			\brief Gets the number of clock ticks per second
			\return System time at resolution() counts per second
		*/
		inline UInt32 ticksPerSecond() const 
		{ return m_ticksPerSecond; }

		/**
			\returns 1.0/ticksPerSecond()
		*/
		inline Float32 invTicksPerSecond() const 
		{ return m_invTicksPerSecond; }
		
		/** 
			\brief Gets the value of the system timer
			\return System time ticks, where ticksPerSecond() returns the ticks per second 
		*/
		UInt32 system_ticks() const;

		/** 
			\brief Gets the value of the system timer at the highest accuracy available on the plaform 
			\return System time in milliseconds
		*/
		inline Real system_ms() const
		{ return system_secs() * (Real)1000.0; }

		/** 
			\brief Gets the value of the system timer at the highest accuracy available on the plaform 
			\return System time in seconds
		*/
		inline Real system_secs() const
		{ return (Real)system_ticks() * invTicksPerSecond(); }

		/** 
			\brief Gets the amount of time since the timer was created or the last resetRunningTime() occured
			\return Time in clock ticks, where ticksPerSecond() returns the ticks per second  
		*/
		inline UInt32 runningTime_ticks() const 
		{ return (UInt32)(system_ticks() - m_startTime); }

		/** 
			\brief Gets the amount of time since the timer was created or the last resetRunningTime() occured
			\return Time in milliseconds 
		*/
		inline Real runningTime_ms() const 
		{ return Real(system_ticks() - m_startTime) * invTicksPerSecond() * (Real)1000.0; }

		/** 
			\brief Gets the amount of time since the timer was created or the last resetRunningTime() occured
			\return Time in seconds 
		*/
		inline Real runningTime_secs() const 
		{ 
			return (Real)runningTime_ticks() * invTicksPerSecond();
		}

	private:
		UInt32 m_ticksPerSecond; ///< The number of clock ticks per second
		Float32 m_invTicksPerSecond; //, inverse ticks per second to be used in multiplying a number of ticks to convert into seconds
		UInt32 m_startTime; ///< The time that the timer was started
		Bool m_usePerformanceCounter; ///< Whether the timer uses the high performance counters
		UInt8 m_tickShift; ///< The tick shift is automatically calculated bitshift to reduce the accruacy of the clock so reduced data sizes are valid
	};

	/** 
		\brief Standard/simple timer for performance tests etc
		\remarks Time is initialised in a tunnign state. Call start() to begin the timer from 0
	*/
	class Timer
	{
	public:

		/**
			\brief Constructor creates the timer ina  runnign state
			\remarks Call start() to reset the timer
		*/
		Timer();
		
		/**
			\brief Starts the timer from 0
		*/
		void start();

		/**
			\brief Stops the timer
			\remarks The timer stores the current time until the next call to start() which will reset it
		*/
		void stop();

		/**
			\brief returns the system time in millisecond ssince the last call to start();
		*/
		Real time_ms() const 
		{ return time_secs() * (Real)1000.0; }

		/**
			\brief returns the system time in millisecond ssince the last call to start();
		*/
		Real time_secs() const
		{ return (Real)time_ticks() * _SystemTimer.invTicksPerSecond(); }

		/**
			\brief returns the system time in ticks since the last call to start() or the time at which stop() was last called
		*/
		UInt32 time_ticks() const
		{ 
			if ( m_running )
				return _SystemTimer.system_ticks() - m_startTime;
			else
				return m_startTime; 
		}

	protected:
		UInt32 m_startTime; ///< The time that the timer was started
		Bool m_running; ///< Whether the clock is running/started
	};

	/** 
		\brief Simulation timer alows changing the speed of time and to even reverse it if required.
		\remarks Defaults to a paused state and a scaleFactor of 1.0 (normal clock speed)
		\remarks All functions in this class return the most upto date time value available (i.e. at time of function call)
	*/
	class SimulationTimer
	{
	public:
		/** 
			\brief Default constructor initialises the timer in a paused state 
		*/
		SimulationTimer();

		/** 
			\brief Gets the simulation time
			\return Time in milliseconds 
		*/
		inline Real simulationTime_ms() const 
		{ return simulationTime_secs() * (Real)1000.0; }

		/** 
			\brief Gets the simulation time
			\return Time in seconds 
		*/
		inline Real simulationTime_secs() const;
		
		/** 
			\brief Starts the simulation timer running
		*/
		void start();

		/** 
			\brief Pauses the simulation timer
		*/
		void pause();

		/** 
			\brief Stops the timer and resets the simulation time to zero 
		*/
		void stop();

		/** 
			\brief Resets the simulation time to zero
		*/
		void reset() 
		{ setSimulationTimeSecs( 0.0f ); }
		
		/** 
			\brief Set the simulation time to a specific time
			\param[in] time The time to set the simulation timer to.
		*/
		void setSimulationTimeSecs( Real seconds );

		/** 
			\brief Sets the scale factor for the speed that time passes.
			\param[in] scaleFactor A scale factor for the passage of time where 1.0 is normal speed,
				2.0 is double speed, 0.0 is paused, and -1.0 is reversed time
		*/
		void setSimulationTimeScale( Real scaleFactor = 1.0f );
		
		/** 
			\brief
			\return The current scale factor for the simulation time 
		*/
		Real simulationTimeScale() const 
		{ return m_simulationTimeScale; }

	private:
		Real m_simulationTimeScale; ///< The current scale factor for simulation time
		Real m_lastSimTimeStamp; ///< The system tme in seconds that the simulation scale factor was last set or timer paused/started
		Real m_baseSimTime; ///< The accumulated simulation time in seconds when the simulation scale factor was last set
		Bool m_running; ///< Whether the clock is running/started
	};

	/** 
		\brief Frame timer used for calculating frame rates and frame times
	*/
	class FrameTimer : public Timer
	{
	public:
		FrameTimer();

		~FrameTimer();

		//void setAverageSamples( 

		void start();

		void endFrame();

		/** 
			\brief Get the current frames per secons (fps)
		*/
		Real getFPS() const
		{ return m_currentFps; }

		/** 
			\brief Get the average frames per second (fps) since the frame timer was started
			\returns getTotalFrameCount() / time_secs()
		*/
		Real getAverageFPS() const
		{ return getTotalFrameCount() / time_secs(); }
		
		/** 
			\brief Get the average frames per second (fps) for a period of time
			\remarks At the moment the average is based on 90% m_rollingAverage + 10% currentFps
			\TOD: we hsould buffer the samples in a ring-buffer/vector for a number of fpsUpdates and average the values
		*/
		Real getRollingAverageFPSA() const { return m_rollingAverageFpsA; }
		Real getRollingAverageFPSB() const 	{ return m_rollingAverageFpsB; }

		/**
			\returns Number of frames since last call to start()
		*/
		UInt32 getTotalFrameCount() const
		{ return m_totalFrames; }

		/**
			\returns Minimum delta time for the previous FPS interval
		*/
		UInt16 minFrameDelta_ticks() const
		{ return m_currentMinFrameDelta; }

		/**
			\returns Maximum delta time for the previous FPS interval
		*/
		UInt16 maxFrameDelta_ticks() const
		{ return m_currentMaxFrameDelta; }

		Real minFrameDelta_secs() const
		{ return minFrameDelta_ticks() * _SystemTimer.invTicksPerSecond(); }

		Real maxFrameDelta_secs() const
		{ return maxFrameDelta_ticks() * _SystemTimer.invTicksPerSecond(); }

		Real minFrameDelta_ms() const		
		{ return minFrameDelta_secs() * 1000; }

		Real maxFrameDelta_ms() const
		{ return maxFrameDelta_secs() * 1000; }

		/** 
			\brief Get the delta time of the last frame
		*/
		UInt32 getFrameDelta_ticks() const
		{ return m_frameDeltaTime; }

		Real getFrameDelta_secs() const
		{ return (Real)getFrameDelta_ticks() * _SystemTimer.invTicksPerSecond(); }
		
		Real getFrameDelta_ms() const
		{ return getFrameDelta_secs() * (Real)1000.0; }

		/** 
			\brief Get the application time of the frame
		*/
		UInt32 getFrameTime_ticks() const
		{ return m_lastFrameTimeStamp; }

		Real getFrameTime_secs() const
		{ return (Real)getFrameTime_ticks() * _SystemTimer.invTicksPerSecond(); }

		Real getFrameTime_ms() const
		{ return getFrameTime_secs() * (Real)1000.0; }

	private:
		UInt32 m_lastFrameTimeStamp; ///< The time stamp of the last end-frame
		UInt32 m_lastFpsTimeStamp; ///< The time stamp of the last fps count
		UInt32 m_totalFrames; ///< The total frame count

		UInt32 m_frameDeltaTime; ///< The frame time
		UInt32 m_fpsUpdateInterval; ///< Defines the numebr fo clcok ticks between the FPS being updated		
		UInt16 m_frames; ///< The counter used to count the number of frames each second
		UInt16 m_minFrameDelta; //, the minimum frame delta since the last fps update
		UInt16 m_maxFrameDelta; //, the maximum frame delta since the last fps update

		Real m_currentFps; ///< The current frame count
		Real m_rollingAverageFpsA, m_rollingAverageFpsB; ///< The current frame count
		UInt16 m_currentMinFrameDelta; //, the minimum frame delta since the last fps update
		UInt16 m_currentMaxFrameDelta; //, the maximum frame delta since the last fps update

	};



} //END: kismo

#endif
