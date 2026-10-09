/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\Math\BoundingBox.h
 * Status: current (newest copy of this file name in the archive)
 */
#ifndef FRX_BOUNDINGBOX_H
#define	FRX_BOUNDINGBOX_H

#include "Framework/Types.h"
#include "Math/CoreMath.h"
#include "Math/Vector.h"

namespace math
{
	//Forward Declaration
	class Matrix44; 
	class Matrix33;
	template< class _TReal > class BoundingSphere;

	/** \brief Axis aligned Bounding box 
	*/
	template < typename _TReal = _TReal >
	class _DllExport BoundingBox
	{
	public:
		typedef VectorN<_TReal,3 > VectorT;
	public: 
		/** \brief Constructor defalts to a zero sized box 
		*/
		BoundingBox();

		/** \brief Constructor from the minimum & maximum corners of a box 
		*/
		BoundingBox( const VectorT& minimum, const VectorT& maximum );

		/** \brief Constructor from an array of vector positions 
		*/
		BoundingBox( const typename VectorT::Array& va );	

		/** \brief Constructor to contain a bounding sphere 
		*/
		BoundingBox( const BoundingSphere<_TReal>& sphere );

		/** \brief Transform the bounding box by a matrix and resizing to fit the resulting box 
		*/
		void transform( const Matrix44& matrix );
		void transform( const Matrix33& matrix );

		/** \brief Expand the box to include the supplied vector 
		*/
		void expand( const VectorT& v );

		/** \brief Expand the box to include the supplied vector array 
		*/
		void expand( const typename VectorT::Array& va );

		/** \brief Expand to include the supplied bounding box 
		*/
		void expand( const BoundingBox& box );	

		/** \brief Expand to include the supplied bounding sphere 
		*/
		void expand( const BoundingSphere<_TReal>& sphere );	

		/** \brief Checks if the bounding box contains the supplied vector position 
		*/
		Bool contains( const VectorT& v ) const;

		/** \brief Checks if this box intersects with another
		\param[in] tolerance TO ADD?
		*/
		Bool intersects( const BoundingBox<_TReal>& box ) const { return false; }

		/** \brief Returns the width of the box in the X-axis 
		*/
		_TReal width() const 
		{ return m_max.x - m_min.x; }

		/** \brief Returns the height of the box in the Y-axis 
		*/
		_TReal height() const 
		{ return m_max.y - m_min.y; }

		/** \brief Returns the height of the box in the Z-axis 
		*/
		_TReal depth() const 
		{ return m_max.z - m_min.z; }

		/** \brief Returns the center of the box 
		*/
		VectorT center() const 
		{ return VectorT( (m_min.x+m_max.x)*(_TReal)0.5, (m_min.y+m_max.y)*(_TReal)0.5, (m_min.z+m_max.z)*(_TReal)0.5); } 

		/** \brief Returns the minimum points of the box
		\remarks 
			This is the corner with the minimum x,y,&z value 
		*/
		const VectorT& min() const 
		{ return m_min; }

		/** \brief Returns the maximum points of the box
		\remarks 
			This is the corner with the maximum x,y,&z value 
			*/
		const VectorT& max() const 
		{ return m_max; }

		/** \brief Checks if the box has been initialised and is valid 
		*/
		Bool isValid() const 
		{ return (m_min.x != m_max.x) && (m_min.y != m_max.y) && (m_min.z != m_max.z); } 

		/** \brief This function takes a vector and returns the nearest point on the box
		\param[in] c Vector positon to test against
		\param[out] p Closest point on the box 
		*/
		void closestPoint( VectorT& p, const VectorT& c ) const;

		/** \brief Calculates the squared distance of a point from the box .
		\param v The position vector to test against
		\return	
			The squared distance from the box to v 
		*/
		inline _TReal distanceSquared( const VectorT& v ) const;

		/** \brief Calculates the  distance of a point from the box .
		\param v The position vector to test against
		\returns	The distance from the box to v 
		*/
		inline _TReal distance( const VectorT& v ) const 
		{ return math::fastSqrt( distanceSquared( v ) ); }

	private: 			
		VectorT m_min; ///< The minimum corner of the box
		VectorT m_max; ///< The maximum corner of the box
	};

	template < typename _TReal >	
	inline BoundingBox<_TReal>::BoundingBox() : 
		m_min( VectorT(0,0,0) ), m_max( VectorT(0,0,0) ) 
	{}

	template < typename _TReal >	
	inline BoundingBox<_TReal>::BoundingBox( const VectorT& minimum, const VectorT& maximum ) :
		m_min( minimum ), m_max( maximum ) 
	{}			

	template < typename _TReal >	
	inline BoundingBox<_TReal>::BoundingBox( const typename VectorT::Array& va )
	{ 
		m_min = m_max = ( va.size() ? va[0] : VectorT(0,0,0) );
		expand( va );
	}	

	template < typename _TReal >	
	inline void BoundingBox<_TReal>::transform( const Matrix44& matrix )
	{
		assert( false );
	}

	template < typename _TReal >	
	inline void BoundingBox<_TReal>::transform( const Matrix33& matrix )
	{
		assert( false );
	}

	template < typename _TReal >	
	inline void BoundingBox<_TReal>::expand( const typename VectorT::Array& va )
	{
		for ( VectorT::Array::const_iterator iter = va.begin(); iter != va.end(); ++iter )
			expand( (*iter) );
	}

	template < typename _TReal >	
	inline void BoundingBox<_TReal>::expand( const VectorT& v )
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

	template < typename _TReal >	
	inline void BoundingBox<_TReal>::expand( const BoundingBox<_TReal>& box )
	{
		expand( box.min() );
		expand( box.max() );
	}			

	template < typename _TReal >	
	inline Bool BoundingBox<_TReal>::contains( const VectorT& v ) const
	{
		return (v.x >= m_min.x) && (v.y >= m_min.y) && (v.z >= m_min.z) 
			&& (v.x <= m_max.x) && (v.y <= m_max.y) && (v.z <= m_max.z); 
	}
	

} // END: math
#endif
