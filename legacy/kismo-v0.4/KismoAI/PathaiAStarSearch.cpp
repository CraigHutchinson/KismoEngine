/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoAI\PathaiAStarSearch.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
/** \author Craig Hutchinson \file 
	\brief 
*/
#include "PathaiAStarSearch.h"
#include "PathaiPath.h"

namespace pathai
{

	AStarSearch::AStarSearch( Space& searchSpace ) :
		Search( searchSpace ) 
	{}

	Bool AStarSearch::search( const State* start, const State* goal )
	{
		//.The open and closed node list
		Path::GoalLookup openList;
		Path::Lookup closedList;

		//Set the goal node for sorting the open list by priority
		openList.comp.goal = goal;

		Path* result = search( openList, closedList, start, goal );

		if ( result )
		{
			//Clear the result array
			m_resultStates.clear();
			
			State::Array results;
			const Path* resultNode = result;
			while ( resultNode )
			{
				results.push_back( resultNode->getState() );
				resultNode = resultNode->getParent();
			}

			//Now put the results into the result array in the correct order as we just read them backwards
			for ( State::Array::const_reverse_iterator iResult = results.rbegin();
				 iResult != results.rend();
				 ++iResult )
			{
				m_resultStates.push_back( *iResult );				
			}

			//Clear the debuggin information
			// We have this so we can display the states that are open or closed with the results
			m_resultClosedStates.clear();
			m_resultOpenStates.clear();

			//Delete all the path nodes that were searched reclaiming the memory and removing any possible memory leaks
			for ( Path::GoalLookup::iterator iPath = openList.begin(); iPath != openList.end(); ++iPath ) 
			{ 
				//For debugging we store a liist of the opened states
				m_resultOpenStates.push_back( (*iPath)->getState() );
			}
			for ( Path::Lookup::iterator iPath = closedList.begin(); iPath != closedList.end(); ++iPath )
			{ 
				//For debugging we store a liist of the close states
				m_resultClosedStates.push_back( (*iPath)->getState() );
			}

			//Now to delete the result node which wion't be in the open or closed list yet as we exit as early as possible at the moment
			delete result;
		}
		else
		{
			logError( "Failed to reach goal while performing A* search!" );
		}

		//Delete all the path nodes that were searched reclaiming the memory and removing any possible memory leaks
		for ( Path::GoalLookup::iterator iPath = openList.begin(); iPath != openList.end(); ++iPath ) 
		{ delete *iPath; }
		for ( Path::Lookup::iterator iPath = closedList.begin(); iPath != closedList.end(); ++iPath )
		{ delete *iPath; }

		return result != 0;
	}


	Path* AStarSearch::search( Path::GoalLookup& openList, Path::Lookup& closedList, const State* start, const State* goal )
	{
		//Array for storing the successors temporarily
		State::Array successors;

		//Create a lookup map.
		// This is needed as the Search is seperate from the searchSpace implementation
		// which makes it possible to perform many searches at one time without multithreading
		// becomeing an issue and the need for a duplicate tile grid or the likes
		StatePathMap pathMap;		

		//Store the start state in the open list and lookup map
		Path* newPath = new Path( start );
		openList.insert( newPath );
		pathMap[ start ] = newPath;		

		while ( !openList.empty() )
		{
			//Get the next path with the lowest cost priority
			Path* nextPath = *openList.begin();
						
			//Erase the open node
			openList.erase( openList.begin() );

			//Add to the closed node list
			closedList.insert( nextPath );

			//Get the successor states
			m_searchSpace.getSuccessors( nextPath->getState(), successors );

			//Add the new successor states to the open list
			for ( State::Array::const_iterator iSuccessor = successors.begin();
				iSuccessor != successors.end();
				++iSuccessor )
			{
				newPath = new Path( nextPath, *iSuccessor );

				//Check if we have found the goal!
				if ( *iSuccessor == goal )
				{
					logSuccess( "We got to the goal OK!" );
					return newPath;
				}

				StatePathMap::iterator iMatch = pathMap.find( *iSuccessor ); ///Check if the state already exists
				if ( iMatch == pathMap.end() )
				{
					//The state hasn't been searched so we add it to the open list
					openList.insert( newPath );
					pathMap[ *iSuccessor ] = newPath;
				}
				else //If the state already exists we check if the cost is less than our new one or not
				if ( nextPath->getCost() > iMatch->second->getCost() )
				{
					Path* oldNode = iMatch->second; 
					assert( oldNode );

					//Re-link the node to the cheaper route
					nextPath->setParent( oldNode );

					//We need to replace the links to this path node with our new cheaper one
					//for ( Path::const_iterator iChild = newPath->childBegin();
					//	iChild != newPath->childEnd();
					//	++iChild )
					//{
					//	(*iChild)->setParent( oldNode );					
					//}

					//Replace the path within the map
					//iMatch->second = newPath;

					//This path is more expensive so don't replace the existing
					delete newPath;
				}
				else
				{
					//This path is more expensive so don't replace the existing
					delete newPath;
				}
				
				newPath = 0;
			}
			successors.clear();
		}

		return 0; //We didn't reach teh goal before we ran out of successors!
	}


} //END: pathai