/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\KismoCore\_SOURCE_\KismoFramework\Thread.h
 * Status: superseded by kismo-core/_SOURCE_/KismoParallel/Thread.h (same name, content differs)
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

#ifndef KISMOFRAMEWORK_THREAD_H
#define KISMOFRAMEWORK_THREAD_H

#include "KismoFramework/Types.h"
#include "KismoFramework/String.h"

#define THREAD_LOCK AutoThreadLock _autoThreadLock( this );
#define THREAD_LOCKEX( timeout ) AutoThreadLock _autoThreadLock( this, timeout);
#define THREAD_RELEASE AutoThreadLock _autoThreadLock.releae();

namespace kismo
{
	#define Win32API __stdcall
	typedef void* Win32Handle;

	///Forward declarations
	class Runnable;

	class Thread
	{
	public:
		
		/**
			\brief Enumerator for device thread priority
		*/
		enum ePriority
		{		
			Priority_Idle,// = THREAD_PRIORITY_IDLE,

			Priority_Low,// = THREAD_PRIORITY_LOWEST,
			Priority_BelowNormal,// = THREAD_PRIORITY_BELOW_NORMAL,
			Priority_Normal,// = THREAD_PRIORITY_NORMAL,
			Priority_AboveNormal,// = THREAD_PRIORITY_ABOVE_NORMAL,
			Priority_High,// = THREAD_PRIORITY_HIGHEST,

			Priority_TimeCritical,// = THREAD_PRIORITY_TIME_CRITICAL,
		};

		/**
			\brief Obtains the enumerator from a string literal defining a thread priority
		*/
		static ePriority getThreadPriorityFromName( const String& name );
		
		static Count kDestructorTimeoutMs; //< Time to wait for thread to stop when the destructor is called (default = 10 seconds)

		class Callback
		{
		public:

			/**
				\brief callback that occurs before a threadables thread is started
				\remarks called in the thread that starts the threadable thread
			*/
			virtual bool preThreadStart() { return true; }

			/**
				\brief Callback fromt the threadables thread at the start of the thread
			*/
			virtual bool onThreadStart() { return true; }
			
			/**
				\brief Callback from the threadables thread at the end of the thread
			*/
			virtual bool onThreadEnd() { return true; }

			/**
				\brief callback that occurs after a runnables thread is terminated
				\remarks called in the thread that starts the threadables thread
			*/
			//bool postThreadEnd( Runnable* threadable ) { return true; }
		};

	public:

		Thread();
		~Thread();
		
		/**
			\brief	Set the runnableObject that this thread updates
			\TODO We should support lots of runnables per thread!
		*/
		bool setThreadableObject( Runnable* runnableObject );

		/** ----- Thread creation ----- **/

		/**
			\brief Sets the thread priority
			\remarks	This informs the operating system how often this thread should be processed
		*/
		bool setPriority( ePriority prority );

		/**
			\brief Sets the cores the thread is to be executed on
			\param[in]	processorAffinitymask	Mask defininf processors the thread may execute on
		*/
		bool setProcessorAffinity( UInt64 processorAffinitymask );

		/**
			\brief	Sets the ideal processor for the thread
			\param[in]	preferredProcessor	Index of preferred processor
			\remarks	If the preferredProcessor = 0 then no preference is set
		*/
		bool setPreferredProcessor( UInt8 preferredProcessor );
		
		/**
			\brief	Sets the sleep in milliseconds that the tread will pause at the end of each iteration
			\remarks	It is not guaranteed the sleep will be exact due to operating system thread managment
		*/
		bool setUpdateSleep( Count sleep );

		/**
			\brief	Starts the runnableObject thread
			\param[in]	mutexName	Optional global-name for the threads mutex. 
									If used the mutex can be found by name in other thread/processed
			\param[in]	stackSize	Optional stack size in bytes. 0 will create the thread with the 
									default size for the programs threads
			\remarks	A thread mutex is always created in each thread
		*/
		bool start( const Char* mutexName = 0, Size stackSize = 0 );

		/**
			\brief	Stops the runnableObject thread and closes any mutex
			\remarks	Any event signal created via createUpdateEvent() will also be destroyed for now
			\param[in]	timeOut		Time to wait for the thread to terminate. After which the thread will 
									be forcefully terminated.
		*/
		bool stop( Count timeOut = Count_Max );

		bool isRunning() const
		{ return m_running; }
			
		/** ----- Cross threading ----- **/

		/**
			\brief	Locks the update thread so that runnableObject data may be written/read by a different thread
			\param[in]	timeOut	Time to wait in milliseconds for the mutex to become available. If the timeout occurs
								false will be returned
			\returns	Whether the lock succeeded wihtin the timeout period
			\remarks	Mutliple locks can occur from a single thread and should have the corresponding number of calls to release()
		*/
		bool lock( Count timeOut = Count_Max );

		/**
			\brief	Releases a mutex lock which was created by lock()
			\remarks	Mutliple locks can occur from a single thread and should have the corresponding number of calls to release()
		*/
		bool release();

		/**
			\brief	Returns whether the current executing thread is this thread
		*/
		bool isCurrent() const;

		/** ----- Thread update events ----- **/

