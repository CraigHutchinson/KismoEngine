/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\KismoMeshLoader\DirectedEdge.h
 * Status: current (newest copy of this file name in the archive)
 */
#ifndef Kismo_DIRECTEDEDGE_H
#define Kismo_DIRECTEDEDGE_H

#include "DataStructures.h"

namespace rage
{
	//Macro to get the index of an edge
#define BUFFINDEX (_TEdgeIndex)(this - buffer)

	//------------------------------------------------------------------------------
	// Directed edge structure which stores adjacency information
	//------------------------------------------------------------------------------

	/** \brief Holds minimum adjacency information conserving the most memory space
	*/
	//template< class _TVertex = Vertex_UNV<>, class _TEdgeIndex = EdgeIndex, class _TPositionStruct = PositionStruct< _TEdgeIndex > >
	//struct DirectedEdge_Small
	//{
	//	_TVertex vertex_end; //End vertex
	//	_TEdgeIndex iedge_adj; //Adjacent edge

	//	/** \brief get the previous edge */
	//	inline DirectedEdge_Calc& prev( DirectedEdge_Calc* buffer ) 
	//	{ 
	//		_TEdgeIndex index = BUFFINDEX; 
	//		return buffer[ ( index%3 == 0 ? index+2 :index-1 ) ];
	//	}

	//	/** \brief get the next edge */
	//	inline DirectedEdge_Calc& next( DirectedEdge_Calc* buffer ) 
	//	{ 
	//		_TEdgeIndex index = BUFFINDEX; 
	//		return buffer[ ( index%3 == 2 ? index-2 :index+1 ) ]; 
	//	}

	//	inline _TPositionStruct& v1( _TPositionStruct* buffer )
	//	{ }
	//	inline _TPositionStruct& v2( _TPositionStruct* buffer )
	//	{ return buffer[ vertex_end ] }
	//};

	///** \brief Holds medium adjacency information conserving memory space
	//*/
	//template< class _TVertex = Vertex_UNV<>, class _TEdgeIndex = EdgeIndex >
	//struct DirectedEdge_Medium
	//{
	//	Vertex_UNV vertex_end; //End vertex
	//	_TEdgeIndex iedge_adj; //Adjacent edge
	//	_TEdgeIndex iedge_prev; //Previous edge (counter-clockwise)

	//};
	
	/** \brief Holds full adjacency information for dynamic mesh updates with calculated
		next and previous edges
	*/
	template< class _TVertex = Vertex_UNV<>, class _TEdgeIndex = EdgeIndex, class _TPositionStruct = PositionStruct< _TEdgeIndex > >
	struct DirectedEdge_Calc
	{
		_TVertex vertex_start; //Start vertex
		_TVertex vertex_end; //End vertex
		_TEdgeIndex iedge_adj; //Adjacent edge

		/** \brief get the previous edge */
		inline DirectedEdge_Calc& prev( DirectedEdge_Calc* buffer ) 
		{ 
			_TEdgeIndex index = BUFFINDEX; 
			return buffer[ ( index%3 == 0 ? index+2 :index-1 ) ];
		}

		/** \brief get the next edge */
		inline DirectedEdge_Calc& next( DirectedEdge_Calc* buffer ) 
		{ 
			_TEdgeIndex index = BUFFINDEX; 
			return buffer[ ( index%3 == 2 ? index-2 :index+1 ) ]; 
		}

		/** \brief get the adjacent edge */
		inline DirectedEdge_Calc& adj( DirectedEdge_Calc* buffer ) 
		{ return buffer[iedge_adj]; }
		
		/** \brief get the start vertex */
		inline _TVertex& v1( _TPositionStruct* buffer )
		{ return buffer[ vertex_start ]; }

		/** \brief get the end vertex */
		inline _TVertex& v2( _TPositionStruct* buffer )
		{ return buffer[ vertex_end ]; }
		
	};

	/** \brief Holds full adjacency information for dynamic mesh updates
	*/
	template< class _TVertex = Vertex_UNV<> , class _TEdgeIndex = EdgeIndex, class _TPositionStruct = PositionStruct< _TEdgeIndex > >
	struct DirectedEdge_Full
	{
		_TVertex vertex_start; //Start vertex
		_TVertex vertex_end; //End vertex
		_TEdgeIndex iedge_adj; //Adjacent edge
		_TEdgeIndex iedge_next; //Next edge (counter-clockwise)
		_TEdgeIndex iedge_prev; //Previous edge (counter-clockwise)

		/** \brief get the previous edge */
		inline DirectedEdge_Full& prev( DirectedEdge_Full* buffer ) 
		{ return buffer[iedge_prev]; }

		/** \brief get the next edge */
		inline DirectedEdge_Full& next( DirectedEdge_Full* buffer ) 
		{ return buffer[iedge_next]; }

