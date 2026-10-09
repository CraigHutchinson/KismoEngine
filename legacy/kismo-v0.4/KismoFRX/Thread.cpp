/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoFRX\Thread.cpp
 * Status: superseded by kismo-core/_SOURCE_/KismoParallel/Thread.cpp (same name, content differs)
 */
#include "KismoFRX/Thread.h"
#include <cassert>
#include <windows.h>

namespace kismo
{
	Count Thread::kDestructorTimeoutMs = 10000;
	
	Thread::ePriority Thread::getThreadPriorityFromName( const String& value )
	{
		if( value.length()==0 )
			return Priority_Normal; //Default 'other control type'
		if ( !_tcsicmp( value.c_str(), _T("Idle") ) )
			return Priority_Idle;
		if ( !_tcsicmp( value.c_str(), _T("Low") ) )
			return Priority_Low;
		if ( !_tcsicmp( value.c_str(), _T("BelowNormal") ) )
			return Priority_BelowNormal;
		if ( !_tcsicmp( value.c_str(), _T("Normal") ) )
			return Priority_Normal;
		if ( !_tcsicmp( value.c_str(), _T("AboveNormal") ) )
			return Priority_AboveNormal;
		if ( !_tcsicmp( value.c_str(), _T("High") ) )
			return Priority_High;
		if ( !_tcsicmp( value.c_str(), _T("TimeCritical") ) )
			return Priority_TimeCritical;
		return Priority_Normal;

	}

	Thread::Thread() : 
		m_runnable( 0 ),
		m_mutex(0), 
		m_handle(0),
		m_id(0), 
		m_running(false),
		m_sleep(0),
		m_event(0),
		m_priority( Priority_Normal ),
		m_processorAffinityMask( 0xffffffff ),
		m_lastUpdateReturn( true ),
		m_preferredProcessor( -1 )
	{
	}
		
	Thread::~Thread()
	{
		stop( kDestructorTimeoutMs );
	}
		
	UInt32 Win32API Thread::staticThreadProcess( LPVOID lpThreadParameter )
	{
		Thread& thread = *(Thread*)lpThreadParameter;
		return thread.threadProcess();
	}
	
	UInt32 Thread::threadProcess()
	{
		m_runnable->onThreadStart();

		while ( isRunning() == true )
		{
			waitOnUpdateEvent();

			//Prevent thread issues
			//For performance the lock and release should be tight within the devices thread update
			//lock( INFINITE );

			m_lastUpdateReturn = m_runnable->threadUpdate();

			//Allow other threads to interface witht eh device data
			//For performance the lock and release should be tight within the devices thread update
			//release();

			//OutputDebugString("updated canBus\n" );
			Sleep( m_sleep );
		}

		m_runnable->onThreadEnd();

		return 0;
	}

	bool Thread::setThreadableObject( Runnable* device  )
	{
		if ( m_runnable == device )
			return true; //, ThreadableObject already set

		lock();
		m_runnable = device;
		if ( m_runnable )
			m_runnable->setThread( this );
		release();
		return true;
	}

