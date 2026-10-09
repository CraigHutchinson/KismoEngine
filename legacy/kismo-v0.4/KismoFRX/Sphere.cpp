/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoFRX\Sphere.cpp
 * Status: superseded by kismo-core/_SOURCE_/KismoFramework/Sphere.cpp (same name, content differs)
 */
#include "KismoFRX/Sphere.h"
#include "KismoFRX/AABox.h"

namespace kismo
{
	template < typename _TReal >
	Sphere<_TReal>::Sphere( const AABox<_TReal>& box )
	{
		m_center = box.center();
		m_radiusSquared = ( m_center - box.max() ).magnitudeSquared();
		m_radius = fastSqrt( m_radiusSquared );
	}

	template < typename _TReal >
	inline Sphere<_TReal>::Sphere( const typename VectorT::Array& va )
	{ 			
		AABox<_TReal> bb( va );

		m_center = bb.center();
		m_radius = max( bb.width()*(_TReal)0.5, max( bb.height()*(_TReal)0.5, bb.depth()*(_TReal)0.5 ) );

		expand( va );
	}

	template < typename _TReal >
	void Sphere<_TReal>::expand( const AABox<_TReal>& box )
	{
		//TODO: Check and verify this functions. Also possibly better way to do this?
		expand( Sphere( box ) );
	}


	template class Sphere< Float32 >;
	template class Sphere< Float64 >;

} // END: math
