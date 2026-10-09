/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\KismoMeshLoader\Types.h
 * Status: superseded by kismo-core/_SOURCE_/KismoFramework/Types.h (same name, content differs)
 */
#ifndef Kismo_TYPES_H
#define Kismo_TYPES_H

#include "Math/Vector.h"

namespace rage
{
	typedef UInt32 EdgeIndex;
	typedef UInt16 PositionIndex;
	typedef UInt16 NormalIndex;
	typedef UInt16 UVIndex;

	typedef math::Vector3<Real> Normal;
	typedef math::Vector3<Real> Position;
	typedef frx::Vector2<Real> UV;

} //END: rage

#endif