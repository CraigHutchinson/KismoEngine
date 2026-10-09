/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\framework\KismoBoundingSphere.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
#include "KismoBoundingSphere.h"
#include "KismoBoundingBox.h"

namespace kismo
{
	namespace frx
	{
		BoundingSphere::BoundingSphere( const BoundingBox& box )
		{
			m_center = box.center();
			m_radiusSquared = ( m_center - box.max() ).lengthSquared();
			m_radius = math::fastSquareRoot( m_radiusSquared );
		}
		
		void BoundingSphere::expand( const BoundingBox& box )
		{
			//TODO: Check and verify this functions. Also possibly better way to do this?
			expand( BoundingSphere( box ) );
		}

		inline BoundingSphere::BoundingSphere( const Vector3::Array& va )
		{ 			
			BoundingBox bb( va );

			m_center = bb.center();
			m_radius = math::max( bb.width()*(k_Real)0.5, math::max( bb.height()*(k_Real)0.5, bb.depth()*(k_Real)0.5 ) );

			expand( va );
		}	

		void BoundingSphere::expand( const Vector3::Array& va )
		{
			for ( Vector3::Array::const_iterator iter = va.begin(); iter != va.end(); ++iter )
				expand( (*iter) );
		}
	
	} // END: frx

} // END: kismo
