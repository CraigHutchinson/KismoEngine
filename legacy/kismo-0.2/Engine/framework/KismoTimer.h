/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\framework\KismoTimer.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef _KISMO_TIMER_H
#define	_KISMO_TIMER_H

#include "framework/KismoPlatform.h"
#include "framework/KismoTypes.h"
#include "framework/KismoSingleton.h"

#define _SystemTimer kismo::frx::SystemTimer::getSingleton()

namespace kismo
{
	namespace frx
	{	
		/** \brief SystemTimer is a timer class which interacts directly with the computers internal high
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
		http://msdn2.microsoft.com/en-us/library/ms644904.aspx */
		class _KismoExport SystemTimer : Singleton< const SystemTimer >
		{
		public:
			/** Get a reference to the singleton instance of the system timer 
			\return Reference to the timer */
			static const SystemTimer& getSingleton( void );
			/** Get a pointer to the singleton instance of the system timer
			\return Pointer to the timer */
			static const SystemTimer* getSingletonPtr( void );
		public:
			/** Default constructor checks system capabilities and gets clock ticks pers second */
			SystemTimer();

			/** Gets the number of clock ticks per second
			\return System time at resolution() counts per second*/
			inline k_UInt32 ticksPerSecond() const { return m_ticksPerSecond; }
			/** Resets the runningTime counter to zero */
			//inline void resetRunningTime() { m_startTime = system_ticks(); }

			/** Gets the value of the system timer
			\return System time ticks, where ticksPerSecond() returns the ticks per second */
			k_Int64 system_ticks() const;
			/** Gets the value of the system timer at the highest accuracy available on the plaform 
			\return System time in milliseconds*/
			inline k_Float64 system_ms() const
			{ return system_secs() * (k_Float64)1000.0; }
			/** Gets the value of the system timer at the highest accuracy available on the plaform 
			\return System time in seconds*/
			inline k_Float64 system_secs() const
			{ return (k_Float64)system_ticks() / (k_Float64)m_ticksPerSecond; }

			/** Gets the amount of time since the timer was created or the last resetRunningTime() occured
			\return Time in clock ticks, where ticksPerSecond() returns the ticks per second  */
			inline k_UInt32 runningTime_ticks() const 
			{ return (k_UInt32)(system_ticks() - m_startTime); }
			/** Gets the amount of time since the timer was created or the last resetRunningTime() occured
			\return Time in milliseconds */
			inline k_Float32 runningTime_ms() const 
			{ return runningTime_secs() * (k_Float32)1000.0; }
			/** Gets the amount of time since the timer was created or the last resetRunningTime() occured
			\return Time in seconds */
			inline k_Float32 runningTime_secs() const 
			{ return (k_Float32)(system_ticks() - m_startTime) / (k_Float32)m_ticksPerSecond; }

		private:
			k_UInt32 m_ticksPerSecond; ///< The number of clock ticks per second
			k_Bool m_usePerformanceCounter; ///< Whether the timer uses the high performance counters
			k_Int64 m_startTime; ///< The time that the timer was started
		};

		/** \brief Simulation timer extends alows changing the speed of time and
		to even reverse it if required.
		\remarks Defaults to a paused state and a scaleFactor of 1.0 (normal clock speed)
		\remarks All functions in this class return the most upto date time value available (i.e. at time of function call)
		*/
		class _KismoExport SimulationTimer
		{
		public:
			/** Default constructor initialises the timer in a paused state */
			SimulationTimer() :
				m_simulationTimeScale( (k_Float32)1.0 ),
				m_lastSimTimeStamp( 0 ), m_baseSimTime( 0 ), m_running( false )
			{}

			/** Gets the simulation time
			\return Time in milliseconds */
			inline k_Float32 simulationTime_ms() const 
			{ return simulationTime_secs() * (k_Float32)1000.0; }
			/** Gets the simulation time
			\return Time in seconds */
			inline k_Float32 simulationTime_secs() const 
			{ 
				if ( m_running )					
					return m_baseSimTime + (k_Float32)(_SystemTimer.system_secs() - m_lastSimTimeStamp) * m_simulationTimeScale;
				else
					return m_baseSimTime; //Return the time at which the clock was paused
			}
			
			/** Starts the simulation timer running */
			void start()
			{
				if ( m_running ) return; //Do Nothing as to stop start() moving time back to the baseSimTime
				m_lastSimTimeStamp = _SystemTimer.system_secs(); //Start timing from now
				m_running = true;
			}
			/** Pauses the simulation timer */
			void pause()
			{
				m_baseSimTime = simulationTime_secs(); //Store the current simulation time
				m_running = false;				
			}
			/** Stops the timer and resets the simulation time to zero */
			void stop()
			{
				m_running = false;				
				reset(); //Reset the simulation time
			}
			/** Resets the simulation time to zero */
			void reset() { setSimulationTimeSecs( 0 ); }
			/** Set the simulation time to a specific time
			\param[in] time The time to set the simulation timer to. */
			void setSimulationTimeSecs( k_Float32 seconds )
			{
				m_baseSimTime = seconds;
				m_lastSimTimeStamp = _SystemTimer.system_secs(); //Start timing from now
			}

