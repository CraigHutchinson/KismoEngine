/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\Framework\Thread.cpp
 * Status: superseded by kismo-core/_SOURCE_/KismoParallel/Thread.cpp (same name, content differs)
 */
#include "Thread.h"
#include "Framework/Common.h"

namespace frx
{
	Threaded::Threaded() : 
		m_thread( 0 )
	{
	}

	Threaded::~Threaded()
	{ 
		//assert( m_thread.isNull() );
	}

	Bool Threaded::attachToThread( Thread* thread, Priority priority )
	{ 
		assert( thread );
		if ( m_thread.getPtr() == thread ) 
			return isValidPointer( thread ); //Already attached to thread!

		detatchFromThread(); //detatch from the current if it exists
		m_thread = thread;

		return m_thread->addThreadedObject( this, priority ); 
	}

	void Threaded::detatchFromThread()
	{
		if ( m_thread.isValid() )
		{
			m_thread->removeThreadedObject( this );
			m_thread = 0;
		}
	}

	Bool Threaded::sleep( UInt32 milliseconds )
	{
		platform::sleep( milliseconds );
		return true;
	}

	//---------------------------------------------------------------------------
	// Thread
	//---------------------------------------------------------------------------
	Thread::Thread( const String& name, eThreadPriority priority ) : 
		Named( name ) ,
		m_threadedMutex( Mutex::create( name ) )
	{}

	Thread::~Thread()
	{
		assert( !isThreadRunning() );
	}

	Bool Thread::addThreadedObject( Threaded* obj, Priority priority )
	{
		//TODO: This needs doing with priority etc
		m_threadedObjects.push( obj );

		return true;
	}

	Bool Thread::removeThreadedObject( Threaded* obj )
	{
		frx::Threaded::PtrLFQueue::Node* deformerNode;
		while ( deformerNode = m_threadedObjects.pop() )
		{
			if ( deformerNode->value == obj )
			{
				delete deformerNode;
				return true;
			}
			else
			{
				//Add the node back onto the list
				m_threadedObjects.push( deformerNode );
			}
		}
		return false;

	}

	Bool Thread::setThreadPriority( eThreadPriority priority )
	{
		m_priority = priority;
		if ( isThreadRunning() )
			return setThreadPriorityImpl( priority );
		return true;
	}


	Bool Thread::startThread()
	{
		if ( isThreadRunning() )
		{
			assert( false );
			return false;
		}
		return startThreadImpl();
	}

	Bool Thread::stopThread( Bool force )
	{
		if ( !isThreadStarted() )
			return false;

		m_state[ ThreadState_ShouldStop ] = true;

		bool ret;
		if ( !force )
			ret = waitThread() && closeThreadImpl();
		else
			ret = closeThreadImpl();

		m_state = 0;
		return 0;
	}

	Bool Thread::pauseThread()
	{
		if ( isThreadRunning() )
			return pauseThreadImpl();
		return false;
	}

	Bool Thread::waitThread()
	{
		if ( isThreadRunning() )
			return waitThreadImpl();
		return false;
	}

	Int32 Thread::threadEntry()
	{
		m_state[ ThreadState_IsStarted ] = true;

		while( !shouldThreadTerminate() )
		{
			if ( isThreadPaused() )
			{
				while ( isThreadPaused() ) //For now pausing is  just a cycle on the spot //TODO: mutex event
					sleepImpl( 100 );
			}
			
			frx::Threaded::PtrLFQueue::Node* deformerNode;
			while ( deformerNode = m_threadedObjects.pop() )
			{
				//Execute a phase of the threaded object
				if ( deformerNode->value->threadPhase() ) //If we get a false returned we should remove the object
				{
					m_threadedObjects.push( deformerNode );
				}
				else
				{
					delete( deformerNode ); ///< Done with the onbject
				}

				//We should bail!
				if ( shouldThreadTerminate() )
					break;
			}
			sleepImpl( 1 ); //sleep to allow the thread to release its processing
		}

		m_state[ ThreadState_IsStarted ] = false;
		return 0;
	}



} //END: frx