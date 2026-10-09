/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\Framework\Timer.h
 * Status: superseded by kismo-core/_SOURCE_/KismoFramework/Timer.h (same name, content differs)
 */
#ifndef FRX_TIMER_H
#define FRX_TIMER_H

#include "Framework/Types.h"
#include "Framework/Singleton.h"

#define _SystemTimer frx::SystemTimer::getSingleton()

namespace frx
{	
	/** \brief SystemTimer is a timer class which interacts directly with the computers internal high
	reolution clock maintaining the highest possible clock resolution.
	\remarks ++ This is modified from work in another module ++
	\remarks Only one system timer is ever created due to the singleton type
	\remarks The system timer becomes the base for most other timers classes
	\remarks All functions in this class return the most upto date time value available (i.e. at time of function call)
	\remarks Runtime values use 32bit floats/integers as higher accuracy is not required and will conform
	with the data size used in most calculations with time.
	\remarks On a multiprocessor computer, it should not matter which processor is called.
	However, you can get different results on different processors due to bugs in the basic 
	input/output system (BIOS) or the hardware abstraction layer (HAL). Specify a processor
	affinity for the current thread if issues occur. 
	http://msdn2.microsoft.com/en-us/library/ms644904.aspx */
	class _DllExport SystemTimer : public Singleton< const SystemTimer >
	{
	public:
		SINGLETON( const SystemTimer );

		/** \brief Default constructor checks system capabilities and gets clock ticks pers second 
		*/
		SystemTimer();

		/** \brief Gets the number of clock ticks per second
		\return System time at resolution() counts per second
		*/
		inline UInt32 ticksPerSecond() const 
		{ return m_ticksPerSecond; }
		
		/** \brief Gets the value of the system timer
		\return System time ticks, where ticksPerSecond() returns the ticks per second */
		UInt32 system_ticks() const;

		/** \brief Gets the value of the system timer at the highest accuracy available on the plaform 
		\return System time in milliseconds
		*/
		inline Real system_ms() const
		{ return system_secs() * (Real)1000.0; }

		/** \brief Gets the value of the system timer at the highest accuracy available on the plaform 
		\return System time in seconds
		*/
		inline Real system_secs() const
		{ return (Real)system_ticks() / m_ticksPerSecond; }

		/** \brief Gets the amount of time since the timer was created or the last resetRunningTime() occured
		\return Time in clock ticks, where ticksPerSecond() returns the ticks per second  
		*/
		inline UInt32 runningTime_ticks() const 
		{ return (UInt32)(system_ticks() - m_startTime); }

		/** \brief Gets the amount of time since the timer was created or the last resetRunningTime() occured
		\return Time in milliseconds 
		*/
		inline Real runningTime_ms() const 
		{ return ((Real)(system_ticks() - m_startTime) / ((Real)m_ticksPerSecond* (Real)1000.0f)); }

		/** \brief Gets the amount of time since the timer was created or the last resetRunningTime() occured
		\return Time in seconds 
		*/
		inline Real runningTime_secs() const 
		{ 
			return ((Real)runningTime_ticks() / (Real)m_ticksPerSecond);
		}

	private:
		UInt32 m_ticksPerSecond; ///< The number of clock ticks per second
		UInt32 m_startTime; ///< The time that the timer was started
		Bool m_usePerformanceCounter; ///< Whether the timer uses the high performance counters
		UInt8 m_tickShift; ///< The tick shift is automatically calculated bitshift to reduce the accruacy of the clock so reduced data sizes are valid
	};

	/** \brief Simulation timer extends alows changing the speed of time and
	to even reverse it if required.
	\remarks Defaults to a paused state and a scaleFactor of 1.0 (normal clock speed)
	\remarks All functions in this class return the most upto date time value available (i.e. at time of function call)
	*/
	class _DllExport SimulationTimer
	{
	public:
		/** \brief Default constructor initialises the timer in a paused state */
		SimulationTimer() :
			m_simulationTimeScale( (Real)1.0f ),
			m_lastSimTimeStamp( (Real)0.0f ), m_baseSimTime( (Real)0.0f ), m_running( false )
		{}

