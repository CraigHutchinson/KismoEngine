/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\KismoCore\_SOURCE_\KismoFramework\EventManager.cpp
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


#include "EventManager.h"
#include "KismoFramework\Common.h"

IMPL_SINGLETON( kismo::EventManager );
IMPLEMENT_UNIQUE( kismo::Event );

namespace kismo
{
	IMPLEMENT_BASE_RTTI( core, Event );
	IMPLEMENT_BASE_RTTI( core, EventAction );

	Event::Event( EventManager* manager, Bool oneOff ) :
		m_manager( manager ),
		m_oneOff( oneOff )
	{
		m_manager->registerEvent( this );
	}

	Event::~Event()
	{
		removeAllActions();
		m_manager->removeEvent( this );
	}

	void Event::trigger( Real dt, Real tApp )
	{
		//execute all the actions for this event
		EventAction::Lookup tLookup = m_actions;
		for ( EventAction::Lookup::const_iterator iAction = tLookup.begin(); iAction != tLookup.end(); ++iAction )
		{
			(*iAction)->doExec( this, dt, tApp );
		}

		//If the event is one off we need to delete all the actions that are left
		if ( m_oneOff )
		{
			removeAllActions();
		}				
	}

	void Event::registerAction( EventAction* newAction )
	{
		if ( !newAction )
			return;

		if ( m_actions.find( newAction ) != m_actions.end() )
			return;

		m_actions.insert( newAction );
		newAction->registerEvent( this );
	}

	void Event::removeAllActions()
	{
		for( EventAction::Lookup::iterator iAction = m_actions.begin(); iAction != m_actions.end(); iAction = m_actions.begin())
		{
			EventAction* action = *iAction;
			Assert( action );
			m_actions.erase( iAction);
			action->removeEvent( this );
		}

		Assert( m_actions.empty() );
	}

	void Event::removeAction( EventAction* action )
	{
		if ( !action )
			return;

		EventAction::Lookup::iterator iAction = m_actions.find( action );
		if ( iAction == m_actions.end() )
			return;
		
		m_actions.erase( iAction);
		action->removeEvent( this );

	}

	EventAction::EventAction( Bool eventManaged , Bool oneOff  ) :
		m_oneOff( oneOff ),
		m_eventManaged( eventManaged )
	{
	}

	EventAction::~EventAction()
	{
		removeAllEvents();
	}
	
	void EventAction::doExec( Event* event, Real dt, Real tApp )
	{		
		exec( event, dt, tApp );

		if ( isOneOff() )
		{	
			removeAllEvents();
			
			if ( isEventManaged() )
				delete this; //< Auto destroy!
		}
	}

	void EventAction::removeAllEvents()
	{
		for ( Event::Lookup::iterator iEvent = m_events.begin(); iEvent != m_events.end(); iEvent = m_events.begin() )
		{
			Event* event = *iEvent;
			Assert( event );
			m_events.erase( iEvent );
			event->removeAction( this );	
		}
		
		Assert( m_events.empty() );
	}
	
	void EventAction::registerEvent( Event* event )
	{
		if ( !event )
		{
			Assert( false );
			return;
		}

		Event::Lookup::iterator iEvent = m_events.find( event );

		if ( iEvent == m_events.end() )
		{
			m_events.insert( event );
			event->registerAction( this );
		}
	}

	void EventAction::removeEvent( Event* event )
	{
		if ( !event )
		{
			Assert( false );
			return;
		}

		Event::Lookup::iterator iEvent = m_events.find( event );

		if ( iEvent == m_events.end() )
			return;

		m_events.erase( iEvent );
		event->removeAction( this );

		if ( m_eventManaged && m_events.empty() )
			delete this; //< Auto destroy!
	}

	//------------------------------------------------------------------------------
	// EventManager
	//------------------------------------------------------------------------------

	EventManager::EventManager( UInt32 bufferBytes )
	{
	}

	void EventManager::update( Real tDelta, Real tApp )
	{
		for ( Event::Lookup::iterator iEvent = m_events.begin(); iEvent != m_events.end(); ++iEvent )
		{
			if ( (*iEvent)->hasActions() ) //Only update if there are actions to fire
				(*iEvent)->update( tDelta, tApp ); //update the event
		}
	}

	void EventManager::registerEvent( Event* event )
	{
		m_events.insert( event );
	}

	void EventManager::removeEvent( Event* event )
	{
		m_events.erase( m_events.find( event ) );
	}

} //END: kismo
