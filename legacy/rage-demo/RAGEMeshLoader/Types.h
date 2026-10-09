/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\RAGEMeshLoader\Types.h
 * Status: superseded by kismo-core/_SOURCE_/KismoFramework/Types.h (same name, content differs)
 */
#ifndef RAGE_TYPES_H
#define RAGE_TYPES_H

#include "Framework/Vector.h"

namespace rage
{
	typedef UInt32 EdgeIndex;
	typedef UInt16 PositionIndex;
	typedef UInt16 NormalIndex;
	typedef UInt16 UVIndex;

	typedef frx::Vector3<Real> Normal;
	typedef frx::Vector3<Real> Position;
	typedef frx::Vector2<Real> UV;

} //END: rage

#endif