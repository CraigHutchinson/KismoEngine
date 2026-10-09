/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoAI\OpsaiBreadthSearch.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
/** \author Craig Hutchinson \file 
	\brief 
*/
#include "OpsaiBreadthSearch.h"
#include "OpsaiMatcher.h"

namespace opsai
{
	Bool BreadthSearch::search( const WorldState& start, const State::Array& goals, const OperatorStore& operators )
	{
		if ( goals.size() == 0 )
		{
			logSuccess( "No Goals were supplied to search! no search required!" );
			return true;
		}

		//Make sure we aren't already at the goal
		if  ( start.lookup( goals, false ) != 0 )
		{
			//We found the goal!!
			logSuccess( "Search not required. The current world state meets the supplied goals" );
			return true;
		}

		//Create our store for the geenrated states that are ready to be searched
		WorldState::PtrBuffer ready( Index_MAX / 4 );
		ready.push( &start );

		WorldState::PtrLookup visited; ////< Set of visited states
		WorldState* pstart = new WorldState( start );
		visited.insert( pstart); //Insert the inital state into the visited list

		const WorldState* result = search( ready, visited, goals, operators );

		if ( result )
		{
			//Iterate over the parent states to build the operator list to reach this goal state
			OperatorResults opResults;
			const WorldState* resultNode = result;
			while ( resultNode )
			{
				if ( resultNode->getOperator() )
					opResults.push_back( OperatorMatch( resultNode->getOperator(), resultNode->getMatchData() ) );
				resultNode = resultNode->getParentState();
			}

			//Now put the results into the result array in the correct order as we just read them backwards
			for ( opsai::Search::OperatorResults::const_reverse_iterator iResult = opResults.rbegin();
				 iResult != opResults.rend();
				 ++iResult )
			{
				m_resultOperators.push_back( *iResult );				
			}

			//Store the resulting goal state
			m_resultState = *result;
			m_resultState.cleanup();
		}
		else
		{
			logError( "Goal could not be reached!" );
		}

		if ( visited.size() > 2048 )
		{
			logWarning( "Search space large. Please wait while memory is reclaimed" );
		}
			
		//Delete all the states and reclaim the memory
		for ( WorldState::PtrLookup::iterator iState = visited.begin();
			iState != visited.end();
			++iState )
		{
				delete *iState;
		}
		
		return result != 0;
	}

	const WorldState* BreadthSearch::search( WorldState::PtrBuffer& ready, WorldState::PtrLookup& visited, const State::Array& goals, const OperatorStore& operators )
	{
		Matcher matcher; ///< The matcher instance we are using

		UInt32 stateCount = 0;
		WorldState* newWorldState = 0; ///< Temporary store for generating a new world state

		while( !ready.empty() && ready.size() < ready.capacity() ) //Loop while there are states
		{
			//Pop the next state from the queue (just use a vector to save allocations at the moment)
			const WorldState* nextWorldState = ready.front(); ////< Pointer to the next state
			++stateCount;

			//Generate all possible states from this state
			for ( opsai::OperatorStore::const_iterator iOperator = operators.begin();
				iOperator != operators.end();
				++iOperator )
			{
				//Check the operators preconditions match
				if ( (*iOperator)->validatePreconditions( matcher, *nextWorldState ) )
				{
					//Create all the successor states and add them to the ready list if they ahvn't been visited										
					for ( State::MatchArray::const_iterator iMatch = matcher.getMatches().begin();
						iMatch != matcher.getMatches().end();
						++iMatch )
					{
						//Create the new world state
						if ( newWorldState == 0 )
							newWorldState = new WorldState( *nextWorldState );
						else
							*newWorldState = *nextWorldState;

						//Create the new successor state
						if ( !(*iOperator)->createSuccessorState( *newWorldState, *iMatch ) )
						{
							logError( "Failed to create successor state using operator '" << (*iOperator)->getName() << "'" );
							return 0;
						}

						//Now to make sure the new state doesn't already exist
						if ( visited.find( newWorldState ) == visited.end() ) 
						{
							visited.insert( newWorldState );

							//Add the operator to the state so we know how we got it
							newWorldState->setOperator( (*iOperator), *iMatch );

							//Check if the state meets the goal requirements
							if ( newWorldState->lookup( goals, false ) != 0 )
							{
								//We found the goal!!
								//logSuccess( "We found the goal after searching " << stateCount << " states" );
								return newWorldState;
							}

							//... add it if not
							ready.push( newWorldState );
							newWorldState = 0;
						}
						else
						{
							//The state has already been visited
						}
					}
				}

			}

			//We no longer need the full state definition so compact it to save memory
			(const_cast< WorldState* >(ready.front()))->compact();

			ready.pop(); ///Pop the state
		}

		if ( newWorldState )
		{
			delete newWorldState;
		}

		if ( !(ready.size() < ready.capacity()) )
		{
			logWarning( "Maximum state queue size reached!" );
		}

		return 0; ///< We didn't find the goal state

	}


} //END: opsai