/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\Gfx\BufferElement.h
 * Status: duplicate of kismo-v0.3/Gfx/BufferElement.h
 */
#ifndef GFX_BUFFERELEMENT_H
#define GFX_BUFFERELEMENT_H

#include "Framework/Types.h"
#include "Framework/TypeInfo.h"
#include "Platform/Platform.h"

namespace gfx
{
	/** \brief Defines an element of a BufferDefinition
		\remarks This is a utility class for Buffer which allows for a flexible buffer definition system
	*/
	class _DllExport BufferElement
	{
		friend class BufferDefinition;
	public:
		
		typedef Vector< BufferElement > Array; ///< An array of elements
			
		/** \brief Enumerator used to identify different vertex elements
		*/
		enum eElement
		{
			Element_Position = 1, ///< Position vector
			Element_Normal, ///< Normal vector
			Element_DiffuseColour, ///< Diffuse colour
			Element_SpecularColour, ///< Specular colour
			Element_TextureCoordinate, ///< UV texture coordinate
			
			Element_Ext = 16, ///< Start of Extended/custom vertex element definitions
		};

		

	public:
		
		/** \brief Constructor
		\param[in] element The type of data element stores
		\param[in] type The type of data ie. UInt8, Float32 etc
		\param[in] count The number of elements stored ie. Vector has 3
		\param[in] offset (optional) The byte offset for the element within the vertex definition
		\remarks
			When added to a vertex definition, the 'offset' will be updated automatically
		*/
		BufferElement( eElement element, eDataType type, UInt8 count = 1, UInt8 offset = 0 );

		/** \brief Get the type of data element stores
		\remarks
			need another name for this as BufferDefinition has getElement( n ).getElement()!!
		*/
		eElement getElement() const
		{ return m_element; }
		
		/** \brief Get the type of data ie. UInt8, Float32 etc
		*/
		eDataType getType() const
		{ return m_type; }
		
		/** \brief Get the number of elements stored ie. Vector has 3
		*/
		UInt8 getCount() const
		{ return m_count; }
		
		/** \brief Get the byte offset for the elemnt within the vertex structure being defined
		*/
		UInt8 getOffset() const
		{ return m_offset; }

		/** \brief Gets the size of the element in Bytes
		*/
		UInt8 getSize() const
		{ return m_size; }

		/** \brief checks whether two elements are equal
		\remarks
			Does not compare the element offsets or size
		*/
		bool operator == ( const BufferElement& rhs ) const
		{ return (	m_element == rhs.m_element && m_type == rhs.m_type && m_count == rhs.m_count
					/* && m_offset == rhs.m_offset && m_size == rhs.m_size*/ ); }
	private: 
		eElement m_element; ///< The type of data element stores
		eDataType m_type; ///< The type of data ie. UInt8, Float32 etc
		UInt8 m_count; ///< The number of elements stored ie. Vector has 3
		UInt8 m_offset; ///< The byte offset for the elemnt within the vertex structure being defined
		UInt8 m_size; ///< The size of the elements data
	};

} //END: gfx

#endif