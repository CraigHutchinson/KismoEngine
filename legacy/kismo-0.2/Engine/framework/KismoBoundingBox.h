/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\framework\KismoBoundingBox.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef _KISMO_BOUNDINGBOX_H
#define	_KISMO_BOUNDINGBOX_H

#include "framework/KismoTypes.h"
#include "framework/KismoMath.h"
#include "framework/KismoVector.h"

namespace kismo
{
	namespace frx
	{
		//Forward Declaration
		class Matrix4x4; 
		class Matrix3x3;
		class BoundingSphere;

		/** \brief 3D Axis aligned Bounding box */
		class _KismoExport BoundingBox
		{
		public: 
			/** \brief Constructor defalts to a zero sized box */
			BoundingBox();
			/** \brief Constructor from the minimum & maximum corners of a box */
			BoundingBox( const Vector3& minimum, const Vector3& maximum );	
			/** \brief Constructor from an array of vector positions */
			BoundingBox( const Vector3::Array& va );				
			/** \brief Constructor to contain a bounding sphere */
			BoundingBox( const BoundingSphere& sphere );
			/** \brief Transform the bounding box by a matrix and resizing to fit the resulting box */
			void transform( const Matrix4x4& matrix );
			void transform( const Matrix3x3& matrix );
			/** \brief Expand the box to include the supplied vector */
			void expand( const Vector3& v );
			/** \brief Expand the box to include the supplied vector array */
			void expand( const Vector3::Array& va );
			/** \brief Expand to include the supplied bounding box */
			void expand( const BoundingBox& box );	
			/** \brief Expand to include the supplied bounding sphere */
			void expand( const BoundingSphere& sphere );			
			/** \brief Checks if the bounding box contains the supplied vector position */
			k_Bool contains( const Vector3& v ) const;
			/** \brief Checks if this box intersects with another
			\param tolerance*/
			k_Bool intersects( const BoundingBox& box ) const;
			/** \brief Returns the width of the box in the X-axis */
			k_Real width() const { return m_max.x - m_min.x; }
			/** \brief Returns the height of the box in the Y-axis */
			k_Real height() const { return m_max.y - m_min.y; }
			/** \brief Returns the height of the box in the Z-axis */
			k_Real depth() const { return m_max.z - m_min.z; }
			/** \brief Returns the center of the box */
			Vector3	center() const { return Vector3( (m_min.x+m_max.x)*(k_Real)0.5, (m_min.y+m_max.y)*(k_Real)0.5, (m_min.z+m_max.z)*(k_Real)0.5); } 
			/** \brief Returns the minimum points of the box
			@remarks This is the corner with the minimum x,y,&z value */
			const Vector3& min() const { return m_min; }
			/** \brief Returns the maximum points of the box
			@remarks This is the corner with the maximum x,y,&z value */
			const Vector3& max() const { return m_max; }
			/** \brief Checks if the box has been initialised and is valid */
			k_Bool isValid() const { return (m_min.x != m_max.x) && (m_min.y != m_max.y) && (m_min.z != m_max.z); } 
			/** \brief This function takes a vector and returns the nearest point on the box
			\param[in] c Vector positon to test against
			\param[out] p Closest point on the box */
			void closestPoint( Vector3& p, const Vector3& c ) const;			
			/** \brief Calculates the squared distance of a point from the box .
			\param v The position vector to test against
			\return	The squared distance from the box to v */
			inline k_Real distanceSquared( const Vector3& v ) const;		
			/** \brief Calculates the  distance of a point from the box .
			\param v The position vector to test against
			\return	The distance from the box to v */
			inline k_Real distance( const Vector3& v ) const 
			{ return math::fastSquareRoot( distanceSquared( v ) ); }
		private: 			
			Vector3 m_min; ///< The minimum corner of the box
			Vector3 m_max; ///< The maximum corner of the box
		};

		inline BoundingBox::BoundingBox() : 
			m_min( Vector3(0,0,0) ), m_max( Vector3(0,0,0) ) 
		{}

		inline BoundingBox::BoundingBox( const Vector3& minimum, const Vector3& maximum ) :
			m_min( minimum ), m_max( maximum ) 
		{}			

		inline BoundingBox::BoundingBox( const Vector3::Array& va )
		{ 
			m_min = m_max = ( va.size() ? va[0] : Vector3(0,0,0) );
			expand( va );
		}	

		inline void BoundingBox::transform( const Matrix4x4& matrix )
		{
			assert( false );
		}
		inline void BoundingBox::transform( const Matrix3x3& matrix )
		{
			assert( false );
		}

		inline void BoundingBox::expand( const Vector3::Array& va )
		{
			for ( Vector3::Array::const_iterator iter = va.begin(); iter != va.end(); ++iter )
				expand( (*iter) );
		}

		inline void BoundingBox::expand( const Vector3& v )
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

		inline void BoundingBox::expand( const BoundingBox& box )
		{
			expand( box.min() );
			expand( box.max() );
		}			

		inline k_Bool BoundingBox::contains( const Vector3& v ) const
		{
			return (v.x >= m_min.x) && (v.y >= m_min.y) && (v.z >= m_min.z) 
				&& (v.x <= m_max.x) && (v.y <= m_max.y) && (v.z <= m_max.z); 
		}
		


	
	} // END: frx

} // END: kismo

#endif
