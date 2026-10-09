/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\KismoCore\KismoFramework\EventManager.h
 * Status: duplicate of kismo-core/_SOURCE_/KismoFramework/EventManager.h
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

#ifndef KISMOFRAMEWORK_EVENTMANAGER_H
#define KISMOFRAMEWORK_EVENTMANAGER_H

#include "KismoFramework/Types.h"
#include "KismoFramework/Singleton.h"
#include "KismoFramework/Rtti.h"
#include "KismoFramework/Unique.h"
#include "KismoFramework/SmallSet.h"

#define _EventManager kismo::EventManager::getSingleton()
#define _EventManagerPtr kismo::EventManager::getSingletonPtr()

namespace kismo
{
	//Forward declaration
	class Event;
	class EventAction;

	/** \brief EventManager holds all the events current in the system and can be updated to fire events as required
	*/
	class EventManager : public kismo::Singleton< EventManager >
	{
		friend class Event; ///< Give Event access to register and remove itself from a manager
	public:
		SINGLETON( EventManager );

		EventManager( UInt32 bufferBytes = 1024 * 1024);

		/** \brief Update the event and check if it should trigger its actions if the event can auto fire fromt the update
			\param[in] dt The delta time since the last update usefull for some events
		*/
		void update( Real tDelta, Real tApp );
		
	protected:

		/** \brief Register an event with the manager so the event can be updated and fired automatically etc
			\remarks This is called automatically when an event is constructed
		*/
		void registerEvent( Event* event );

		/** \brief Remove an event from the manager
			\remarks this is called automatically when an event is destroyed
			*/
		void removeEvent( Event* event );

	protected:

		kismo::SmallSet< Event* > m_events; ///< Vector of events
	};

	/** \brief Base event type
		\remarks Each event has a unique id which can be used for fast runtime identifying
	*/
	class Event : public kismo::Unique< Event >
	{
	public:
		DECLARE_ROOT_RTTI;
		friend class EventAction;


		typedef std::vector< Event* > Array;
		typedef kismo::SmallSet< Event* > Lookup;

	public:

		Event( EventManager* manager, Bool oneOff = false );

		~Event();

		/** \brief Update the event and check if it should trigger its actions if the event can auto fire fromt the update
			\param[in] dt The delta time since the last update usefull for some events
		*/
		virtual void update( Real tDelta, Real tApp ) {}

		/** \brief Trigger the event and all its actions
			\remarks If this event isOneTime() then the event and all its actions will be removed afterwards
		*/
		void trigger( Real dt = 0, Real tApp = 0 );

		/** \brief get whether the event is one off or not
		*/
		Bool isOneOff() const
		{ return m_oneOff; }

		/** \brief returns whether the event has actins
		*/
		Bool hasActions() const
		{ return !m_actions.empty(); }

		/** \brief Add an action
		*/
		void registerAction( EventAction* action );

		/** \brief Remove an action
		*/
		void removeAction( EventAction* action );

		/** \brief Clear all the actions on this event
		*/
		void removeAllActions();

		/** \brief Set whether the event is one off or not
		*/
		void setOneOff( Bool oneOff )
		{ m_oneOff = oneOff; }
	

	private:
		EventManager* m_manager; ///< A pointer to the event manager which owns his event
		kismo::SmallSet< EventAction* > m_actions; ///< The actions to trigger when the event occurs		
		Bool m_oneOff; ///< Whether the event is one off and to be removed after being triggered
	};

	/** \brief Base event action type which defines what an event does when it is triggered
	*/
	class EventAction
	{
	public:
		DECLARE_ROOT_RTTI;
		friend class Event;

		typedef kismo::SmallSet< EventAction* > Lookup; ///< Lookup set

	public:

		EventAction( Bool eventManaged = true, Bool oneOff = false );

		~EventAction();

		/** \brief Virtual execute function which occurs when the owning event is triggered and is called
			via doExec()
			\param[in] dt The delta time since the event was last called (if the event is timed)
			\remarks If isOneOff() then doExec() will delete the action after exec() is called
		*/
		virtual void exec( Event* event, Real tDelta, Real tApp ) = 0;

		/** \brief get whether the action is one off or not and to be removed once the action occurs
		*/
		Bool isOneOff() const
		{ return m_oneOff; }

		/** \brief get whether the action is managed by the event so should be deleted automaticcally when all the events
			it is listening to are deleted
		*/
		Bool isEventManaged() const
		{ return m_eventManaged; }

		/** \brief Gets whether there are veents connected to the action
		*/
		Bool hasEvents() const
		{ return !m_events.empty(); }

		/** \brief Base execute function called when an event is triggered
		*/
		void doExec( Event* event, Real dt, Real tApp );

		/** \brief Register an event with the manager so the event can be updated and fired automatically etc
			\remarks This is called automatically when an event is constructed
		*/
		void registerEvent( Event* event );

		/** \brief Remove an event from the manager
			\remarks this is called automatically when an event is destroyed
			*/
		void removeEvent( Event* event );

		/** \brief Remove a all events
		*/
		void removeAllEvents();

	protected:

		/** \brief Set whether the action is one off or not
		*/
		void setOneOff( Bool oneOff )
		{ m_oneOff = oneOff; }

	private:
		Event::Lookup m_events; //< Owning events that can trigger this action	
		Bool m_oneOff; ///< Whether the action is one off and to be removed after being executed
		Bool m_eventManaged; //< Stores whether the event manages the lifetime of the action
	};

} //END: kismo

#endif
