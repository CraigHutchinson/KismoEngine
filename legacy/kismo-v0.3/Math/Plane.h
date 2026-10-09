/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\Math\Plane.h
 * Status: superseded by kismo-core/_SOURCE_/KismoFramework/Plane.h (same name, content differs)
 */
#ifndef FRX_PLANE_H
#define FRX_PLANE_H

#include "Math/Vector.h"

namespace math
{
	/** \brief Templated plane class for deifning a plane in 3d
	*/
	template< class _TReal >
	class _DllExport PlaneT
	{
	public:

		/** \brief typedef for convenience 
		*/
		typedef VectorN<_TReal,3 > Vector_T;

	public:
		
		Vector_T n; ///< The normal of the plane
		_TReal d;  ///< distance from the plane to the origin

		/** \brief Default constructor does nothing for efficiency
		*/
		PlaneT(){}

		/** \brief PlaneT constructor from a normal and a point on the plane
			//TODO: implement this
		*/
		//PlaneT( const Vector_T& normal, const Vector_T& position ) :			
		//{
		//}

		PlaneT( const Vector_T& v1, const Vector_T& v2, const Vector_T& v3 )  :
			n( ( Vector_T( v2 - v1 ).cross( v3 - v1 ) ).normalised() ),
			d( n.dot( v1 ) )
		{}

		/** \brief Get the diatance of a point from the plan
		*/
		_TReal distanceTo( const Vector_T& p ) const
		{ return n.dot( p ) + d; }

		/** \brief Casts a ray onto the plane
			\param[in] start The start position of the ray
			\param[in] dir The direction vector of the ray
			\param[out] distance (optional) Ouputs the distance to the intersection
			\param[out] intersection (optional) The position of intersection
			\return Whether interesection occurs. False if ray is perpendicular to the plane or pointing away
		*/
		Bool intersect( const Vector_T& start, const Vector_T& dir, _TReal* distance = 0 , Vector_T* intersection = 0 ) const
		{
			//get the dot product
			_TReal dirDotNorm = dir.dot( n );
			
			//check that direction of the ray is towards the plane an will intersect
			if ( dirDotNorm <= (_TReal)0.0 )
				return false;

			_TReal dist = distanceTo( start );
			if ( distance )
				*distance = dist;

			if ( intersection ) //work out the point of plane intersection
			{
				*intersection = start + (dir * -dist / dirDotNorm);;
			}

			return true;
		}

	};

	typedef PlaneT< Real > Plane;
	typedef PlaneT< Float32 > Planef;
	typedef PlaneT< Float64 > Planed;

} //END: math

#endif