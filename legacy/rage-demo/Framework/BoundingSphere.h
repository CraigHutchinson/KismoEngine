/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\Framework\BoundingSphere.h
 * Status: superseded by kismo-v0.3/Math/BoundingSphere.h (same name, content differs)
 */
#ifndef FRX_BOUNDINGSPHERE_H
#define	FRX_BOUNDINGSPHERE_H

#include "Framework/Common.h"
#include "Framework/Vector.h"

namespace frx
{
	//Forward Declaration
	class Matrix4x4; 
	class Matrix3x3;
	template< class _TReal  > class BoundingBox;

	/** \brief 3D Axis aligned Bounding box 
	*/
	template < typename _TReal = Real >
	class _DllExport BoundingSphere
	{
	public: 
		/** \brief Constructor defalts to a zero sized box 
		*/
		BoundingSphere();

		/** \brief Constructor from a center position and a radius 
		*/
		BoundingSphere( const Vector3< _TReal >& center, _TReal radius );	

		/** \brief Constructor from an array of vector positions 
		\remarks
			This function performs a first pass to make array construction much 
			faster 
		*/
		BoundingSphere( const typename Vector3< _TReal >::Array& va );	

		/** \brief Constructor to contain a bounding box 
		*/
		BoundingSphere( const BoundingBox<_TReal>& box );

		/** \brief Transform the bounding box by a matrix and resizing to fit the resulting box 
		*/
		void transform( const Matrix4x4& matrix );
		void transform( const Matrix3x3& matrix );

		/** \brief Expand the box to include the supplied vector */
		void expand( const Vector3< _TReal >& v );

		/** \brief Expand the box to include the supplied vector array
		\remarks 
			Use the array constructor for optimised creation of a sphere from points 
		*/
		void expand( const typename Vector3< _TReal >::Array& va );

		/** \brief Expand to include the supplied bounding box 
		*/
		void expand( const BoundingBox<_TReal>& box );	

		/** \brief Expand to include the supplied bounding sphere 
		*/
		void expand( const BoundingSphere<_TReal>& sphere );	

		/** \brief Checks if the bounding box contains the supplied vector position 
		*/
		Bool contains( const Vector3< _TReal >& v ) const;

		/** \brief Checks if this Sphere intersects with a box 
		*/
		Bool intersects( const BoundingBox<_TReal>& box ) const { return false; }

		/** \brief Returns the radius of the sphere 
		*/
		_TReal radius() const 
		{ return m_radius; }

		/** \brief Returns the squared radius of the sphere 
		*/
		_TReal radiusSquared() const 
		{ return m_radiusSquared; }

		/** \brief Returns the center of the sphere 
		*/
		Vector3< _TReal > center() const 
		{ return m_center; } 

		/** \brief Checks if the sphere has been initialised and is valid 
		*/
		Bool isValid() const 
		{ return m_radius != 0 && m_center != 0; } 

		/** \brief This function takes a vector and returns the nearest point on the spheres surface
		\param[in] vec Vector position to test against
		\return	
			The nearest point on the surface of the sphere 
		*/
		Vector3< _TReal > closestPoint( const Vector3< _TReal >& vec ) const;

	private:
		Vector3< _TReal > m_center; ///< The center of the sphere
		_TReal m_radius; ///< The radius of the sphere
		_TReal m_radiusSquared; ///< The squared radius of the sphere
	};

	
	template < typename _TReal >
	inline BoundingSphere<_TReal>::BoundingSphere() : 
		m_center( (_TReal)0.0 ), m_radius( (_TReal)0 ) 
	{}

	template < typename _TReal >
	inline BoundingSphere<_TReal>::BoundingSphere( const Vector3< _TReal >& center, _TReal radius ) :
		m_center( center ), m_radius( radius ) 
	{}	



	template < typename _TReal >
	inline void BoundingSphere<_TReal>::transform( const Matrix4x4& matrix )
	{
		assert( false );
	}

	template < typename _TReal >
	inline void BoundingSphere<_TReal>::transform( const Matrix3x3& matrix )
	{
		assert( false );
	}

	template < typename _TReal >
	void BoundingSphere<_TReal>::expand( const Vector3< _TReal >& v )
	{
		if ( !isValid() ) //If the sphere is not valid then this is the first point added
		{
			m_center = v;
			return;
		}

		_TReal distSquared = ( v - m_center ).lengthSquared();
		if ( distSquared > m_radiusSquared )
		{
			_TReal dist = math::fastSqrt( distSquared );
			m_radius = (m_radius + dist) * (_TReal)0.5;
			m_radiusSquared = math::sqr( m_radius );
			_TReal offsetDist = dist - m_radius;
			
			//Calculate the new sphere center
			m_center.x = ( (m_radius * m_center.x) + (offsetDist * v.x) ) / dist;
			m_center.y = ( (m_radius * m_center.y) + (offsetDist * v.y) ) / dist;
			m_center.z = ( (m_radius * m_center.z) + (offsetDist * v.z) ) / dist;
		}
	}

	template < typename _TReal >
	inline void BoundingSphere<_TReal>::expand( const typename Vector3< _TReal >::Array& va )
	{
		for ( Vector3< _TReal >::Array::const_iterator iter = va.begin(); iter != va.end(); ++iter )
			expand( (*iter) );
	}

	template < typename _TReal >
	void BoundingSphere<_TReal>::expand( const BoundingSphere<_TReal>& sphere )
	{
		//TODO: Check and verify this function is correct
		Vector3< _TReal > offset = ( sphere.center() - m_center );
		_TReal dist = offset.length() +  radius();
		offset.normalise() *= dist;
		expand( offset );
	}			

	template < typename _TReal >
	inline Bool BoundingSphere<_TReal>::contains( const Vector3< _TReal >& v ) const
	{
		if ( isValid() )
			return ( v - m_center ).lengthSquared() <= m_radius;
		else
			return false;
	}

	template < typename _TReal >
	inline Vector3< _TReal > BoundingSphere<_TReal>::closestPoint( const Vector3< _TReal >& v ) const
	{
		//Unit vector from center to the point scaled to the radiyus...
		return m_center + (v - m_center).normalise() * m_radius;
	}

} // END: frx

#endif