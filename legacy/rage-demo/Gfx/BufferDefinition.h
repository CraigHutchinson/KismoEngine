/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\Gfx\BufferDefinition.h
 * Status: duplicate of kismo-v0.3/Gfx/BufferDefinition.h
 */
#ifndef GFX_BUFFERDEFINITION_H
#define GFX_BUFFERDEFINITION_H

#include "Platform/Platform.h"
#include "Gfx/BufferElement.h"

namespace gfx
{
	/** \brief Defines a vertex definition. Ie. Position, normal and colour. This is synonymous
		to the Dx FVF format idea
	*/
	class _DllExport BufferDefinition
	{
	public:

		typedef BufferElement::Array::const_iterator ElementIterator;

	public:

		BufferDefinition();

		/** \brief initialises the deifnition with a single element definition for convenience
		*/
		BufferDefinition( const BufferElement& element );

		/** \brief Adds a new element type to the end of the vertex definition
		\return 
			A pointer to the internal vertex element which can be used to get the vertex offset
			as required.
		*/
		const BufferElement& addElement( const BufferElement& element );

		/** \brief Gets the total size of the definition in Bytes
		*/
		UInt8 getSize() const
		{ return m_size; }

		/** \brief Get a pointer to an element
		*/
		const BufferElement& getElement( Index index ) const
		{ return m_elements[ index ]; }

		/** \brief Element iterator start
		*/
		ElementIterator elementBegin() const
		{ return m_elements.begin(); }
		
		/** \brief Element iterator end position
		*/
		ElementIterator elementEnd() const
		{ return m_elements.end(); }

	private:

		BufferElement::Array m_elements; ///< List of the elements

		UInt8 m_size; ///< The total size of all the vertex elements
	};

} //END: gfx

#endif