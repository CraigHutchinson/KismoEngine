/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\KismoCore\_SOURCE_\KismoFramework\AABox.cpp
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


#include "AABox.hpp"
#include "Sphere.h"

namespace kismo
{
	template < typename _Real >
	AABox<_Real>::AABox( const Sphere<_Real>& sphere ) :
		m_min( sphere.center() - sphere.radius() ),
		m_max( sphere.center() + sphere.radius() )
	{
	}

	template < typename _Real >
	void AABox<_Real>::expand( const Sphere<_Real>& sphere )
	{
		expand( sphere.center() + sphere.radius() );
		expand( sphere.center() - sphere.radius() );
	}

	template < typename _Real >
	void AABox<_Real>::closestPoint( const _Vector& point, _Vector& closestPoint  ) const
	{
		//Check each coordinate axis and clamp to the box
		for ( unsigned int i = 0; i < 3; ++i )
		{
			_Real v = point[i];
			if ( v < m_min[i] ) v = m_min[i]; // v = max(v, min[i])
			if ( v > m_max[i] ) v = m_max[i]; // v = max(v, min[i])
			closestPoint[i] = v;
		}
	}

	template < typename _Real >
	_Real AABox<_Real>::distanceSquared( const _Vector& c ) const
	{
		//Calculate the distance outside the box for each axis
		_Real squaredDist = (_Real)0.0;
		for ( unsigned int i = 0; i < 3; ++i )
		{
			_Real v = c[i];
			if ( v < m_min[i] ) 
				squaredDist += (m_min[i] - v); 

			if ( v > m_max[i] ) 
				squaredDist += sqr( v - m_max[i] );
		}
		return squaredDist;
	}

	template class AABox< Float32 >;
	template class AABox< Float64 >;

} // END: math
