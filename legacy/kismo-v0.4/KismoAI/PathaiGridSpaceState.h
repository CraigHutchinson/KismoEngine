/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoAI\PathaiGridSpaceState.h
 * Status: unique (no other copy of this file name in the archive)
 */
/** \author Craig Hutchinson \file 
	\brief 
*/
#ifndef PATHAI_GRIDPSPACESTATE_H
#define PATHAI_GRIDPSPACESTATE_H

#include "PathaiState.h"
#include "Vector2.h"
#include "Texture.h"

namespace pathai
{
	//forward delcaration
	class GridSpace;

	/** \brief A Grid position state for path finding using positions
	*/
	class GridSpaceState : public State
	{
		friend class GridSpace; ////< Grid space needs access to the set the position
	public:

		/** \brief Deafult constructor we need for array creation
		*/
		GridSpaceState() {}

		/** \brief State constructor takes a position so that a distance heuristic
			can be used for reaching the goal fatser
		*/
		GridSpaceState( frx::Point2i pos ) :
			m_position( pos )
		{}

		/** get the position of this grid state
		*/
		const frx::Point2i& getPosition() const
		{ return m_position; }

		/** \brief Gets the squared distance to the goal state
		*/
		Real getDist( const State* goalState ) const
		{ return (m_position - ((GridSpaceState*)goalState)->getPosition() ).length(); } 

		/** \brief Get the heuristic priority of the state
			\remarks For now we use a basic distance heuuristic with cost effecting the next path
			iteration instead of being used here. this is advantageous if the goal node is of a high cost
		*/
		virtual Real getPriority( const State* goalState ) const
		{ return getDist( goalState ); }

		/** \brief Priority operator
			\remarks bases upon the distance and the cost
		*/
		//virtual Bool lessPriority( const State* rhs, const State* goalState ) const
		//{ return ( getCost() + getDistSquared( goalState )) < (((GridSpaceState*)rhs)->getDistSquared( goalState ) + rhs->getCost()); }

	protected:
		
		/** \brief helper function for setting the position of the state for distance calculations
		*/
		void setPosition( UInt16 x, UInt16 y )
		{ m_position.x = x; m_position.y = y; }


	protected:
		frx::Point2i m_position; ////< The position of the grid state
	};

} //END: lexai

#endif