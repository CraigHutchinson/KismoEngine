/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoAI\OpsaiWorldState.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
/** \author Craig Hutchinson \file 
	\brief 
*/
#include "OpsaiWorldState.h"

namespace opsai
{
	IMPLEMENT_RTTI_BASE( opsai, WorldState );

	WorldState::WorldState( WorldStateLoader* loader ) :	
		m_loader( loader ),
		m_staticStateLookup( 0 ),
		m_staticStates( 0 ),
		m_parentState( 0 ),
		m_operator( 0 )
	{
	}

	//WorldState::WorldState( const State::Array* staticStates, const State::Array& dynamicStates ) :	
	//	m_loader( 0 ),	
	//	m_staticStates( 0 ),
	//	m_staticStateLookup( 0 )
	//{
	//	setStates( staticStates, dynamicStates );
	//}

	WorldState::WorldState( const WorldState& rhs ) :
		m_loader( 0 ),	
		m_staticStates( rhs.m_staticStates ),
		m_dynamicStates( rhs.m_dynamicStates ),
		m_staticStateLookup( rhs.m_staticStateLookup ),
		m_dynamicStateLookup( rhs.m_dynamicStateLookup ),
		m_parentState( &rhs ),
		m_operator( 0 )
	{
		//
	}

	WorldState::~WorldState()
	{	
		if ( m_loader )
		{
			safeDelete( m_staticStates );
			safeDelete( m_staticStateLookup );
		}
	}

	WorldState& WorldState::operator = ( const WorldState& rhs )
	{
		//Debug checks to make sure the world state is from the same scope
		assert( m_staticStates == 0 || m_staticStates == rhs.m_staticStates );
		assert( m_staticStateLookup == 0 || m_staticStateLookup == rhs.m_staticStateLookup );
		
		m_staticStates = rhs.m_staticStates;
		m_staticStateLookup = rhs.m_staticStateLookup;

		m_dynamicStates = rhs.m_dynamicStates;
		m_dynamicStateLookup = rhs.m_dynamicStateLookup;

		m_parentState = &rhs;
		m_operator = 0;

		return *this;
	}


	void WorldState::clear()
	{
		if ( m_loader )
		{
			safeDelete( m_staticStates );
			safeDelete( m_staticStateLookup );
		}

		m_staticStates = 0;
		m_staticStateLookup = 0;
		m_dynamicStates.clear();
		m_dynamicStateLookup.clear();
	}

	Bool WorldState::load()
	{
		if ( !m_loader )
		{
			logRecoverable( "No WorldState loader has been specified - operators cannot be (re)loaded" );
			return false;
		}
		
		//Clear the state
		clear();

		if ( m_loader->load( *this ) )
		{
			logSuccess( "The WorldState loaded from '" << m_loader->getFilename() << "' successfully" );
			return true;
		}
		else
		{
			logRecoverable( "The WorldState failed to load from '" << m_loader->getFilename() << "'" );
			return false;
		}
	}

	Bool WorldState::setStates( const State::Array* staticStates, const State::Array& dynamicStates )
	{
		//Make suare any exiting states are cleared
		clear();

		//Store the pointer to the static data
		m_staticStates = staticStates;

		//Reserve the intial array space and double the amount of dynamic space
		//This should save any re-allocation of this quite large buffer!
		m_dynamicStates.reserve( dynamicStates.size() * 2 );

		//Now to store the dynamic world data
		m_dynamicStates = dynamicStates;

		//Now to create the state lookup map
		
		//If this is the loader node then we will create the static map just once for this state and it will be re-used throughout
		if( m_loader )
		{
			assert( m_staticStates );
			m_staticStateLookup = new StateMap;

			Index index = 0; 
			for ( State::Array::const_iterator iState = m_staticStates->begin();
				iState != m_staticStates->end();
				++iState )
			{
				m_staticStateLookup->insert( StateHashLookup( iState->getHashValue(), index ) );
				++index;
			}

		}
		else
		{
			assert( m_staticStateLookup );
		}

		//Now to do the lookup map for the dyanmic states		
		Index index = 0; 
		for ( State::Array::const_iterator iState = m_dynamicStates.begin();
			iState != m_dynamicStates.end();
			++iState )
		{
			m_dynamicStateLookup.insert( StateHashLookup( iState->getHashValue(), index ) );
			++index;
		}

		return true;
	}

