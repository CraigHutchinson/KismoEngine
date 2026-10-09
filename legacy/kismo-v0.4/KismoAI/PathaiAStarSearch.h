/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoAI\PathaiAStarSearch.h
 * Status: unique (no other copy of this file name in the archive)
 */
/** \author Craig Hutchinson \file 
	\brief 
*/
#ifndef PATHAI_ASTARSEARCH_H
#define PATHAI_ASTARSEARCH_H

#include "PathaiSearch.h"
#include "PathaiPath.h"

namespace pathai
{

	/** \brief Base path search interface
	*/
	class AStarSearch : public Search
	{
	public:
		
		typedef Map< const State*, Path* > StatePathMap;

	public:

		AStarSearch( Space& searchSpace );

		/** \brief Search interface function
			\param[in] start the starting state
			\param[in] goal The target goal state
			\returns Whether the goal was reached successfully
		*/
		virtual Bool search( const State* start, const State* goal );


	protected:

		/** \brief AStar search implementation
		*/
		Path* search( Path::GoalLookup& openList, Path::Lookup& closedList, const State* start, const State* goal );

	};

} //END: pathai

#endif