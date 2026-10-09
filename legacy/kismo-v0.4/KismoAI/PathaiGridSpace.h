/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoAI\PathaiGridSpace.h
 * Status: unique (no other copy of this file name in the archive)
 */
/** \author Craig Hutchinson \file 
	\brief 
*/
#ifndef PATHAI_GRIDSPACE_H
#define PATHAI_GRIDSPACE_H

#include "PathaiSpace.h"
#include "PathaiGridSpaceState.h"
#include "TileGrid.h"

namespace pathai
{

	/** \brief Grid for GridPositionStates path search
	*/
	class GridSpace : public Space
	{
	public:

		typedef GridSpaceState State_T; ///< The type of state node we use with this search space
		typedef world::TileGrid< State_T > TileGrid; ////< The state search space

	public:
		
		/** \brief Grid search space constrructor
			\param[in] size The size of the search space grid
		*/
		GridSpace( frx::Size2i size );

		/** \copydoc Space::getSuccessors()
		*/
		virtual Count getSuccessors( const State* source, State::Array& successors );

		/** \brief Get a pointer to a tile and return 0 if the location is invalid
		*/
		GridSpaceState* getGridState( UInt16 x, UInt16 y, UInt16 z = 0 )
		{
			if ( x < m_grid.getWidth() && y < m_grid.getHeight() && z < m_grid.getDepth() )
				return &m_grid.getTile( x, y, z );
			else
				return 0;
		}


	protected:

		TileGrid m_grid; ////< The grid of nodes

	};


} //END: lexai

#endif