		/** \brief get the adjacent edge */
		inline DirectedEdge_Full& adj( DirectedEdge_Full* buffer ) 
		{ return buffer[iedge_adj]; }

		/** \brief get the start vertex */
		inline _TVertex& v1( _TPositionStruct* buffer )
		{ return buffer[ vertex_start ]; }

		/** \brief get the end vertex */
		inline _TVertex& v2( _TPositionStruct* buffer )
		{ return buffer[ vertex_end ]; }
	};


	//------------------------------------------------------------------------------
	// Vertex Implimentation
	//------------------------------------------------------------------------------
	//template<	class _TPositionIndex	= PositionIndex, 
	//			class _TNormalIndex		= NormalIndex, 
	//			class _TUVIndex			= UVIndex, 
	//			class _TEdgeIndex		= EdgeIndex, 	
	//			class _TVertex			= Vertex_UNV< _TPositionIndex, _TNormalIndex, _TUVIndex > 
	//		>
	//struct Vertex
	//{		
	//	Byte* buffer; ///< Pointer to the vertex buffer(s)

	//};

	//------------------------------------------------------------------------------
	// Directed edge adjacency implimentation
	//------------------------------------------------------------------------------
	//template<	class _TPositionIndex	= PositionIndex, 
	//			class _TNormalIndex		= NormalIndex, 
	//			class _TUVIndex			= UVIndex, 
	//			class _TEdgeIndex		= EdgeIndex, 	
	//			class _TVertex			= Vertex_UNV< _TPositionIndex, _TNormalIndex, _TUVIndex >, 
	//			class _TDirectedEdge	= DirectedEdge_Full< _TVertex, _TEdgeIndex >  >
	//struct DirectedEdge
	//{
	//	typedef _TDirectedEdge Edge_T;
	//	typedef _TVertex Vertex_T;
	//	typedef _TEdgeIndex EdgeIndex_T;
	//	typedef _TNormalIndex NormalIndex_T;
	//	typedef _TPositionIndex PositionIndex_T;
	//	typedef _TUVIndex UVIndex_T;

	//	Edge_T* buffer; ///< Pointer to the edge buffer
	//	EdgeIndex_T index; ///< The index of this edge within the buffer

	//	DirectedEdge( Edge_T* edgeBuffer, EdgeIndex_T edgeIndex ) :
	//		buffer( edgeBuffer ),
	//		index( edgeIndex )
	//	{}

	//	/** \brief Gets the size of the edge structure in memory
	//	*/
	//	inline Size size() const
	//	{ return sizeof( _TDirectedEdge ); }

	//	/** \brief Gets the next edge
	//	*/
	//	DirectedEdge next() const;

	//	/** \brief Gets the previous edge
	//	*/
	//	DirectedEdge prev() const;

	//	/** \brief Gets the adjacent edge
	//	*/
	//	DirectedEdge adj() const;
	//	
	//	/** \brief Gets the start vertex
	//	*/
	//	//Vertex v1();

	//	///** \brief Gets the end vertex
	//	//*/
	//	//Vertex v2();

	//	void t();
	//};
	////---------------------------------------------------------------------------------
	//// DirectedEdge_# Generic #
	////---------------------------------------------------------------------------------
	//template<>
	//inline DirectedEdge<> DirectedEdge<>::prev() const
	//{ return DirectedEdge( buffer, buffer[index].prev( buffer ) ); }
	//template<>
	//inline DirectedEdge<> DirectedEdge<>::next() const
	//{ return DirectedEdge( buffer, buffer[index].next( buffer ) ); }

	////---------------------------------------------------------------------------------
	//// DirectedEdge_Calc specialisation
	////---------------------------------------------------------------------------------
	////template< class _TPI, class _TNI, class _TUVI, class _TEI, class _TV >
	////DirectedEdge< _TPI, _TNI, _TUVI, _TEI, _TV, DirectedEdge_Calc< _TV, _TEI > > 
	////	DirectedEdge< _TPI, _TNI, _TUVI, _TEI, _TV, DirectedEdge_Calc< _TV, _TEI > >::prev() const
	////{ return DirectedEdge( buffer, ( buffer[index].iedge_prev%3 == 0 ? buffer[index].iedge_prev+2 : buffer[index].iedge_prev-1 ) ); }

	//template< class _TPositionIndex, class _TNormalIndex, class _TUVIndex, class _TEdgeIndex, class _TVertex, DirectedEdge_Calc< _TVertex, _TEdgeIndex > >
	//void DirectedEdge< _TPositionIndex, _TNormalIndex, _TUVIndex, _TEdgeIndex, _TVertex >::t() {}

	////template<>
	////DirectedEdge< DirectedEdge_Calc > DirectedEdge< DirectedEdge_Calc >::next()
	////{ return DirectedEdge( buffer, ( buffer[index].iedge_next%3 == 2 ? buffer[index].iedge_next-2 : buffer[index].iedge_next+1 ) ); }


} //END: rage

#endif