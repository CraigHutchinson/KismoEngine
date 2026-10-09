/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoFRX\AABox.cpp
 * Status: superseded by kismo-core/_SOURCE_/KismoFramework/AABox.cpp (same name, content differs)
 */
#include "AABox.h"
#include "Sphere.h"

namespace kismo
{
	template < typename _TReal >
	AABox<_TReal>::AABox( const Sphere<_TReal>& sphere ) :
		m_min( sphere.center() - sphere.radius() ),
		m_max( sphere.center() + sphere.radius() )
	{
	}

	template < typename _TReal >
	void AABox<_TReal>::expand( const Sphere<_TReal>& sphere )
	{
		expand( sphere.center() + sphere.radius() );
		expand( sphere.center() - sphere.radius() );
	}

	template < typename _TReal >
	void AABox<_TReal>::closestPoint( const VectorT& point, VectorT& closestPoint  ) const
	{
		//Check each coordinate axis and clamp to the box
		for ( unsigned int i = 0; i < 3; ++i )
		{
			_TReal v = point[i];
			if ( v < m_min[i] ) v = m_min[i]; // v = max(v, min[i])
			if ( v > m_max[i] ) v = m_max[i]; // v = max(v, min[i])
			closestPoint[i] = v;
		}
	}

	template < typename _TReal >
	_TReal AABox<_TReal>::distanceSquared( const VectorT& c ) const
	{
		//Calculate the distance outside the box for each axis
		_TReal squaredDist = (_TReal)0.0;
		for ( unsigned int i = 0; i < 3; ++i )
		{
			_TReal v = c[i];
			if ( v < m_min[i] ) 
				squaredDist += sqr( m_min[i] - v  ); 

			if ( v > m_max[i] ) 
				squaredDist += sqr( v - m_max[i] );
		}
		return squaredDist;
	}

	template class AABox< Float32 >;
	template class AABox< Float64 >;

} // END: math
