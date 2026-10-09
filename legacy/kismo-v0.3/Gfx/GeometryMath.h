/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\Gfx\GeometryMath.h
 * Status: current (newest copy of this file name in the archive)
 */
#ifndef GFX_GEOMETRYMATH_H
#define GFX_GEOMETRYMATH_H

#include "Framework/Types.h"
#include "Math/Vector.h"

namespace gfx
{
	// Real time collision detection by Christer Ericson p 191
	bool _DllExport rayInTriangle( const math::Vector3& start, const math::Vector3& dir, 
					   const math::Vector3& v1, const math::Vector3& v2, const math::Vector3& v3 );

	bool _DllExport intersectSweeptSpherePlane( const math::Vector3& start, const math::Vector3& direction, Real radius,
		const math::Vector3& point1, const math::Vector3& point2, const math::Vector3& point3, 
		Real* distance, math::Vector3* intersect );
	
	bool _DllExport pointInTriangle( const math::Vector3& p, 
		math::Vector3 a, math::Vector3 b, math::Vector3 c );

} //END: gfx

#endif