		/** \brief Gets the simulation time
		\return Time in milliseconds */
		inline Real simulationTime_ms() const 
		{ return simulationTime_secs() * (Real)1000.0f; }
		/** \brief Gets the simulation time
		\return Time in seconds */
		inline Real simulationTime_secs() const 
		{ 
			if ( m_running )					
				return m_baseSimTime + (Real)(_SystemTimer.runningTime_secs() - m_lastSimTimeStamp) * m_simulationTimeScale;
			else
				return m_baseSimTime; //Return the time at which the clock was paused
		}
		
		/** \brief Starts the simulation timer running */
		void start()
		{
			if ( m_running ) return; //Do Nothing as to stop start() moving time back to the baseSimTime
			m_lastSimTimeStamp = _SystemTimer.runningTime_secs(); //Start timing from now
			m_running = true;
		}

		/** \brief Pauses the simulation timer */
		void pause()
		{
			m_baseSimTime = simulationTime_secs(); //Store the current simulation time
			m_running = false;				
		}
		/** \brief Stops the timer and resets the simulation time to zero */
		void stop()
		{
			m_running = false;				
			reset(); //Reset the simulation time
		}
		/** \brief Resets the simulation time to zero */
		void reset() { setSimulationTimeSecs( 0.0f ); }
		/** \brief Set the simulation time to a specific time
		\param[in] time The time to set the simulation timer to. */
		void setSimulationTimeSecs( Real seconds )
		{
			m_baseSimTime = seconds;
			m_lastSimTimeStamp = _SystemTimer.runningTime_secs(); //Start timing from now
		}

		/** \brief Sets the scale factor for the speed that time passes.
		\param[in] scaleFactor A scale factor for the passage of time where 1.0 is normal speed,
		2.0 is double speed, 0.0 is paused, and -1.0 is reversed time*/
		void setSimulationTimeScale( Real scaleFactor = 1.0f )
		{
			m_baseSimTime = simulationTime_secs(); //Store the current simulation time
			m_lastSimTimeStamp = _SystemTimer.runningTime_secs(); //Store the system time stamp
			m_simulationTimeScale = scaleFactor; //set the new simulation scale
		}
		/** \brief \return The current scale factor for the simulation time */
		Real simulationTimeScale() const { return m_simulationTimeScale; }

	private:
		Real m_simulationTimeScale; ///< The current scale factor for simulation time
		Real m_lastSimTimeStamp; ///< The system tme in seconds that the simulation scale factor was last set or timer paused/started
		Real m_baseSimTime; ///< The accumulated simulation time in seconds when the simulation scale factor was last set
		Bool m_running; ///< Whether the clock is running/started
	};

	/** \brief Frame timer used for calculating frame rates and frame times
	*/
	class _DllExport FrameTimer : private SimulationTimer
	{
	public:
		FrameTimer();

		~FrameTimer();

		void start();

		Real endFrame();

		/** \brief Get the current frames per secons (fps)
		*/
		Real getFps() const
		{ return m_currentFps; }

		/** \brief Get the delta time of the last frame
		*/
		const Real& getFrameDelta() const
		{ return m_frameTime; }

		/** \brief Get theapplication time of the frame
		*/
		const Real& getFrameTime() const
		{ return m_lastFrameTimeStamp; }
	private:
		Real m_timerStartTimeStamp; ///< The time stamp that the timer was started
		Real m_lastFrameTimeStamp; ///< The time stamp of the last end-frame
		Real m_lastFpsTimeStamp; ///< The time stamp of the last fps count
		Real m_frameTime; ///< The frame time
		Real m_currentFps; ///< The current frame count
		UInt16 m_frames; ///< The counter used to count the number of frames each second
		UInt32 m_totalFrames; ///< The total frame count
	};


} //END: frx

#endif
