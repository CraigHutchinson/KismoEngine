/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\framework\KismoRect.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef _KISMO_RECT_H
#define	_KISMO_RECT_H

#include "framework/KismoVector.h"
#include "framework/KismoSize.h"

namespace kismo
{
	namespace frx
	{

		/** \brief Templated 2D Rectangle defining a box with 2 dimensions and a center */
		template< typename T > struct Rect2
		{
		public:

			typedef k_Vector< Rect2< T > > Array;

		public:

			Vector2< T > center; ///< Center of the rectangle
			Size2< T > size; ///< Size extents of the rectangle

			/** \brief Constructor does not initialise the rect for efficiency */
			Rect2() {}
			/** \brief Constructor takes 4 values for the rectangle sides */
			Rect2( const T left, const T top, const T right, const T bottom ) :
				center( Vector2< T >( (right+left) * (T)0.5, (bottom+top) * (T)0.5 ) ),
				size( Size2< T >( right-left, bottom-top ) ) {}
			/** \brief Constructor takes a size and center */
			Rect2( const Vector2< T >& _center, const Size2< T >& _size ) :
				center( _center ), size( _size ) {}
		};
		
		/** \brief Templated 2D Rectangle defining a box with 2 dimensions and a center */
		template< typename T > struct Rect3
		{
		public:

			typedef k_Vector< Rect3 > Array;

		public:

			Vector3 center; ///< Center of the rectangle
			Vector3 axis[2]; ///< Unit vectors determinitng the local x and y axes for the rectangle
			Size2< T > size; ///< Size extents of the rectangle along the axes

			/** \brief Constructor does not initialise the rect for efficiency */
			Rect3() {}
			/** \brief Constructor takes 4 values for the rectangle sides */
			Rect3( const T left, const T top, const T right, const T bottom ) :
				center( Vector2< T >( (right+left) * (T)0.5, (bottom+top) * (T)0.5 ) ),
				size( Size2< T >( right-left, bottom-top ) ) {}
			/** \brief Constructor takes a size and center */
			Rect3( const Vector2< T >& _center, const Size2< T >& _size ) :
				center( _center ), size( _size ) {}
		};
		
		//Typedef more common rectangle types
		typedef Rect2< k_Float32 > Rect2f; ///< A 2dimensional rectangle of float type
		typedef Rect2< k_Float64 > Rect2d; ///< A 2dimensional rectangle of double type
		typedef Rect2< k_Real > Rect2r; ///< A 2dimensional rectangle of real type
		typedef Rect2< k_Int32 > Rect2i; ///< A 2dimensional rectangle of integer type
		typedef Rect2< k_Int16 > Rect2s; ///< A 2dimensional rectangle of short type

		//Typedef more common rectangle types
		typedef Rect3< k_Float32 > Rect3f; ///< A 3dimensional rectangle of float type
		typedef Rect3< k_Float64 > Rect3d; ///< A 3dimensional rectangle of double type
		typedef Rect3< k_Real > Rect3r; ///< A 3dimensional rectangle of real type
	
	} // END: frx

} // END: kismo

#endif
