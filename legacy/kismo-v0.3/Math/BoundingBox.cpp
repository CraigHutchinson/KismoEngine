/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\Math\BoundingBox.cpp
 * Status: current (newest copy of this file name in the archive)
 */
#include "BoundingBox.h"
#include "BoundingSphere.h"

namespace math
{
	template < typename _TReal >
	BoundingBox<_TReal>::BoundingBox( const BoundingSphere<_TReal>& sphere ) :
		m_min( sphere.center() - sphere.radius() ),
		m_max( sphere.center() + sphere.radius() )
	{
	}

	template < typename _TReal >
	void BoundingBox<_TReal>::expand( const BoundingSphere<_TReal>& sphere )
	{
		expand( sphere.center() + sphere.radius() );
		expand( sphere.center() - sphere.radius() );
	}

	template < typename _TReal >
	void BoundingBox<_TReal>::closestPoint( VectorN<_TReal,3 >& p, const VectorN<_TReal,3 >& c ) const
	{
		//Check each coordinate axis and clamp to the box
		for ( unsigned int i = 0; i < 3; ++i )
		{
			_TReal v = c[i];
			if ( v < m_min[i] ) v = m_min[i]; // v = max(v, min[i])
			if ( v > m_max[i] ) v = m_max[i]; // v = max(v, min[i])
			p[i] = v;
		}
	}

	template < typename _TReal >
	_TReal BoundingBox<_TReal>::distanceSquared( const VectorN<_TReal,3 >& c ) const
	{
		//Calculate the distance outside the box for each axis
		_TReal squaredDist = (_TReal)0.0;
		for ( unsigned int i = 0; i < 3; ++i )
		{
			_TReal v = c[i];
			if ( v < m_min[i] ) 
				squaredDist += math::sqr( m_min[i] - v  ); 

			if ( v > m_max[i] ) 
				squaredDist += math::sqr( v - m_max[i] );
		}
		return squaredDist;
	}

	template class BoundingBox< Float32 >;
	template class BoundingBox< Float64 >;

} // END: math
