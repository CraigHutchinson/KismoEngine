/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\KismoCore\_SOURCE_\KismoFramework\Plane.h
 * Status: current (newest copy of this file name in the archive)
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

#ifndef KISMOFRAMEWORK_PLANE_H
#define KISMOFRAMEWORK_PLANE_H

#include "KismoFramework/VectorN.h"

namespace kismo
{
	/** \brief Templated plane class for deifning a plane in 3d
	*/
	template< class _Real >
	class /*KISMOFRAMEWORK_EXPORT*/ PlaneT
	{
	public:

		/** \brief typedef for convenience 
		*/
		typedef VectorN<_Real,3 > _Vector;

	public:
		
		_Vector n; ///< The normal of the plane
		_Real d;  ///< distance from the plane to the origin

		/** \brief Default constructor does nothing for efficiency
		*/
		PlaneT(){}

		/** \brief PlaneT constructor from a normal and a point on the plane
			//TODO: implement this
		*/
		//PlaneT( const _Vector& normal, const _Vector& position ) :			
		//{
		//}

		PlaneT( const _Vector& v1, const _Vector& v2, const _Vector& v3 )  :
			n( ( _Vector( v2 - v1 ).cross( v3 - v1 ) ).normalised() ),
			d( n.dot( v1 ) )
		{}

		/** \brief Get the diatance of a point from the plan
		*/
		_Real distanceTo( const _Vector& p ) const
		{ return n.dot( p ) + d; }

		/** \brief Casts a ray onto the plane
			\param[in] start The start position of the ray
			\param[in] dir The direction vector of the ray
			\param[out] distance (optional) Ouputs the distance to the intersection
			\param[out] intersection (optional) The position of intersection
			\return Whether interesection occurs. False if ray is perpendicular to the plane or pointing away
		*/
		Bool intersect( const _Vector& start, const _Vector& dir, _Real* distance = 0 , _Vector* intersection = 0 ) const
		{
			//get the dot product
			_Real dirDotNorm = dir.dot( n );
			
			//check that direction of the ray is towards the plane an will intersect
			if ( dirDotNorm <= (_Real)0.0 )
				return false;

			_Real dist = distanceTo( start );
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

} //END: kismo

#endif