/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\Framework\BoundingBox.cpp
 * Status: superseded by kismo-v0.3/Math/BoundingBox.cpp (same name, content differs)
 */
#include "BoundingBox.h"
#include "BoundingSphere.h"

namespace frx
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
	void BoundingBox<_TReal>::closestPoint( Vector3< _TReal >& p, const Vector3< _TReal >& c ) const
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
	_TReal BoundingBox<_TReal>::distanceSquared( const Vector3< _TReal >& c ) const
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

} // END: frx
