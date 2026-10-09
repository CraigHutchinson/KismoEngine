/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\framework\KismoBoundingBox.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
#include "KismoBoundingBox.h"
#include "KismoBoundingSphere.h"

namespace kismo
{
	namespace frx
	{
		BoundingBox::BoundingBox( const BoundingSphere& sphere ) :
			m_min( sphere.center() - sphere.radius() ),
			m_max( sphere.center() + sphere.radius() )
		{
		}

		void BoundingBox::expand( const BoundingSphere& sphere )
		{
			expand( sphere.center() + sphere.radius() );
			expand( sphere.center() - sphere.radius() );
		}

		void BoundingBox::closestPoint( Vector3& p, const Vector3& c ) const
		{
			//Check each coordinate axis and clamp to the box
			for ( unsigned int i = 0; i < 3; ++i )
			{
				k_Real v = c[i];
				if ( v < m_min[i] ) v = m_min[i]; // v = max(v, min[i])
				if ( v > m_max[i] ) v = m_max[i]; // v = max(v, min[i])
				p[i] = v;
			}
		}
	
		k_Real BoundingBox::distanceSquared( const Vector3& c ) const
		{
			//Calculate the distance outside the box for each axis
			k_Real squaredDist = (k_Real)0.0;
			for ( unsigned int i = 0; i < 3; ++i )
			{
				k_Real v = c[i];
				if ( v < m_min[i] ) 
					squaredDist += math::square( m_min[i] - v  ); 

				if ( v > m_max[i] ) 
					squaredDist += math::square( v - m_max[i] );
			}
			return squaredDist;
		}
	
	} // END: frx

} // END: kismo
