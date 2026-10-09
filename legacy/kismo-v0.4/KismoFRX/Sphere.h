/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoFRX\Sphere.h
 * Status: superseded by kismo-core/_SOURCE_/KismoFramework/Sphere.h (same name, content differs)
 */
#ifndef KISMOFRX_SPHERE_H
#define	KISMOFRX_SPHERE_H

#include "KismoFRX/Common.h"
#include "KismoFRX/VectorN.h"

namespace kismo
{
	//Forward Declaration
	template< class _TReal > class AABox; //Orientated box
	template< class _TReal > class OBox; //Orientated box
	template < class _Real, Count _Rows, Count _Cols > class MatrixT;


	/** \brief 3D Sphere
	*/
	template < typename _TReal = Real >
	class KISMOFRX_EXPORT Sphere
	{
	public:
		typedef VectorT<_TReal,3> VectorT;
	public: 
		/** \brief Constructor defalts to a zero sized box 
		*/
		Sphere();

		/** \brief Constructor from a center position and a radius 
		*/
		Sphere( const VectorT& center, _TReal radius );	

		/** \brief Constructor from an array of vector positions 
		\remarks
			This function performs a first pass to make array construction much faster 
		*/
		Sphere( const typename VectorT::Array& va );	

		/** \brief Constructor to contain a bounding box completely including it 8 corners
			\remarks Use 'fitInBounds( AABox.. )' to calculate a sphere that fits inside the shape instead
		*/
		Sphere( const AABox<_TReal>& box );

		/** \brief Transform the sphere by a matrix and resizes to fit the resulting elipsoid 
			\TODO: Support arbitrary rotations and translations etc
		*/
		void transform( const MatrixT<_TReal,4,4>& matrix );
		void transform( const MatrixT<_TReal,3,3>& matrix );

		/** \brief Expand the box to include the supplied vector within its shape */
		void expand( const VectorT& v );

		/** \brief Expand the box to include the supplied vector array
		\remarks 
			Use the constructor from an array for optimised creation of a sphere from points 
		*/
		void expand( const typename VectorT::Array& va );

		/** \brief Expand to include the supplied sphere 
		*/
		void expand( const AABox<_TReal>& box );	

		/** \brief Expand to include the supplied bounding sphere 
		*/
		void expand( const Sphere<_TReal>& sphere );	

		/** \brief Checks if the sphere contains the supplied vector position 
		*/
		Bool contains( const VectorT& v ) const;

		/** \brief Checks if this Sphere intersects with a box 
		*/
		Bool intersects( const AABox<_TReal>& box ) const { return false; }

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
		VectorT center() const 
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
		VectorT closestPoint( const VectorT& vec ) const;

	private:
		VectorT m_center; ///< The center of the sphere
		_TReal m_radius; ///< The radius of the sphere
		_TReal m_radiusSquared; ///< The squared radius of the sphere
	};

	
	template < typename _TReal >
	inline Sphere<_TReal>::Sphere() : 
		m_center( VectorT( (_TReal)0.0) ), m_radius( (_TReal)0 ) 
	{}

	template < typename _TReal >
	inline Sphere<_TReal>::Sphere( const VectorT& center, _TReal radius ) :
		m_center( center ), m_radius( radius ) 
	{}	



	template < typename _TReal >
	inline void Sphere<_TReal>::transform( const MatrixT<_TReal,4,4>& matrix )
	{
		assert( false );
	}

	template < typename _TReal >
	inline void Sphere<_TReal>::transform( const MatrixT<_TReal,3,3>& matrix )
	{
		assert( false );
	}

	template < typename _TReal >
	void Sphere<_TReal>::expand( const VectorT& v )
	{
		if ( !isValid() ) //If the sphere is not valid then this is the first point added
		{
			m_center = v;
			return;
		}

		_TReal distSquared = ( v - m_center ).magnitudeSquared();
		if ( distSquared > m_radiusSquared )
		{
			_TReal dist = fastSqrt( distSquared );
			m_radius = (m_radius + dist) * (_TReal)0.5;
			m_radiusSquared = sqr( m_radius );
			_TReal offsetDist = dist - m_radius;
			
			//Calculate the new sphere center
			m_center.x = ( (m_radius * m_center.x) + (offsetDist * v.x) ) / dist;
			m_center.y = ( (m_radius * m_center.y) + (offsetDist * v.y) ) / dist;
			m_center.z = ( (m_radius * m_center.z) + (offsetDist * v.z) ) / dist;
		}
	}

	template < typename _TReal >
	inline void Sphere<_TReal>::expand( const typename VectorT::Array& va )
	{
		for ( VectorT::Array::const_iterator iter = va.begin(); iter != va.end(); ++iter )
			expand( (*iter) );
	}

	template < typename _TReal >
	void Sphere<_TReal>::expand( const Sphere<_TReal>& sphere )
	{
		//TODO: Check and verify this function is correct
		VectorT offset = ( sphere.center() - m_center );
		_TReal dist = offset.magnitude() +  radius();
		offset.normalise() *= dist;
		expand( offset );
	}			

	template < typename _TReal >
	inline Bool Sphere<_TReal>::contains( const VectorT& v ) const
	{
		if ( isValid() )
			return ( v - m_center ).magnitudeSquared() <= m_radius;
		else
			return false;
	}

	template < typename _TReal >
	inline typename Sphere<_TReal>::VectorT Sphere<_TReal>::closestPoint( const VectorT& v ) const
	{
		//Unit vector from center to the point scaled to the radiyus...
		return m_center + (v - m_center).normalise() * m_radius;
	}

} // END: math

#endif