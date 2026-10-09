/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\Gfx\BufferDefinition.cpp
 * Status: duplicate of kismo-v0.3/Gfx/BufferDefinition.cpp
 */
#include "BufferDefinition.h"

namespace gfx
{
	BufferDefinition::BufferDefinition() :
		m_size( 0 )
	{

	}

	BufferDefinition::BufferDefinition( const BufferElement& element ) :
		m_size( 0 )
	{
		addElement( element );
	}

	const BufferElement& BufferDefinition::addElement( const BufferElement& element )
	{
		//add the new element to the back fo the deifnition
		m_elements.push_back( element );

		//get the new element
		BufferElement& pElement = m_elements.back();
		assert( pElement == element );

		//Set the offset of the vertex element
		pElement.m_offset = m_size;
 
		// Update the current definition size
		m_size += pElement.getSize();

		return pElement;
	}


} //END: gfx