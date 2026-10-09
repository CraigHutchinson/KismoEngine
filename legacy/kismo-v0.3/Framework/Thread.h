/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\Framework\Thread.h
 * Status: superseded by kismo-core/_SOURCE_/KismoParallel/Thread.h (same name, content differs)
 */
#ifndef THREAD_H
#define THREAD_H

#include "Platform/Platform.h"
#include "Framework/Types.h"
#include "Framework/Named.h"
#include "Framework/Mutex.h"
#include "Framework/Smallset.h"
#include "Framework/SmartPointer.h"
#include "Framework/LockFreeQueue.h"

namespace frx
{
	//forward declarations
	class Thread;

	/** \brief The threaded class defines the base type for any user class which desires
		threading and can be atatched to a thread to allow processing on another 
		cpu or system thread
	*/
	class _DllExport Threaded
	{
		friend class Thread; //< Give trhe thread access to the threadIteration() function
	public:		

		typedef Vector< Threaded* > PtrArray;
		typedef frx::SmallSet< Threaded* > PtrLookup;
		typedef frx::LockFreeQueue< Threaded* > PtrLFQueue;

	public:
		/** \brief Constructor - does nothing 
		*/
		Threaded();

		/** \brief Destructor removes this threaded object from the thread 
		*/
		virtual ~Threaded();

		/** \brief Assigns the threaded object to be called by a thread.
			\remarks 
				The thread that is attached will call the threadIteration() until the thread is paused,
				stopped or this Threaded object is detached
			\param[in] thread Pointer to the thread this object will execute in 
			\param[in] priority Priority level for the process if multiple 'Threaded' processes are on the Thread 
			\remarks The priority of a thread object is currently not taken into account
		*/		
		Bool attachToThread( Thread* thread, Priority priority = 0 );
		
		/** \brief Detatched the threaded object from the running thread 
		*/
		void detatchFromThread();
	
	protected:
		
		/** \brief Called from the attached thread
			\remarks 
				This should be ovveridden to perofmr the required task and then return. If
				the process is long it should preferably perform the function in small chunks
				so that other processes on the thread can operate 
			\return Returning false will cause the running thread to detatch this Threaded object from its queue */
		virtual Bool threadIteration() { return false; }

		
		/** \brief Stops execution for a specified number of milliseconds before continuing
		*/
		Bool sleep( UInt32 milliseconds );

	private:
		SmartPointer< Thread > m_thread; ///< Pointer to the thread this threaded object is running on
	};

	/** \brief Thread defines the base thread type. 
		\remarks 
			It is not intended to be the base type of a threaded class.
			For this you use the Threaded base class which should be passed to the Thread it should run on 
	*/
	class _DllExport Thread : public Named, public SmartReferenced
	{
		friend class Threaded; ///< The threaded class needs access to register and un-register itself on the thread

	public:
		
		typedef SmartPointer< Thread > Ptr;

		typedef Vector< Ptr > Array;

		/** \brief Thread priority enumerator 
		*/
		enum eThreadPriority
		{
			ThreadPriority_Idle,
			ThreadPriority_Minimum,
			ThreadPriority_Low,		
			ThreadPriority_Normal,
			ThreadPriority_Medium,
			ThreadPriority_High,				
			ThreadPriority_Maximum,
		};

		/** \brief Static createe function returns a smaert pointer thread object maintaining its lifetime
			so that it is ensure the thread does not destroy whiel a aontaining object is still attached to it
		*/
		static Ptr create( const Char* name, eThreadPriority priority = ThreadPriority_Normal );

	protected:

		/** \brief State enumerator for the state bitset 
		*/
		enum eThreadStateBit
		{
			ThreadState_IsStarted, ///< Stores whether the thread has been started
			ThreadState_IsPaused, ///< Stores whether the thread is paused
			ThreadState_ShouldStop, ///< Specifies whether the thread should terminate when possible
			ThreadState_ShouldPause, ///< Specifies whether the thread should pause at the next convenient time
			_ThreadState_Count
		};
		
	public:
		//Constructor is protected

		/** \brief Destructor
			\remarks Will assert if the thread process has not been stopped before the destructor is called */
		~Thread();
		
		/** \brief Set the threads priority 
		*/
		Bool setThreadPriority( eThreadPriority priority );