	bool Thread::start( const Char* mutexName )
	{
		if ( m_running && m_handle )
			return true;

		assert( m_handle == 0 );
		assert( m_running == false );

		m_running = true; // global initalised flag must be set to reset the cockpit to defaults

		m_runnable->preThreadStart();

		//Start the cockpit thread
		m_handle = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)staticThreadProcess, this, 0, (LPDWORD)&m_id );
		m_running = (m_handle != 0);

		if ( m_running && m_mutex == 0 )
		{
			//Create the cockpitControlsThread m_mutex to allow use to prevent overlapping operations
			m_mutex = CreateMutex(NULL, false, mutexName);
		}

		//Set the threads priority which won't succeed if we aren't running
		setPriority( m_priority );
		setProcessorAffinity( m_processorAffinityMask );
		setPreferredProcessor( m_preferredProcessor );

		return m_running;
	}

	bool Thread::setProcessorAffinity( UInt64 processorAffinityMask )
	{
		m_processorAffinityMask = processorAffinityMask; 
		if ( !m_handle )
		{
			return true;
		}

		return SetThreadAffinityMask( m_handle, (DWORD_PTR)m_processorAffinityMask ) != 0;
	}
	
	bool Thread::setUpdateSleep( Count sleep )
	{ 
		m_sleep = sleep;
		return true; 
	}


	bool Thread::setPreferredProcessor( UInt8 preferredProcessor )
	{
		m_preferredProcessor = preferredProcessor;
		if ( !m_handle )
		{ 
			return true;
		}

		if ( m_preferredProcessor < 0 )
			return true;

		return SetThreadIdealProcessor( m_handle, (DWORD)m_preferredProcessor ) != (DWORD)-1;
	}

	bool Thread::setPriority( ePriority prority )
	{
		m_priority = prority;
		if ( !m_handle )
		{
			return true;
		}

		int win32Prority = THREAD_PRIORITY_NORMAL;
		switch( prority )
		{
			case Priority_Idle:
				win32Prority = THREAD_PRIORITY_IDLE;
				break;
			case Priority_Low:
				win32Prority = THREAD_PRIORITY_LOWEST; 
				break;
			case Priority_BelowNormal:
				win32Prority = THREAD_PRIORITY_BELOW_NORMAL; 
				break;
			case Priority_Normal:
				win32Prority = THREAD_PRIORITY_NORMAL; 
				break;
			case Priority_AboveNormal:
				win32Prority = THREAD_PRIORITY_ABOVE_NORMAL; 
				break;
			case Priority_High:
				win32Prority = THREAD_PRIORITY_HIGHEST;
				break;
			case Priority_TimeCritical:
				win32Prority = THREAD_PRIORITY_TIME_CRITICAL; 
				break;
			default:
				assert( false );
		}

		return SetThreadPriority( m_handle, win32Prority ) != 0;
	}


	bool Thread::stop( Count timeOut )
	{
		if ( !m_running && m_handle == 0 )
			return false;

		assert( m_running == true && m_handle );

		m_running = false;

		if ( m_mutex ) ///, Destroy the thread m_mutex to make sure we clean everythign up
		{
			CloseHandle( m_mutex );
			m_mutex = 0;
		}
		
		if ( m_event ) ///, Destroy the thread m_event to make sure we clean everythign up and that the thread exists cleanly
		{
			CloseHandle( m_event );
			m_event = 0;
		}

		if ( m_handle )
		{			
			if ( timeOut == Count_Max )
				timeOut = INFINITE;

			if ( WaitForSingleObject( m_handle, timeOut ) != WAIT_OBJECT_0 )
			{
				//Forcefully termiante the thread
				TerminateThread( m_handle, -1 );
			}
			CloseHandle( m_handle );
			m_handle = 0;
		}
		return true;
	}

	bool Thread::isCurrent() const
	{ return GetCurrentThreadId() == m_id; }

	bool Thread::lock( Count timeOut  )
	{
		if ( m_mutex == 0 )
			return true;

		if ( timeOut == Count_Max )
			timeOut = INFINITE;

		switch( WaitForSingleObject( m_mutex, timeOut ) )
		{
		case WAIT_TIMEOUT:
		case WAIT_FAILED:
			return false;
			break;
		case WAIT_ABANDONED:// drop through
			break;
		case WAIT_OBJECT_0:
		default:
			break;
		}

		return true;
	}

	bool Thread::release()
	{
		if ( m_mutex == 0 )
			return true;

		return ReleaseMutex(m_mutex) != 0;
	}



	bool Thread::createUpdateEvent( bool initialState, const Char* eventName )
	{
		if ( m_event != 0 )
		{
			HANDLE tevent = m_event; //, Create temporary to prevent user threads accessing invalid m_event m_handle
			m_event = 0;
			CloseHandle( tevent );
		}

		m_event = CreateEvent(NULL, FALSE, (initialState?TRUE:FALSE), eventName );
		return m_event != 0;
	}

	bool Thread::triggerUpdateEvent()
	{
		if ( m_event == 0 )
			return false;

		return SetEvent( m_event ) != 0;
	}

	bool Thread::waitOnUpdateEvent( Count timeOut )
	{	
		if ( m_event == 0 )
			return false;

		if ( timeOut == Count_Max )
			timeOut = INFINITE;

		switch( WaitForSingleObject( m_event, timeOut ) )
		{
		case WAIT_TIMEOUT:
		case WAIT_FAILED:
			return false;
			break;
		case WAIT_ABANDONED:// drop through
			break;
		case WAIT_OBJECT_0:
		default:
			break;
		}

		return true;
	}

	Runnable::Runnable() : 
		m_thread(0)
	{
	}
	Runnable::~Runnable()
	{
		setThread(0);
	}

	bool Runnable::setThread( Thread* thread )
	{
		if ( m_thread != thread )
		{
			if ( m_thread )
			{
				delete m_thread;
				m_thread = 0;
			}
		
			m_thread = thread;

			if ( m_thread )
			{
				if ( !m_thread->setThreadableObject( this ) )
				{
					delete m_thread;
					return false;
				}
			}
		}

		return true;
	}

	bool Runnable::createThread( Thread::ePriority priority, Count updateSleep, bool useUpdateEvent, UInt64 processorAffinitymask, UInt8 preferredProcessor )
	{
		if ( m_thread )
		{
			delete m_thread;
			m_thread = 0;
		}

		m_thread = new Thread();
		if ( !m_thread )
			return false;

		m_thread->setUpdateSleep( updateSleep );
		m_thread->setPriority( priority );
		m_thread->setProcessorAffinity( processorAffinitymask );
		m_thread->setPreferredProcessor( preferredProcessor );

		if ( !m_thread->setThreadableObject( this ) )
		{
			delete m_thread;
			m_thread = 0;
		}

		if ( !m_thread->start() )
		{
			delete m_thread;
			m_thread = 0;
		}

		return m_thread != 0;
	}

} //END: kismo