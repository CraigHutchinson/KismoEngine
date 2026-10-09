/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\Gfx\BufferElement.cpp
 * Status: duplicate of kismo-v0.3/Gfx/BufferElement.cpp
 */
#include "BufferElement.h"

namespace gfx
{
	BufferElement::BufferElement( eElement element, eDataType type, UInt8 count, UInt8 offset ) :
		m_element( element ), 
		m_count( count ),
		m_type( type ),
		m_offset( offset ),
		m_size( sizeOf( type ) * count )
	{}

} //END: gfx