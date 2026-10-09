/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\framework\KismoSize.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef _KISMO_SIZE_H
#define	_KISMO_SIZE_H

#include "framework/KismoMath.h"

namespace kismo
{
	namespace frx
	{
		/** \brief Templated 2D Size type */
		template< typename T > struct Size2
		{
		public:

			typedef k_Vector< Size2 > Array;

		public:

			T width; ///< Width
			T height; ///< height

			/** \brief Constructor does not initialise the size for efficiency */
			Size2() {}
			/** \brief Constructor takes the width and height of the size */
			Size2( const T _width, const T _height ) :
				width( _width ), height( _height ) {}
		};
		
		/** \brief Templated 3D Size type */
		template< typename T > struct Size3
		{
			T width; ///< Width
			T height; ///< Height
			T depth; ///< Depth

			/** \brief Constructor does not initialise the size for efficiency */
			Size3() {}

			/** \brief Constructor takes the width, height and depth of the size */
			Size3( const T _width, const T _height, const T _depth ) :
				width( _width ), height( _height ), depth( _depth ) {}
		};
		
		//Typedef more common 2D size types
		typedef Size2< k_Float32 > Size2f; ///< A 2-dimensional size of float type
		typedef Size2< k_Float64 > Size2d; ///< A 2-dimensional size of double type
		typedef Size2< k_Real > Size2r; ///< A 2-dimensional size of real type
		typedef Size2< k_Int32 > Size2i; ///< A 2-dimensional size of integer type
		typedef Size2< k_Int16 > Size2s; ///< A 2-dimensional size of short type

		//Typedef more common 3D size types
		typedef Size3< k_Float32 > Size3f; ///< A 3-dimensional size of float type
		typedef Size3< k_Float64 > Size3d; ///< A 3-dimensional size of double type
		typedef Size3< k_Real > Size3r; ///< A 3-dimensional size of real type
		typedef Size2< k_Int32 > Size3i; ///< A 3-dimensional size of integer type
		typedef Size2< k_Int16 > Size3s; ///< A 3-dimensional size of short type

	} // END: frx

} // END: kismo

#endif
