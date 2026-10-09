/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\Gfx\GeometryMath.cpp
 * Status: superseded by kismo-v0.3/Gfx/GeometryMath.cpp (same name, content differs)
 */
#include "Gfx/GeometryMath.h"
#include "Framework/Plane.h"

namespace gfx
{

	bool rayInTriangle( const frx::Vector3r& start, const frx::Vector3r& dir, 
					   const frx::Vector3r& v1, const frx::Vector3r& v2, const frx::Vector3r& v3 ) 
	{
		//Get the triangles edges
		frx::Vector3r e1 = v2 - v1;
		frx::Vector3r e2 = v3 - v1;
		frx::Vector3r qp = -dir;

		//get the normal of the plane
		frx::Vector3r n = e1.cross( e2 );

		//get the dot product
		Real d = qp.dot( n );
		
		//check that direction of the ray is towards the plane an will intersect
		if ( d <= (Real)0.0 )
			return false;
		
		//assert( dir.lengthSquared() == 1 );
		//check for intersection by the ray. The direction should be normalised already. assert to check!
		frx::Vector3r ap = start - v1;
		Real t = ap.dot( n );
		if ( t < (Real)0.0 )
			return false;

		// if ( t > d ) return 0; ///< For segment test

		//Compute the barycentric coordinates components and test within bounds
		frx::Vector3r e = qp.cross( ap );
		Real v = e2.dot( e );
		if ( v < (Real)0.0 || v > d )
			return false;
		Real w = -e1.dot( e );
		if ( w < (Real)0.0 || v + w > d )
			return false;


		return true;
	}

	bool intersectSweeptSpherePlane( const frx::Vector3r& start, const frx::Vector3r& direction, Real radius,
		const frx::Vector3r& point1, const frx::Vector3r& point2, const frx::Vector3r& point3, 
		Real* distance, frx::Vector3r* intersect )
	{
		frx::Planer p( point1, point2, point3 );

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

	bool pointInTriangle( const frx::Vector3r& p, 
		frx::Vector3r a, frx::Vector3r b, frx::Vector3r c )
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