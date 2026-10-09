/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\KismoCore\KismoFramework\AaxisAlignedBox.h
 * Status: unique (no other copy of this file name in the archive)
 */
/*^**************************************************************************
*	
*	Kismo Engine 
*	© 2008-2011 Labyrinth Entertainment Ltd.
*	All right reserved
*
*	This file is subject to the terms and conditions defined in
*	file 'license.txt', which is part of this source code package.
*
***************************************************************************^*/

#ifndef KISMOFRAMEWORK_AABOX_H
#define	KISMOFRAMEWORK_AABOX_H

#include "KismoFramework/Types.h"
#include "KismoFramework/Math.h"
#include "KismoFramework/VectorN.h"

namespace kismo
{
	//Forward declarations
	template< class _Real > class Sphere;
	template< class _Real > class OBox; //Orientated box
	template < class _Real, Count _Rows, Count _Cols > class MatrixT;

	/** \brief Axis aligned Bounding box 
	*/
	template < typename _Real = Real >
	class KISMOFRAMEWORK_EXPORT AABox
	{
	public:
		typedef VectorT<_Real,3 > _Vector;
	public: 
		/** \brief Constructor defalts to a zero sized box 
		*/
		AABox();

		/** \brief Constructor from the minimum & maximum corners of a box 
		*/
		AABox( const _Vector& minimum, const _Vector& maximum );

		/** \brief Constructor from an array of vector positions 
		*/
		AABox( const typename _Vector::Array& va );	

		/** \brief Constructor to calculate bounds of a sphere 
		*/
		AABox( const Sphere<_Real>& sphere );

		/** \brief Transform the bounding box by a matrix and resizing to fit the resulting box 
		*/
		void transform( const MatrixT<_Real,4,4>& matrix );
		void transform( const MatrixT<_Real,3,3>& matrix );

		/** \brief Expand the box to include the supplied vector 
		*/
		void expand( const _Vector& v );

		/** \brief Expand the box to include the supplied vector array 
		*/
		void expand( const typename _Vector::Array& va );

		/** \brief Expand to include the supplied bounding box 
		*/
		void expand( const AABox& box );	

		/** \brief Expand to include the supplied bounding sphere 
		*/
		void expand( const Sphere<_Real>& sphere );	

		/** \brief Checks if the bounding box contains the supplied vector position 
		*/
		Bool contains( const _Vector& v ) const;

		/** \brief Checks if this box intersects with another
		\param[in] tolerance TO ADD?
		*/
		Bool intersects( const AABox<_Real>& box ) const { return false; }

		/** \brief Returns the width of the box in the X-axis 
		*/
		_Real width() const 
		{ return m_max.x - m_min.x; }

		/** \brief Returns the height of the box in the Y-axis 
		*/
		_Real height() const 
		{ return m_max.y - m_min.y; }

		/** \brief Returns the height of the box in the Z-axis 
		*/
		_Real depth() const 
		{ return m_max.z - m_min.z; }

		/** \brief Returns the center of the box 
		*/
		_Vector center() const 
		{ return _Vector( (m_min.x+m_max.x)*(_Real)0.5, (m_min.y+m_max.y)*(_Real)0.5, (m_min.z+m_max.z)*(_Real)0.5); } 

		/** \brief Returns the minimum points of the box
		\remarks 
			This is the corner with the minimum x,y,&z value 
		*/
		const _Vector& min() const 
		{ return m_min; }

		/** \brief Returns the maximum points of the box
		\remarks 
			This is the corner with the maximum x,y,&z value 
			*/
		const _Vector& max() const 
		{ return m_max; }

		/** \brief Checks if the box has been initialised and is valid 
		*/
		Bool isValid() const 
		{ return (m_min.x != m_max.x) && (m_min.y != m_max.y) && (m_min.z != m_max.z); } 

		/** \brief This function takes a vector and returns the nearest point on the box
		\param[in] c Vector positon to test against
		\param[out] p Closest point on the box 
		*/
		void closestPoint( const _Vector& point, _Vector& closestPoint ) const;

		/** \brief Calculates the squared distance of a point from the box .
		\param v The position vector to test against
		\return	
			The squared distance from the box to v 
		*/
		_Real distanceSquared( const _Vector& v ) const;

		/** \brief Calculates the  distance of a point from the box .
		\param v The position vector to test against
		\returns	The distance from the box to v 
		*/
		inline Real distance( const _Vector& v ) const 
		{ return fastSqrt( (Real)distanceSquared( v ) ); }

	private: 			
		_Vector m_min; ///< The minimum corner of the box
		_Vector m_max; ///< The maximum corner of the box
	};

	template < typename _Real >	
	inline AABox<_Real>::AABox() : 
		m_min( _Vector(0,0,0) ), m_max( _Vector(0,0,0) ) 
	{}

	template < typename _Real >	
	inline AABox<_Real>::AABox( const _Vector& minimum, const _Vector& maximum ) :
		m_min( minimum ), m_max( maximum ) 
	{}			

	template < typename _Real >	
	inline AABox<_Real>::AABox( const typename _Vector::Array& va )
	{ 
		m_min = m_max = ( va.size() ? va[0] : _Vector(0,0,0) );
		expand( va );
	}	

	template < typename _Real >	
	inline void AABox<_Real>::transform( const MatrixT<_Real,4,4>& matrix )
	{
		Assert( false );
	}

	template < typename _Real >	
	inline void AABox<_Real>::transform( const MatrixT<_Real,3,3>& matrix )
	{
		Assert( false );
	}

	template < typename _Real >	
	inline void AABox<_Real>::expand( const typename _Vector::Array& va )
	{
		for ( _Vector::Array::const_iterator iter = va.begin(); iter != va.end(); ++iter )
			expand( (*iter) );
	}

	template < typename _Real >	
	inline void AABox<_Real>::expand( const _Vector& v )
	{
		if ( v.x < m_min.x ) //Check for the lower bounds
			m_min.x = v.x;
		else //Only check the maximum bounds if min fails...
		if ( v.x > m_max.x )
			m_max.x = v.x;

		if ( v.y < m_min.y ) //Check for the lower bounds
			m_min.y = v.y;
		else //Only check the maximum bounds if min fails...
		if ( v.y > m_max.y )
			m_max.y = v.y;
		
		if ( v.z < m_min.z ) //Check for the lower bounds
			m_min.z = v.z;
		else //Only check the maximum bounds if min fails...
		if ( v.z > m_max.z )
			m_max.z = v.z;
	}

	template < typename _Real >	
	inline void AABox<_Real>::expand( const AABox<_Real>& box )
	{
		expand( box.min() );
		expand( box.max() );
	}			

	template < typename _Real >	
	inline Bool AABox<_Real>::contains( const _Vector& v ) const
	{
		return (v.x >= m_min.x) && (v.y >= m_min.y) && (v.z >= m_min.z) 
			&& (v.x <= m_max.x) && (v.y <= m_max.y) && (v.z <= m_max.z); 
	}
	

} // END: math
#endif
