/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\framework\KismoPlane.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef _KISMO_PLANE_H
#define	_KISMO_PLANE_H

#include "framework/KismoCommon.h"
#include "framework/KismoVector.h"

namespace kismo
{
	namespace frx
	{
		//Forward Declaration
		class Matrix4x4; 
		class Matrix3x3;

		/** \brief 3D Plane */
		class _KismoExport Plane
		{
		public:

			typedef k_Vector< Plane > Array;
			
			// special planes
			static const Plane ZERO;

		public: 
			
			Vector3 n; ///< Plane normal/coefficient
			k_Real d; ///< Plane equation constant

			/** \brief Default constructor
			\remarks WARNING: does not initialise the plane for efficiency */
			inline Plane() {}
			/** \brief Constructor			
			\param	x X coefficient of the plane equation
			\param	y X coefficient of the plane equation
			\param	z Y coefficient of the plane equation
			\param	nd D constant of plane equation */
			inline Plane( const k_Real x, const k_Real y, const k_Real z, const k_Real nd ) :
				n( Vector3( x, y, z ) ), d( nd ) {}
			/** \brief Constructor from the surface normal and plane coefficient of the plane
			\param v Surface normal of the plane
			\param d Plane coefficient */
			inline Plane( const Vector3& normal, const k_Real nd ) :
				n( normal ), d( nd ) {}
			/** \brief Constructs a plane from 3 vertices ( a triangle) */
			Plane( const Vector3& v1, const Vector3& v2, const Vector3& v3)
			{
				setNormal( ( v3 - v2 ).cross( v1 - v2 ).normalise() );
				d = -n.dot( v1 );
			}			
			/** \brief Set the plane normal */
			inline void setNormal( const Vector3& normal )
			{ n = normal; }
			/** \brief Gets the normal of the plane */
			inline const Vector3& normal() const
			{ return n; }
			/** \brief Calculates the distance of a point from the plane. if the point is
			behind the plane the distance will be negative
			\param v The position vector to test against
			\return	The distance from the plane to v */
			inline k_Real distance( const Vector3& v ) const
			{ return normal().dot( v ) + d; }

			/** \brief Calculates the closest point on the plane from another point
			\param v Point to project to the plane
			\return Closest point on the plane */
			inline Vector3 closestPoint( const Vector3& v ) const
			{
				k_Real t = n.dot( v ) - d;
				return v - (n * t);
			}

			 /*inline XReal LineIntersection( IN const XVector &origin , IN const XVector &direction ) const
			{
				return -((Normal() * origin) + d)/(Normal() * direction);
			}

			/// \brief	a function to find out if the plane equation is valid
			/// \return	true if equation is OK
			/// \note
			///
			inline bool Valid() const
			{
				float l = Normal().Length();
				return ( ( l > 0.0f ) && ( ( 1.0f/l ) > 0.0f ) );
			}*/
		};


	
	} // END: frx

} // END: kismo

#endif
