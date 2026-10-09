/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoGFX\Common.h
 * Status: superseded by kismo-core/_SOURCE_/KismoGL/Common.h (same name, content differs)
 */
#ifndef KISMOGFX_COMMON_H
#define KISMOGFX_COMMON_H

#include "KismoFRX/Types.h"

// Dll export interface
#ifdef KISMOGFX_EXPORTS	
#	define KISMOGFX_EXPORT __declspec(dllexport)
#else	
#	define KISMOGFX_EXPORT __declspec(dllimport)
#endif 

namespace kismo { namespace gfx
{
} /* END: gfx*/ } /*END: Kismo*/

#endif