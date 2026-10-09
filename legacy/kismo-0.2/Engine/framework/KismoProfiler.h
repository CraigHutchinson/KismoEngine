/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\framework\KismoProfiler.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef _KISMO_PROFILER_H
#define	_KISMO_PROFILER_H

#include "framework/KismoTimer.h"
#include "framework/KismoCommon.h"

#include <stack>
#include <list>
#include <cassert>

namespace kismo
{
	namespace frx
	{		
		/*
			Profiler starts a timer with a push and ends it on a pop
			-using this data forms a timings tree
			-NOTICE: The tree is only constructed when an output is requested so making 
				the profiler more efficient during runtime
			TODO: Create the 'FunctionProfiler' class for automatic function timings using a local instance 
				thats destroid at the end of the function
		*/

		class _KismoExport Profiler : private Timer, public Singleton< Profiler >
		{
		public:
			Profiler();

			//Pushes a new timer onto the stack
			void pushTimer( k_String sectionName );

			//pops the last 
			void popTimer();

			//Stream operator
			inline friend k_OStream& operator << ( k_OStream& outStream, const Profiler& v )
			{
				outStream << "Timings:" << std::endl;

				for( k_List< STimer* >::const_iterator iter = v.m_timingResults.begin(); iter != v.m_timingResults.end(); ++iter )
				{
					STimer* parent = (*iter);
					while( parent = parent->parent ) 
						outStream << _T('\t');

					outStream << _T('[') << (*iter)->name.c_str() << _T("] >> ") << (*iter)->time << std::endl;
				}
			    
				return outStream;
			}

			static Profiler& getSingleton( void );
			static Profiler* getSingletonPtr( void );

		protected:		
			struct STimer
			{
				STimer( k_UInt32 ntime, k_String nname, STimer* nparent) :
					time(ntime), name(nname),parent(nparent)
				{}

				k_UInt32 time; //Start time of the timer...and elapsed time after completion
				k_String name; //Name identifyer of the timer
				STimer* parent; //The parent timer section (used to form tree on output request)
			};


			std::stack< STimer* > m_runningTimers; //Stack of STimer pointers for quicker moving into the timings tree
			
			k_List< STimer* > m_timingResults; //List of timings

	        
		};
	} // END: frx
} // END: kismo

#endif