			/** Sets the scale factor for the speed that time passes.
			\param[in] scaleFactor A scale factor for the passage of time where 1.0 is normal speed,
			2.0 is double speed, 0.0 is paused, and -1.0 is reversed time*/
			void setSimulationTimeScale( k_Float32 scaleFactor = 1.0f )
			{
				m_baseSimTime = simulationTime_secs(); //Store the current simulation time
				m_lastSimTimeStamp = _SystemTimer.system_secs(); //Store the system time stamp
				m_simulationTimeScale = scaleFactor; //set the new simulation scale
			}
			/** \return The current scale factor for the simulation time */
			k_Float32 simulationTimeScale() const { return m_simulationTimeScale; }

		private:
			k_Float32 m_simulationTimeScale; ///< The current scale factor for simulation time
			k_Float64 m_lastSimTimeStamp; ///< The system tme in seconds that the simulation scale factor was last set or timer paused/started
			k_Float32 m_baseSimTime; ///< The accumulated simulation time in seconds when the simulation scale factor was last set
			k_Bool m_running; ///< Whether the clock is running/started
		};



		class  _KismoExport  Timer
		{
		public:
			//NOTICE: Application time is the time since the timer class was instanciated

			//Constructor
			Timer();

			//----------------------------------------
			// Timer controls
			//----------------------------------------
			void start() // Starts the simulation timer running (Un-Pauses)
				{ m_isPaused = false; m_lastGetElapsedTime = m_frameStartTime = getSystemTime(); }
			void pause() { m_isPaused = true; } // Stops the simulation timer progressing (Pauses)

			void reset();//Resets the simulation time to 0
			void resetAll();//Resets the application and simulation time to 0

					
			void startFrame();	//Records a frame and calculates timings	
								//NOTICE: Must be called every frame for simulation time to be calculated

			//----------------------------------------
			// Get timer values as of last call to startFrame()
			//----------------------------------------
			k_UInt32 getSystemTime() const; //Gets the computer system time
			k_UInt32 getApplicationTime() const //Gets the time that the application has been running since the timer was created
				{ return getSystemTime() - m_timerStartTime; }
			k_Float32 getSimulationTime() const//Gets the simulation time as of the last startFrame
				{ return m_simulationTime; }

			//----------------------------------------
			// Frame elapsed timings
			//----------------------------------------
			k_Float32 getFrameSimulationTime() const//Gets the simulation time elapsed betweem the last startFrame() calls
				{ return m_frameSimulationTimeElapsed; } 

			k_UInt32 getFrameTime() const //Gets the system time elapsed betweem the last startFrame() calls
				{ return m_frameTimeElapsed; } 

			k_UInt32 getElapsedTime();//Gets the time elapsed from the last call to getElapsedTime() or start()

			//----------------------------------------
			// Timer properties
			//		simulation time scaling: 0.5 = half speed, 1 = normal speed, 2 = double speed, etc...
			//----------------------------------------		  
			void setSimulationTimeScale( k_Float32 timeScale = 1.0f ) //sets the speed of the simulation timer
				{ m_simulationTimeScale = timeScale; }

			k_Float32 getSimulationTimeScale() const //Gets the current speed of simulation
				{ return m_simulationTimeScale; }

			k_Bool isPaused() const { return m_isPaused; } //returns whether the timer is paused

			//Gets the current frames per second
			//k_Int32 getFPS() { return m_FPS; }

			//Stream operator
			inline friend k_OStream& operator << ( k_OStream& outStream, const Timer& t )
			{
				outStream << "Timer[ System= " << t.getSystemTime() << 
								",  Application= " << t.getApplicationTime() << 
								",  Simulation= " << t.getSimulationTime() << " ]";
				return outStream;
			}

		protected:
			k_Bool m_isPaused;//Is the timer paused or not

			//----------------------------------------
			// Performance timer
			//----------------------------------------
			k_Bool m_usePerformanceCounter; //Stores whether the performance counter is being used or not
			k_UInt64 m_ticksPerSecond;//The number of ticks the timer has per second

			//----------------------------------------
			// Time adjustments
			//----------------------------------------
			k_Float32 m_simulationTimeScale;//the speed of the simulation time ( m_frameTimeElapsed * m_timeScaling )

			//----------------------------------------
			// Timers
			//----------------------------------------
			k_UInt32 m_timerStartTime; //The system time at which the time was started	
			k_Float32 m_simulationTime; //The simulation time taking into effect time scaling as of the last startFrame

			//----------------------------------------
			// Frame timings
			//----------------------------------------
			k_UInt32 m_frameStartTime; //The time at which the last frame started
			k_UInt32 m_frameTimeElapsed;//The system time elapsed betweem the last startFrame() calls
			k_Float32 m_frameSimulationTimeElapsed;//The system time elapsed betweem the last startFrame() calls
		
			//----------------------------------------
			// Elapsed timings
			//----------------------------------------		
			k_UInt32 m_lastGetElapsedTime;//The system time at which the getElapsedTime function was last called

			//###### FPS ####
			//long m_FPSframeCount;
			//long m_FPSlastCheck;
			//long m_FPS;

		};
	} //END: frx
} //END: kismo

#endif

