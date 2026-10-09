/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\Framework\Rect.cpp
 * Status: superseded by kismo-core/_SOURCE_/KismoFramework/Rect.cpp (same name, content differs)
 */
#include "Rect.h"

namespace frx
{	
	template < class T >
	Rect2< T >::Rect2( const T left, const T top, const T right, const T bottom ) :
		m_center( Position( (right+left) * (T)0.5, (bottom+top) * (T)0.5 ) ),
		m_size( Size( right-left, bottom-top ) ) {}


	template class Rect2< Int16 >;
	template class Rect2< Int32 >;
	template class Rect2< Float32 >;
	template class Rect2< Float64 >;

	template class Rect3< Int16 >;
	template class Rect3< Int32 >;
	template class Rect3< Float32 >;
	template class Rect3< Float64 >;

} //END: frx