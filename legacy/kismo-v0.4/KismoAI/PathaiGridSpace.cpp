/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoAI\PathaiGridSpace.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
/** \author Craig Hutchinson \file 
	\brief 
*/
#include "PathaiGridSpace.h"

/** \brief Array of successor state positions in the 8 directions of the grid
*/
frx::Point2i successorOffsets[8] = {
	frx::Point2i( -1,1 ),	frx::Point2i( 0,-1 ),	frx::Point2i( 1,1 ),
	frx::Point2i( -1,0 ),							frx::Point2i( 1,0 ),
	frx::Point2i( -1,-1 ),	frx::Point2i( 0,1 ),	frx::Point2i( 1,-1 )
};


namespace pathai
{
	GridSpace::GridSpace( frx::Size2i size ) :
		m_grid( size )
	{
		//Set the position information within each tile as we need it for getting the distance between states
		for ( UInt16 y = 0; y < size.h; ++y )		
			for ( UInt16 x = 0; x < size.w; ++x )
				m_grid.getTile( x, y ).setPosition( x, y );
	}

	Count GridSpace::getSuccessors( const State* source, State::Array& successors )
	{
		GridSpaceState* gridState = (GridSpaceState*)source; ///Convert the source state to our type

		Count count = 0;
		for ( Index i = 0; i < 8; ++i )
		{
			const GridSpaceState* newState = getGridState( gridState->getPosition().x + successorOffsets[i].x,  
				gridState->getPosition().y + successorOffsets[i].y ); 

			//If the tile psoition is valid and not a wall add it to the successor list
			if ( newState && newState->getCost() < Int8_MAX ) //If the tile psoition is valid radd it to the successor list
			{
				successors.push_back( newState );
				++count;
			}
		}
		return count; 
	}

} //END: pathai
