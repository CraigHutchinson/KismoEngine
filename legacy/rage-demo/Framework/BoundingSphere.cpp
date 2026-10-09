/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\Framework\BoundingSphere.cpp
 * Status: superseded by kismo-v0.3/Math/BoundingSphere.cpp (same name, content differs)
 */
#include "BoundingSphere.h"
#include "BoundingBox.h"

namespace frx
{
	template < typename _TReal >
	BoundingSphere<_TReal>::BoundingSphere( const BoundingBox<_TReal>& box )
	{
		m_center = box.center();
		m_radiusSquared = ( m_center - box.max() ).lengthSquared();
		m_radius = math::fastSqrt( m_radiusSquared );
	}

	template < typename _TReal >
	inline BoundingSphere<_TReal>::BoundingSphere( const typename Vector3< _TReal >::Array& va )
	{ 			
		BoundingBox<_TReal> bb( va );

		m_center = bb.center();
		m_radius = math::max( bb.width()*(_TReal)0.5, math::max( bb.height()*(_TReal)0.5, bb.depth()*(_TReal)0.5 ) );

		expand( va );
	}

	template < typename _TReal >
	void BoundingSphere<_TReal>::expand( const BoundingBox<_TReal>& box )
	{
		//TODO: Check and verify this functions. Also possibly better way to do this?
		expand( BoundingSphere( box ) );
	}


	template class BoundingSphere< Float32 >;
	template class BoundingSphere< Float64 >;

} // END: frx
