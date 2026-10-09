/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\Framework\Profiler.h
 * Status: superseded by kismo-core/_SOURCE_/KismoFramework/Profiler.h (same name, content differs)
 */
#ifndef FRX_PROFILER_H
#define	FRX_PROFILER_H

#include "Framework/Singleton.h"
#include "Framework/Timer.h"
#include "Framework/Common.h"

#define _Profiler frx::Profiler::getSingleton()

#ifdef METRICS
#	ifdef WIN32
#		define __PRETTY_FUNCTION__ _T(__FUNCSIG__)
#	endif

#	define INIT_PROFILER frx::Profiler g_profiler; frx::Log profilerLog( _T("profiler_") BUILD_MODE_FULL )
#	define END_PROFILER _Profiler.writeTimings( profilerLog );
#	define PROFILE frx::Profiler::AutoTimer autoTimer( __PRETTY_FUNCTION__ )
#	define STOP_PROFILE autoTimer.stop()

#	ifdef WIN32
#		define IPROFILE( code ) { frx::Profiler::AutoTimer autoTimer( __PRETTY_FUNCTION__ _T("{ ") _T(#code) _T(" }") ); code; }
#		define SCOPE_PROFILE( name ) frx::Profiler::AutoTimer autoTimer( __PRETTY_FUNCTION__ _T("{ ") _T(#name) _T(" }") );
#	else
//		Sadly GCC does __PRETTY_FUNCTION__ as a variable so can't concatenate with code so IProfile is ignored for now :(
#		define IPROFILE( code ) code
#		define SCOPE_PROFILE( name )
#	endif

#else
#	define INIT_PROFILER
#	define END_PROFILER
#	define STOP_PROFILE
#	define SCOPE_PROFILE( name )
#	define PROFILE
#	define IPROFILE( code ) code
#endif

namespace frx
{		
	//Forward declarations
	class Log;

	/** \brief Profiler is used to time function times by creating AutoTimer, or Timer instances
	*/
	class _DllExport Profiler : public Singleton< Profiler >
	{
	public:
		SINGLETON( Profiler );

		/** \brief Holds the timing statistics of a functiojn timer
		*/
		struct Stat
		{
			Stat() : calls(0), ticks(0), childTicks(0) {}

			UInt32 ticks; ///< The total call time in system ticks
			UInt32 childTicks; ///< The total time spent in child functions that are profiled
			UInt32 calls; ///< The number of calls made to the function
		};

		typedef Map< const Char*, Stat > StatMap;
		
		/** \brief Manual timer 
		*/
		class Timer
		{
		public:
			inline Timer( const Char* timerName ) :
				m_timerName( timerName ),
				m_childTicks( 0 )
			{
				parentTimer = Profiler::getCurrentTimer(); /// Store the parent timer
				Profiler::setCurrentTimer( this );
			}
				
			inline virtual ~Timer()
			{
				Profiler::setCurrentTimer( parentTimer );
				assert( !m_timerName ); //< Timer must be stopped unless it won't work!
			}

			inline void start()
			{
				m_startTime = _SystemTimer.runningTime_ticks();
			}

			inline void stop()
			{
				if ( !m_timerName ) return; //Check if stop has already been called

				UInt32 ticks = (UInt32)(_SystemTimer.runningTime_ticks() - m_startTime);
				if ( parentTimer )
					parentTimer->m_childTicks += ticks; //Subtract this time from the parents

				//Add the timing to the profiler
				_Profiler.addTiming( m_timerName, ticks, m_childTicks );
				m_timerName = 0; //< Stop a re-occurance of top occuring
			}

			const Char* m_timerName; ///< The name of the timer
			UInt32 m_startTime; //Start time of the timer
			UInt32 m_childTicks; //The time spent in the child functions
			Timer* parentTimer; ///< Pointer to the parent timer

		};

		/** \brief Automatic profiler adds the timing of the objects lifetime to the profiler.
		*/
		class AutoTimer : public Timer
		{
		public:
			inline AutoTimer( const Char* timerName ) :
				Timer( timerName )
			{ start(); }

			inline ~AutoTimer()
			{ stop(); }
		};

		/** \brief get the current timer in this scope
		*/
		static Timer* getCurrentTimer()
		{ return m_currentTimer; }

	protected:

		friend class Timer;

		/** \brief Set the current timer in this scope
			\remarks Called by a Timer when it is created
		*/
		static void setCurrentTimer( Timer* timer )
		{ m_currentTimer = timer; }

	public:

		Profiler()
		{
			//To save allocation at runtime we pre-allocate 256 elements for the map
			//m_timingResults.get_allocator().allocate( 256 ) ;
		}

		~Profiler()
		{
		}

		/** \brief Adds a timing to the profiler
		*/
		inline void addTiming( const Char* timerName, UInt32 ticks, UInt32 childTicks = 0 )
		{
			Stat& stat = m_timingResults[ timerName ];
			stat.calls += 1;
			stat.ticks += ticks;
			stat.childTicks += childTicks;
		}

		/** \brief Write the profilers timing to a log file
			\remarks The log format will be a table deliminated list which can be 
			loaded directly into a spredsheet application i.e Microsoft Excel
		*/
		void writeTimings( Log& log ) const;

	protected:		

		StatMap m_timingResults; ///<Map of timings indexed by the const char* for the function address
		static Timer* m_currentTimer; ///< Pointer to the current timer in scope i.e last timer created
	};
} // END: frx

#endif
