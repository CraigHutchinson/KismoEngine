/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\KismoCore\KismoParallel\Thread.h
 * Status: duplicate of kismo-core/_SOURCE_/KismoParallel/Thread.h
 */
#ifndef KISMOPARALLEL_THREAD_H
#define KISMOPARALLEL_THREAD_H

#include "KismoParallel\Common.h"
#include "KismoFramework\Types.h"

namespace kismo
{
	
		
	/**
		\brief Enumerator for device thread state
	*/
	enum eThreadState
	{
		ThreadState_Constructed,
		ThreadState_Starting,
		ThreadState_Running,
		ThreadState_Suspended,
		ThreadState_Stopping,
		ThreadState_Finished,
	};
		
	/**
		\brief Enumerator for device thread priority
	*/
	enum eThreadPriority
	{		
		ThreadPriority_Idle,// = THREAD_PRIORITY_IDLE,

		ThreadPriority_Low,// = THREAD_PRIORITY_LOWEST,
		ThreadPriority_BelowNormal,// = THREAD_PRIORITY_BELOW_NORMAL,
		ThreadPriority_Normal,// = THREAD_PRIORITY_NORMAL,
		ThreadPriority_AboveNormal,// = THREAD_PRIORITY_ABOVE_NORMAL,
		ThreadPriority_High,// = THREAD_PRIORITY_HIGHEST,

		ThreadPriority_TimeCritical,// = THREAD_PRIORITY_TIME_CRITICAL,
	};

	/**
		\brief Obtains the enumerator from a string literal defining a thread priority
	*/
	static eThreadPriority getThreadPriorityFromName( const Char* name );

	class Thread
	{
		
#if KISMO_PLATFORM == KISMO_PLATFORM_WINDOWS
#	include "KismoParallel/Thread_Win32_Members.h"
#else
#error Unsupported platform for Threading::Thread
#endif

	public:
		
		static Count kDestructorTimeoutMs; //< Time to wait for thread to stop when the destructor is called (default = 10 seconds)

	public:
		inline Thread();
		inline ~Thread();
				
        operator ThreadHandle ()
        {  return m_handle; }

        // .. INQUIRIES

		/*
        const PARALLEL_THREAD_IDENTIFIER & GetIdentifier() const
        {
            return Identifier;
        }
		*/

        eThreadState getState() const
        { return m_state; }
		
        bool isSuspended() const
        { return m_state == ThreadState_Suspended;  }
		
        bool isRunning()
		{ return m_state == ThreadState_Running; }
		
        bool isFinished()
		{ return m_state == ThreadState_Finished; }

        // .. OPERATIONS

        void create(
            const int processor_index = 0
            );

        // ~~

        void start(  const int processorIndex = 0 );

#if !RETAIL_Mode
		/** Set a thread name to aid with thread debugging
		*/
		void setDebugName( const Char* threadName );
#endif
		
        void suspend();

        void resume();

        void stop();

        void stopAndWait();

		void terminate( const UINT32 exitCode );

        bool wait( const float time = 0.0f );

        void setPriority( const eThreadPriority priority );
        eThreadPriority getPriority();
						
    protected:

        Thread( const Thread& other );
        Thread& operator = ( const Thread& other );
		
        int run();

	private:
		eThreadState m_state;
		ThreadHandle m_handle;
	};

} //END: kismo

#if KISMO_PLATFORM == KISMO_PLATFORM_WINDOWS
#	include "KismoParallel/Thread_Win32.inl"
#else
#error Unsupported platform for Threading::Thread
#endif

#endif