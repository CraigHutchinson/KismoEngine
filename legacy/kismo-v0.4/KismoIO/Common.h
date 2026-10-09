/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoIO\Common.h
 * Status: superseded by kismo-core/_SOURCE_/KismoGL/Common.h (same name, content differs)
 */
#ifndef KISMOIO_COMMON_H
#define KISMOIO_COMMON_H

#include "KismoFRX/Types.h"

// Dll export interface
#ifdef KISMOIO_EXPORTS	
#	define KISMOIO_EXPORT __declspec(dllexport)
#else	
#	define KISMOIO_EXPORT __declspec(dllimport)
#endif 

namespace kismo { namespace io
{	
} /* END: io*/ } /*END: Kismo*/

#endif