		/** \brief Gets the threads priority 
		*/
		eThreadPriority getThreadPriority() const
		{ return m_priority; }

		/** \brief Starts the thread running 
			\remarks Flags that the thread should exit and then waits for the thread to exit
			\param[in] force Specifies whether the thread should be forced to quit. This is 
			not recomended as will stop code execution at any point causing memory leaks 
			and possibly errors but is usefull tos top a deradlocked thread
		*/
		Bool startThread();

		/** \brief Terminates the thread 
		*/
		Bool stopThread( Bool force = false );

		/** \brief Pauses execution of the thread so stopping any execution at this time
		*/
		Bool pauseThread();

		/** \brief Waits for the thread to finish executing.
			\remarks Will return false if the thread is paused or the wait fails
			//TODO: add a getLastError method for users of this class. Currently error
			should go to the globalLog
		*/
		Bool waitThread();

		/** \brief Resumes a paused thread 
		*/
		bool resumeThread() 
		{ 
			if ( isThreadPaused() ) 
				return startThread(); 
			assert( false );
			return false;
		}

		/** \brief Returns whether the thread has been started and is created
			\remarks Does not return if the thread is paused. use isThreadRunning() if that is needed
		*/
		bool isThreadStarted() const 
		{ return m_state[ ThreadState_IsStarted ]; }

		/** \brief Returns whether the thread is running
			\remarks Will return false if the thread is paused. 
			Use isThreadStarted() to check the alternate case
		*/
		bool isThreadRunning() const 
		{ return m_state[ ThreadState_IsStarted ] && !isThreadPaused(); }
	
		/** \brief returns if the thread is paused 
		*/
		bool isThreadPaused() const 
		{ return m_state[ ThreadState_IsPaused ]; }

	protected:

		/** \brief Thread constructor
			\param[in] name Unique name used to identity this thread
			\param[in] priority Specifies the threads priority enumerator 
			\remarks
				Constructor, must overridden in the platforms implimentation 
		*/
		Thread( const String& name, eThreadPriority priority = ThreadPriority_Normal );

		/** \brief The thread entry point which is where the thread runs from and is called on the actual thread
		*/
		Int32 threadEntry();

		/** \brief Smart ference free() overload stops the thread before it is destroyed
		*/
		virtual void free()
		{
			stopThread();
			delete this;
		}

		/** \brief returns whether the thread has been asked to terminate 
		*/
		bool shouldThreadTerminate() const 
		{ return m_state[ ThreadState_ShouldStop ]; }

		/** \brief Called from the Threaded class to register itself with the thread 
		*/
		Bool addThreadedObject( Threaded* obj, Priority priority = 0 );

		/** \brief Called from the Threaded class to remove itself from the thread 
		*/
		Bool removeThreadedObject( Threaded* obj );

		/** \brief imnplementation of setThreadPriority which will be called if the thread has been 
			started already
		*/
		virtual Bool setThreadPriorityImpl( eThreadPriority priority ) = 0;

		/** \brief Virtual function should cause the callign thread to sleep for the respective 
			number of milliiseconds
			\remarks Used by Threaded objects attached to this thread so that they can cause execution to stall if required
		*/
		virtual Bool sleepImpl( UInt32 milliseconds ) = 0;

		/** \brief Virtual function should start the thread
		*/
		virtual Bool startThreadImpl() = 0;

		/** \brief Virtual function should terminate the thread
		*/
		virtual Bool closeThreadImpl() = 0;

		/** \brief  Virtual function should Pause execution of the thread so stopping any execution at this time
		*/
		virtual Bool pauseThreadImpl() = 0;

		/** \brief  Virtual function should Wait for the thread to finish executing.
		*/
		virtual Bool waitThreadImpl() = 0;

	private:	
		Threaded::PtrLFQueue m_threadedObjects; ///< Vector of pointers to threaded objects running on this thread
		eThreadPriority m_priority; ///< The threads priority
		BitSet< _ThreadState_Count > m_state; ///< Stores the current state flags of the thread ie. running, paused etc
		frx::Mutex::Ptr m_threadedMutex; ///< Mutex to prevent threading issues when adding and removing threaded objects
	};

} //END: frx

#endif