	Bool WorldState::addState( const State& state )
	{
#ifdef _DEBUG
		//TODO: Should we check for duplicate states added to the world or not?
		StateMap::const_iterator iExists = m_dynamicStateLookup.find( state.getHashValue() );
		if ( iExists != m_dynamicStateLookup.end() )
		{
			assert( false );
			return false;
		}
#endif

		//Add a lookup from the states hash value to the index within the dynamic state array
		m_dynamicStateLookup.insert( StateHashLookup( state.getHashValue(), (Index)m_dynamicStates.size() ) );

		//Store the new state within the state array
		m_dynamicStates.push_back( state );

		return true;
	}

	Count WorldState::lookup( const State::Array& goals, Bool includeStatic, Bool mustExist ) const
	{
		Count found = 0; ///< Counts the number of goals that are found
		for ( State::Array::const_iterator iGoal = goals.begin();
			iGoal != goals.end();
			++iGoal )
		{
			//Find the state within the dynamic state lookup
			StateMap::const_iterator iDynFind = m_dynamicStateLookup.find( iGoal->getHashValue() );
			if ( iDynFind != m_dynamicStateLookup.end() )
			{
				++found;
			}
			else
			if( includeStatic )
			{
				//Check the static state lookup for the state
				StateMap::const_iterator iStaticFind = m_staticStateLookup->find( iGoal->getHashValue() );
				if ( iStaticFind != m_staticStateLookup->end() )
				{
					++found;
				}
				else
				if ( mustExist )
				{
					return 0;
				}
			}
			else
			if ( mustExist )
			{
				return 0;
			}
		}

		return found;
	}

	Bool WorldState::removeAdd( const State::Array& removeStates, const State::Array& addStates )
	{ PROFILE;

		State::Array::const_iterator iAddState = addStates.begin();

		Count found = 0; ///< Counts the number of goals that are found
		for ( State::Array::const_iterator iState = removeStates.begin();
			iState != removeStates.end();
			++iState )
		{
			//Find the state within the dynamic state lookup
			StateMap::iterator iDynFind = m_dynamicStateLookup.find( iState->getHashValue() );
			if ( iDynFind != m_dynamicStateLookup.end() )
			{
				//If we found the state then we can replace it with a state from the addStates array
				if ( iAddState != addStates.end() )
				{
					//Store the new state in the place of the deleted one
					m_dynamicStates[ iDynFind->second ] = *iAddState;

					//Store the lookup to the new state
					m_dynamicStateLookup.insert(
						StateHashLookup( iAddState->getHashValue(), iDynFind->second ) );

					++iAddState; //Iterate to the next state to add
				}
				else
				{
					//We have to take the last state in the dynamicStates and put it where the deleted state is
					m_dynamicStates[ iDynFind->second ] = m_dynamicStates.back();

					//Update the lookup map accordingly by modifying the index in the lookup map to point
					// to the location of the deleted state
					StateMap::iterator iBackLookup = m_dynamicStateLookup.find( m_dynamicStates.back().getHashValue() );
					iBackLookup->second = iDynFind->second;

					//Now to shrink the dynamicState Vector to remve the last element
					m_dynamicStates.erase( m_dynamicStates.end()-1 );
				}
				
				//Remove the lookup for the deleted state
				m_dynamicStateLookup.erase( iDynFind );
			}
			else
			{
				//The state to delete does nto exist!!
				//TODO: Is this an error? for now we will jsut ignore this
				//assert( false );
				//return false;
			}
		}

		if ( iAddState != addStates.end() )
		{
			m_dynamicStates.reserve( m_dynamicStates.size() + (addStates.size() - removeStates.size() ) );

			//Add any extra states
			while ( iAddState != addStates.end() )
			{
				//Add the state
				addState( *iAddState );

				++iAddState; //Iterate to the next state to add
			}
		}

		return true;

	}

} //END: opsai