		/**
			\brief	Creates an event which must be triggered for the update thread to process an iteration
			\param[in]	initialState	Specifies whether the event is created in the 'triggered' state
			\patam[in]	eventName		Optional name of the event which is set globally on the running computer for access
										lookup by name in a different thread or process.
		*/
		bool createUpdateEvent( bool initialState = false, const Char* eventName = 0 );

		/**
			\brief	Returns whether the thread was created with an update event which will stop the thread
					updates until the event is flagged. 
			\remarks	The update event may be useful for threads that are dependant on the results of others
		*/
		bool hasUpdateEvent() const
		{ return m_event != 0; }

		/**
			\brief	Fire the update event to signal a thread iteration
		*/
		bool triggerUpdateEvent();

	protected:

		/**
			\brief Updates the runnableObject
		*/
		UInt32 threadProcess();

		
		/**
			\brief Wait for the event to be triggered - USed internally for devices but other systems may use if required
			\param[in] timeOut Waitout period after which the fucntion will return false, and true if the timeOut is nto reached before the evnt is triggered
			\remarks Always returns true if not event is created. use hasUpdateEvent() to check
		*/
		bool waitOnUpdateEvent( Count timeOut = Count_Max );

	private:		

		static UInt32 Win32API staticThreadProcess( void* lpThreadParameter );

	protected:	
		Runnable* m_runnable; //, The divce this thread updates
		Win32Handle m_mutex; //, mutex used for corss thread syncronisation
		Win32Handle m_handle; ///< Thread handle
		UInt32 m_id; ///< Thread ID for identifying
		
		Win32Handle m_event; ///< Event handle used to update the runnableObject only at specified intervals etc
		Count m_sleep; ///, The sleep time between updates
		
		bool m_running; ///< Flag used to terminate update thread
		
		//TODO: We could remove these somehow to conserve memory?
		bool m_lastUpdateReturn; //, The value returned form the last runnableObject update
		UInt8 m_preferredProcessor; //, Specifies a prefered processor or -1 to leave windows to it
		ePriority m_priority; //, priority enumerator
		UInt64 m_processorAffinityMask; //, The processor affinity
	};

	class Runnable : protected Thread::Callback
	{
		friend class ThreadLock; //< Thread locking object needs access to lock the thread etc
		friend class Thread; //, the thread needs to call update
	public:

		/**
			\brief Thread safe object to prevent users of the threable object locking the object and not releaseing. 
			\remarks Does not automatically lock when created. Use AutoThreadLock for this.
			\remarks Also simplifies cases where there are multiple exits/returns from a function
		*/
		class ThreadLock
		{
		public:
			ThreadLock( Runnable* threadable ) : m_runnable(threadable), m_locked(false) { Assert(m_runnable); }			
			~ThreadLock() { release(); }

			bool lock( Count timeOut = Count_Max ) const 
			{ return (m_locked = m_runnable->lock(timeOut)); }

			bool release() const 
			{ return m_locked && m_runnable->release(); }

		private:
			Runnable* m_runnable;
			mutable bool m_locked; //< Whether the lock is alreayd in place
		};

		/**
			\brief Thread safe object automatically locks the current thread when created and prevents users of the threable object locking the object
			and not releaseing control
			\remarks Also simplifies cases where there are multiple exits/returns from a function
		*/
		class AutoThreadLock : public ThreadLock
		{
		public:
			AutoThreadLock( Runnable* threadable, Count timeOut = Count_Max ) : ThreadLock( threadable ) { lock(timeOut); }
		};

	public:

		Runnable();
		virtual ~Runnable();
		
		/**
			\brief		Creates an update thread for the runnableObject
			\param[in]	updateSleep		Specifies the 'Sleep()' time performed per iteration of the devices udpate cycle
			\param[in]	useUpdateEvent	Specifies that an event will be created which will cause the runnableObject to only be updated once for every call to Container::Update()
			\returns	Whether the thread is started successfully
		*/
		bool createThread( Thread::ePriority priority = Thread::Priority_Normal, Count updateSleep = 0, bool useUpdateEvent = false, UInt64 processorAffinitymask = 0xFFFFFFFF, UInt8 preferredProcessor = -1 );

		/**
			\brief	Sets the thread that this runnableObject is on
			\todo	We could have multiple devices on the same thread but then you get update rate issues
		*/
		bool setThread( Thread* deviceThread );

		/**
			\brief	Returns whrther the object has a thread
		*/
		bool hasThread() const
		{ return m_thread && m_thread->isRunning(); }

		/**
			\brief		Returns the thread that the object is attached to
			\warning	It is recommended that a ThreadLock or AutoThreadLock object is used to lock and release the thread
		*/
		Thread* getThread() const
		{ return m_thread; }

	protected:
		
		/**
			\brief		Updates the runnable and is called in the thread
			\warning	lock() and release() will NOT be called automatically and should be 
						called when reading or writing memory that may be accesed on another thread
		*/
		virtual bool threadUpdate() 
		{ Assert(false); return false; }

		bool lock( Count timeOut = Count_Max ) const
		{ return !m_thread || m_thread->lock(timeOut); }

		bool release() const
		{ return !m_thread || m_thread->release(); }

	private:
		Thread* m_thread; //, The thread this object is processed on
	};

} //END: kismo

#endif