/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\Gfx\GeometryMath.h
 * Status: superseded by kismo-v0.3/Gfx/GeometryMath.h (same name, content differs)
 */
#ifndef GFX_GEOMETRYMATH_H
#define GFX_GEOMETRYMATH_H

#include "Framework/Types.h"
#include "Framework/Vector.h"

namespace gfx
{
	// Real time collision detection by Christer Ericson p 191
	bool _DllExport rayInTriangle( const frx::Vector3r& start, const frx::Vector3r& dir, 
					   const frx::Vector3r& v1, const frx::Vector3r& v2, const frx::Vector3r& v3 );

	bool _DllExport intersectSweeptSpherePlane( const frx::Vector3r& start, const frx::Vector3r& direction, Real radius,
		const frx::Vector3r& point1, const frx::Vector3r& point2, const frx::Vector3r& point3, 
		Real* distance, frx::Vector3r* intersect );
	
	bool _DllExport pointInTriangle( const frx::Vector3r& p, 
		frx::Vector3r a, frx::Vector3r b, frx::Vector3r c );

} //END: gfx

#endif