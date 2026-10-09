/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\KismoCore\_SOURCE_\KismoFramework\Rect.cpp
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



#include "Rect.h"

namespace kismo
{	
	template < class T >
	Rect2< T >::Rect2( const T left, const T top, const T width, const T height ) :
		m_topLeft( left, top ),
		m_size( width, height ) {}


	template class Rect2< Int16 >;
	template class Rect2< Int32 >;
	template class Rect2< UInt16 >;
	template class Rect2< UInt32 >;
	template class Rect2< Float32 >;
	template class Rect2< Float64 >;

	template class Rect3< Int16 >;
	template class Rect3< Int32 >;
	template class Rect3< UInt16 >;
	template class Rect3< UInt32 >;
	template class Rect3< Float32 >;
	template class Rect3< Float64 >;

} //END: kismo