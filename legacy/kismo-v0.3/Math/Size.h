/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\Math\Size.h
 * Status: current (newest copy of this file name in the archive)
 */
#ifndef FRX_SIZE_H
#define FRX_SIZE_H

#include "Framework/Types.h"

namespace math
{
	/** \brief Templated 2D Size type
	*/
	template< typename _T >
	class _DllExport Size2
	{
	public:
		typedef _T Real_T;

		_T width;
		_T height;

		/** \brief Constructor - does noting for efficiency
		*/
		Size2(){}

		/** \brief Constructor - initialises the height and width the the specified sizes
		*/
		Size2( _T nwidth, _T nheight ) :
			width( nwidth ),
			height( nheight )
		{}

		inline Size2 operator +( _T f ) const 
		{ return Size2(width + f, height + f ); } 

		inline Size2 operator -( _T f ) const 
		{ return Size2(width - f, height - f ); } 

		inline Size2 operator *( _T f ) const 
		{ return Size2(width * f, height * f ); }

		inline Size2 operator /( _T f ) const 
		{ return Size2(width / f, height / f ); }

		inline Size2& operator +=( _T f ) 
		{ width += f; height += f; return (*this); }

		inline Size2& operator -=( _T f ) 
		{ width -= f; height -= f; return (*this); }

		inline Size2& operator *=( _T f ) 
		{ width *= f; height *= f; return (*this); }

		inline Size2& operator /=( _T f ) 
		{ width /= f; height /= f; return (*this); }

		inline Bool operator !=( const Size2& rhs ) const
		{ return width != rhs.width || height != rhs.height ; }

		inline Bool operator ==( const Size2& rhs ) const 
		{ return width == rhs.width && height == rhs.height ; }

		/** \brief Returns whether the size is a zero size
		*/
		Bool isZero() const
		{ return width == 0 && height == 0; }
			
		template< class T2 >
		Size2& operator = ( const Size2<T2>& rhs )
		{ 
			width = (Real_T)rhs.width; 
			height = (Real_T)rhs.height;
			return *this;
		}
	};
	
	/** \brief Templated 3D Size type
	*/
	template< typename T >
	class _DllExport Size3
	{
	public:
		typedef Vector< Size3 > Array;
		
		T width; ///< Width
		T height; ///< Height
		T depth; ///< Depth

		/** \brief Constructor does not initialise the size for efficiency 
		*/
		Size3() {}

		/** \brief Constructor takes the width, height and depth of the size 
		*/
		Size3( const T nwidth, const T nheight, const T ndepth ) :
			width( nwidth ), height( nheight ), depth( ndepth ) {}

		inline Bool operator !=( const Size3& rhs ) const
		{ return width != rhs.width || height != rhs.height || depth != rhs.depth; }

		inline Bool operator ==( const Size3& rhs ) const 
		{ return width == rhs.width && height == rhs.height && depth == rhs.depth; }
			
		template< class T2 >
		Size3& operator = ( const Size3<T2>& rhs )
		{ 
			width = (Real_T)rhs.width; 
			height = (Real_T)rhs.height;
			depth = (Real_T)rhs.depth;
			return *this;
		}
	};

	typedef Size2< UInt16 > Size2s;
	typedef Size2< UInt32 > Size2i;
	typedef Size2< Real > Size2r;
	typedef Size2< Float32 > Size2f;
	typedef Size2< Float64 > Size2d;
	
	typedef Size3< UInt16 > Size3s;
	typedef Size3< UInt32 > Size3i;
	typedef Size3< Real > Size3r;
	typedef Size3< Float32 > Size3f;
	typedef Size3< Float64 > Size3d;
	
} // END: math

#endif