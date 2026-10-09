/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoFRX\Rect.cpp
 * Status: superseded by kismo-core/_SOURCE_/KismoFramework/Rect.cpp (same name, content differs)
 */
#include "Rect.h"

namespace kismo
{	
	template < class T >
	Rect2< T >::Rect2( const T left, const T top, const T width, const T height ) :
		m_center( Position( left + T(0.5f*width), top + T(0.5f*height) ) ),
		m_size( Size( width, height ) ) {}


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