/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\framework\KismoBoundingSphere.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef _KISMO_BOUNDINGSPHERE_H
#define	_KISMO_BOUNDINGSPHERE_H

#include "framework/KismoCommon.h"
#include "framework/KismoVector.h"

namespace kismo
{
	namespace frx
	{
		//Forward Declaration
		class Matrix4x4; 
		class Matrix3x3;
		class BoundingBox;

		/** \brief 3D Axis aligned Bounding box */
		class _KismoExport BoundingSphere
		{
		public: 
			/** \brief Constructor defalts to a zero sized box */
			BoundingSphere();
			/** \brief Constructor from a center position and a radius */
			BoundingSphere( const Vector3& center, k_Real radius );	
			/** \brief Constructor from an array of vector positions 
			@remarks This function performs a first pass to make array construction much faster  */
			BoundingSphere( const Vector3::Array& va );			
			/** \brief Constructor to contain a bounding box */
			BoundingSphere( const BoundingBox& box );
			/** \brief Transform the bounding box by a matrix and resizing to fit the resulting box */
			void transform( const Matrix4x4& matrix );
			void transform( const Matrix3x3& matrix );
			/** \brief Expand the box to include the supplied vector */
			void expand( const Vector3& v );			
			/** \brief Expand the box to include the supplied vector array
			@remarks Use the array constructor for optimised creation of a sphere from points */
			void expand( const Vector3::Array& va );
			/** \brief Expand to include the supplied bounding box */
			void expand( const BoundingBox& box );	
			/** \brief Expand to include the supplied bounding sphere */
			void expand( const BoundingSphere& sphere );			
			/** \brief Checks if the bounding box contains the supplied vector position */
			k_Bool contains( const Vector3& v ) const;
			/** \brief Checks if this Sphere intersects with a box */
			k_Bool intersects( const BoundingBox& box ) const;
			/** \brief Returns the radius of the sphere */
			k_Real radius() const { return m_radius; }
			/** \brief Returns the squared radius of the sphere */
			k_Real radiusSquared() const { return m_radiusSquared; }
			/** \brief Returns the center of the sphere */
			Vector3	center() const { return m_center; } 
			/** \brief Checks if the sphere has been initialised and is valid */
			k_Bool isValid() const { return m_radius != 0 && m_center != 0; } 
			/** \brief This function takes a vector and returns the nearest point on the spheres surface
			\param v Vector position to test against
			\return	The nearest point on the surface of the sphere */
			Vector3 closestPoint( const Vector3& v ) const;
		private:
			Vector3 m_center; ///< The center of the sphere
			k_Real m_radius; ///< The radius of the sphere
			k_Real m_radiusSquared; ///< The squared radius of the sphere
		};

		inline BoundingSphere::BoundingSphere() : 
			m_center( (k_Real)0.0 ), m_radius( 0 ) 
		{}

		inline BoundingSphere::BoundingSphere( const Vector3& center, k_Real radius ) :
			m_center( center ), m_radius( radius ) 
		{}			


		inline void BoundingSphere::transform( const Matrix4x4& matrix )
		{
			assert( false );
		}
		inline void BoundingSphere::transform( const Matrix3x3& matrix )
		{
			assert( false );
		}

		inline void BoundingSphere::expand( const Vector3& v )
		{
			if ( !isValid() ) //If the sphere is not valid then this is the first point added
			{
				m_center = v;
				return;
			}

			k_Real distSquared = ( v - m_center ).lengthSquared();
			if ( distSquared > m_radiusSquared )
			{
				k_Real dist = math::fastSquareRoot( distSquared );
				m_radius = (m_radius + dist) * (k_Real)0.5;
				m_radiusSquared = math::square( m_radius );
				k_Real offsetDist = dist - m_radius;
				
				//Calculate the new sphere center
				m_center.x = ( (m_radius * m_center.x) + (offsetDist * v.x) ) / dist;
				m_center.y = ( (m_radius * m_center.y) + (offsetDist * v.y) ) / dist;
				m_center.z = ( (m_radius * m_center.z) + (offsetDist * v.z) ) / dist;
			}
		}

		inline void BoundingSphere::expand( const BoundingSphere& sphere )
		{
			//TODO: Check and verify this function is correct
			Vector3 offset = ( sphere.center() - m_center );
			k_Real dist = offset.length() +  radius();
			offset.normalise() *= dist;
			expand( offset );
		}			

		inline k_Bool BoundingSphere::contains( const Vector3& v ) const
		{
			if ( isValid() )
				return ( v - m_center ).lengthSquared() <= m_radius;
			else
				return false;
		}

		inline Vector3 BoundingSphere::closestPoint( const Vector3& v ) const
		{
			//Unit vector from center to the point scaled to the radiyus...
			return m_center + (v - m_center).normalise() * m_radius;
		}
	
	} // END: frx

} // END: kismo

#endif