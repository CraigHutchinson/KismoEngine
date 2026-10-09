/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\KismoCore\KismoFramework\Sphere.h
 * Status: duplicate of kismo-core/_SOURCE_/KismoFramework/Sphere.h
 */
/*^**************************************************************************
*	
*	Kismo Engine 
*	© 2008-2013 Craig Hutchinson (craig@craighutchinson.co.uk)
*	All right reserved
*
*	This file is subject to the terms and conditions defined in
*	file 'license.txt', which is part of this source code package.
*
***************************************************************************^*/

#ifndef KISMOFRAMEWORK_SPHERE_H
#define	KISMOFRAMEWORK_SPHERE_H

#include "KismoFramework\Common.h"
#include "KismoFramework/VectorN.h"

namespace kismo
{
	//Forward Declaration
	template< class _Real > class AABox; //Orientated box
	template< class _Real > class OBox; //Orientated box
	template < class _Real, Count _Rows, Count _Cols > class MatrixT;


	/** \brief 3D Sphere
	*/
	template < typename _Real = Real >
	class Sphere
	{
	public:
		typedef VectorT<_Real,3> _Vector;
	public: 
		/** \brief Constructor defalts to a zero sized box 
		*/
		Sphere();

		/** \brief Constructor from a center position and a radius 
		*/
		Sphere( const _Vector& center, _Real radius );	

		/** \brief Constructor from an array of vector positions 
		\remarks
			This function performs a first pass to make array construction much faster 
		*/
		Sphere( const typename _Vector::Array& va );	

		/** \brief Constructor to contain a bounding box completely including it 8 corners
			\remarks Use 'fitInBounds( AABox.. )' to calculate a sphere that fits inside the shape instead
		*/
		Sphere( const AABox<_Real>& box );

		/** \brief Transform the sphere by a matrix and resizes to fit the resulting elipsoid 
			\todo Support arbitrary rotations and translations etc
		*/
		void transform( const MatrixT<_Real,4,4>& matrix );
		void transform( const MatrixT<_Real,3,3>& matrix );

		/** \brief Expand the box to include the supplied vector within its shape */
		void expand( const _Vector& v );

		/** \brief Expand the box to include the supplied vector array
		\remarks 
			Use the constructor from an array for optimised creation of a sphere from points 
		*/
		void expand( const typename _Vector::Array& va );

		/** \brief Expand to include the supplied sphere 
		*/
		void expand( const AABox<_Real>& box );	

		/** \brief Expand to include the supplied bounding sphere 
		*/
		void expand( const Sphere<_Real>& sphere );	

		/** \brief Checks if the sphere contains the supplied vector position 
		*/
		Bool contains( const _Vector& v ) const;

		/** \brief Checks if this Sphere intersects with a box 
		*/
		Bool intersects( const AABox<_Real>& box ) const { return false; }

		/** \brief Returns the radius of the sphere 
		*/
		_Real radius() const 
		{ return m_radius; }

		/** \brief Returns the squared radius of the sphere 
		*/
		_Real radiusSquared() const 
		{ return m_radiusSquared; }

		/** \brief Returns the center of the sphere 
		*/
		_Vector center() const 
		{ return m_center; } 

		/** \brief Checks if the sphere has been initialised and is valid 
		*/
		Bool isValid() const 
		{ return m_radius != 0; } 

		/** \brief This function takes a vector and returns the nearest point on the spheres surface
		\param[in] vec Vector position to test against
		\return	
			The nearest point on the surface of the sphere 
		*/
		_Vector closestPoint( const _Vector& vec ) const;

	private:
		_Vector m_center; ///< The center of the sphere
		_Real m_radius; ///< The radius of the sphere
		_Real m_radiusSquared; ///< The squared radius of the sphere
	};

	
	template < typename _Real >
	inline Sphere<_Real>::Sphere() : 
		m_center( _Vector( (_Real)0.0) ), m_radius( (_Real)0 ) 
	{}

	template < typename _Real >
	inline Sphere<_Real>::Sphere( const _Vector& center, _Real radius ) :
		m_center( center ), m_radius( radius ) 
	{}	



	template < typename _Real >
	inline void Sphere<_Real>::transform( const MatrixT<_Real,4,4>& matrix )
	{
		Assert( false );
	}

	template < typename _Real >
	inline void Sphere<_Real>::transform( const MatrixT<_Real,3,3>& matrix )
	{
		Assert( false );
	}

	template < typename _Real >
	void Sphere<_Real>::expand( const _Vector& v )
	{
		if ( !isValid() ) //If the sphere is not valid then this is the first point added
		{
			m_center = v;
			return;
		}

		_Real distSquared = ( v - m_center ).magnitudeSquared();
		if ( distSquared > m_radiusSquared )
		{
			_Real dist = (_Real)fastSqrt( (Real)distSquared );
			m_radius = (m_radius + dist) * (_Real)0.5;
			m_radiusSquared = (_Real)sqr( (Real)m_radius );
			_Real offsetDist = dist - m_radius;
			
			//Calculate the new sphere center
			m_center.x = ( (m_radius * m_center.x) + (offsetDist * v.x) ) / dist;
			m_center.y = ( (m_radius * m_center.y) + (offsetDist * v.y) ) / dist;
			m_center.z = ( (m_radius * m_center.z) + (offsetDist * v.z) ) / dist;
		}
	}

	template < typename _Real >
	inline void Sphere<_Real>::expand( const typename _Vector::Array& va )
	{
		for ( _Vector::Array::const_iterator iter = va.begin(); iter != va.end(); ++iter )
			expand( (*iter) );
	}

	template < typename _Real >
	void Sphere<_Real>::expand( const Sphere<_Real>& sphere )
	{
		//TODO: Check and verify this function is correct
		_Vector offset( sphere.center() - m_center );
		_Real dist = offset.magnitude() +  radius();
		offset.normalise() *= dist;
		expand( offset );
	}			

	template < typename _Real >
	inline Bool Sphere<_Real>::contains( const _Vector& v ) const
	{
		if ( isValid() )
			return ( v - m_center ).magnitudeSquared() <= m_radius;
		else
			return false;
	}

	template < typename _Real >
	inline typename Sphere<_Real>::_Vector Sphere<_Real>::closestPoint( const _Vector& v ) const
	{
		//Unit vector from center to the point scaled to the radiyus...
		return m_center + (v - m_center).normalise() * m_radius;
	}

} // END: math

#endif