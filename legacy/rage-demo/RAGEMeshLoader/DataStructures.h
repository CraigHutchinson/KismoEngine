/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\RAGEMeshLoader\DataStructures.h
 * Status: superseded by kismo-v0.3/KismoMeshLoader/DataStructures.h (same name, content differs)
 */
#ifndef RAGE_DATASTRUCTURES_H
#define RAGE_DATASTRUCTURES_H

#include "Types.h"
#include "Gfx/BufferDefinition.h"

namespace rage
{

	//------------------------------------------------------------------------------
	// Buffer data types for vertex position, normal and UV coordinates
	//------------------------------------------------------------------------------
	/** \brief Holds the vertex position data
	*/
	template< class _TEdgeIndex = EdgeIndex >
	struct PositionStruct 
	{

		Position vec;
		_TEdgeIndex iedge_next;

		static gfx::BufferDefinition getDefinition()
		{
			gfx::BufferDefinition def;
			def.addElement( gfx::BufferElement( gfx::BufferElement::Element_Position, typeOf<Real>(), 3 ) );	
				assert( sizeof( _TEdgeIndex ) == 4 );
			def.addElement( gfx::BufferElement( gfx::BufferElement::Element_Ext, typeOf<_TEdgeIndex>(), 1 ) );
				assert( def.getSize() == sizeof( PositionStruct ) );
			return def;
		}
	};

	/** \brief Holds the vertex normal data
	*/
	struct NormalStruct 
	{
		Normal vec;
		
		static gfx::BufferDefinition getDefinition()
		{
			gfx::BufferDefinition def;
			def.addElement( gfx::BufferElement( gfx::BufferElement::Element_Normal, typeOf<Real>(), 3 ) );
				assert( def.getSize() == sizeof( NormalStruct ) );
			return def;
		}
	};

	/** \brief Holds the vertex texture UV data
	*/
	struct UVStruct 
	{
		UV uv;

		static gfx::BufferDefinition getDefinition()
		{
			gfx::BufferDefinition def;
			def.addElement( gfx::BufferElement( gfx::BufferElement::Element_TextureCoordinate, typeOf<Real>(), 2 ) );
				assert( def.getSize() == sizeof( UVStruct ) );
			return def;
		}
	};

	//------------------------------------------------------------------------------
	// Buffer index types for different mesh data
	//------------------------------------------------------------------------------

	/** \brief UV, Normal and position index type
	*/
	template< class _TPositionIndex = PositionIndex, class _TNormalIndex = NormalIndex, class _TUVIndex = UVIndex >
	struct Vertex_UNV
	{
		_TUVIndex iuv;
		_TNormalIndex inormal;
		_TPositionIndex ipos;

		static gfx::BufferDefinition getDefinition()
		{
			gfx::BufferDefinition def;
			def.addElement( gfx::BufferElement( gfx::BufferElement::Element_TextureCoordinate, typeOf<_TUVIndex>(), 1 ) );
			def.addElement( gfx::BufferElement( gfx::BufferElement::Element_Normal, typeOf<_TNormalIndex>(), 1 ) );
			def.addElement( gfx::BufferElement( gfx::BufferElement::Element_Position, typeOf<_TPositionIndex>(), 1 ) );
				assert( def.getSize() == sizeof( Vertex_UNV ) );
			return def;
		}
	};

	///** \brief Normal and position index type
	//*/
	//template< class _TPositionIndex = PositionIndex, class _TNormalIndex = NormalIndex >
	//struct Vertex_NV
	//{
	//	_TPositionIndex ipos;
	//	_TNormalIndex inormal;
	//};

	///** \brief UV and Position index type
	//*/
	//template< class _TPositionIndex = PositionIndex, class _TUVIndex = UVIndex >
	//struct Vertex_UV
	//{
	//	_TPositionIndex ipos;
	//	_TUVIndex iuv;
	//};

	///** \brief Position index type
	//*/
	//template< class _TPositionIndex = PositionIndex >
	//struct Vertex_V
	//{
	//	_TPositionIndex ipos;
	//};


} //END: rage

#endif