/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\Gfx\GeometryMath.cpp
 * Status: current (newest copy of this file name in the archive)
 */
#include "Gfx/GeometryMath.h"
#include "Math/Plane.h"

namespace gfx
{

	bool rayInTriangle( const math::Vector3& start, const math::Vector3& dir, 
					   const math::Vector3& v1, const math::Vector3& v2, const math::Vector3& v3 ) 
	{
		//Get the triangles edges
		math::Vector3 e1 = v2 - v1;
		math::Vector3 e2 = v3 - v1;
		math::Vector3 qp = -dir;

		//get the normal of the plane
		math::Vector3 n = e1.cross( e2 );

		//get the dot product
		Real d = qp.dot( n );
		
		//check that direction of the ray is towards the plane an will intersect
		if ( d <= (Real)0.0 )
			return false;
		
		//assert( dir.magnitudeSquared() == 1 );
		//check for intersection by the ray. The direction should be normalised already. assert to check!
		math::Vector3 ap = start - v1;
		Real t = ap.dot( n );
		if ( t < (Real)0.0 )
			return false;

		// if ( t > d ) return 0; ///< For segment test

		//Compute the barycentric coordinates components and test within bounds
		math::Vector3 e = qp.cross( ap );
		Real v = e2.dot( e );
		if ( v < (Real)0.0 || v > d )
			return false;
		Real w = -e1.dot( e );
		if ( w < (Real)0.0 || v + w > d )
			return false;


		return true;
	}

	bool intersectSweeptSpherePlane( const math::Vector3& start, const math::Vector3& direction, Real radius,
		const math::Vector3& point1, const math::Vector3& point2, const math::Vector3& point3, 
		Real* distance, math::Vector3* intersect )
	{
		math::Plane p( point1, point2, point3 );

		Real dist = p.n.dot( start ) - p.d;
		if ( fabs(dist) <= radius )
		{
			//Already intersecting with the plane
			if ( distance ) *distance = (Real)0.0;
			if ( intersect ) *intersect = start;
			return true;
		}
		else
		{
			Real denom = p.n.dot( direction );
			if ( denom * dist >= (Real)0.0 )
			{
				//No intersect as the sphere is moving parallel or away from teh plane
				return false; 
			}
			else
			{
				//Sphere is monving towards he plane
				Real r = dist > (Real)0.0 ? radius : -radius;
				Real t = (r - dist) / denom;
				if ( distance ) *distance = t;
				if ( intersect ) *intersect = start + (direction * t) - (p.n * r);
				return true;
			}
		}				
	}

	bool pointInTriangle( const math::Vector3& p, 
		math::Vector3 a, math::Vector3 b, math::Vector3 c )
	{
		// Translate point and triangle so the point lies at the origin
		a -= p;
		b -= p;
		c -= p;

		Real ab = a.dot( b );
		Real ac = a.dot( c );
		Real bc = b.dot( c );
		Real cc = c.dot( c );

		//Make sure the plane normals for pab and pbc point int he same direction
		if ( bc * ac - cc * ab < (Real)0.0 )
			return false;

		//Make sure the plane normals for pab and pca point in the same direction
		Real bb = b.dot( b );
		if ( ab * bc - ac * bb < (Real)0.0 )
			return false;

		//P must be in or on the triangle
		return true;
	}

	
} //END; gfx