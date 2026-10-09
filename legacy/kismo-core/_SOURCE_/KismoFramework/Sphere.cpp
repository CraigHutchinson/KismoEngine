/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\KismoCore\_SOURCE_\KismoFramework\Sphere.cpp
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



#include "KismoFramework/Sphere.h"
#include "KismoFramework/AABox.h"

namespace kismo
{
	template < typename _Real >
	Sphere<_Real>::Sphere( const AABox<_Real>& box )
	{
		m_center = box.center();
		m_radiusSquared = ( m_center - box.max() ).magnitudeSquared();
		m_radius = (_Real)fastSqrt( (Real)m_radiusSquared );
	}

	template < typename _Real >
	inline Sphere<_Real>::Sphere( const typename _Vector::Array& va )
	{ 			
		AABox<_Real> bb( va );

		m_center = bb.center();
		m_radius = max( bb.width()*(_Real)0.5, max( bb.height()*(_Real)0.5, bb.depth()*(_Real)0.5 ) );

		expand( va );
	}

	template < typename _Real >
	void Sphere<_Real>::expand( const AABox<_Real>& box )
	{
		//TODO: Check and verify this functions. Also possibly better way to do this?
		expand( Sphere( box ) );
	}


	template class Sphere< Float32 >;
	template class Sphere< Float64 >;

} // END: math
