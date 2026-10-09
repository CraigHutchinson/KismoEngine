/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoInput\Common.h
 * Status: superseded by kismo-core/_SOURCE_/KismoGL/Common.h (same name, content differs)
 */
#ifndef KISMOINPUT_COMMON_H
#define KISMOINPUT_COMMON_H

#include "KismoFRX/Types.h"

// Dll export interface
#ifdef KISMOINPUT_EXPORTS	
#	define KISMOINPUT_EXPORT __declspec(dllexport)
#else	
#	define KISMOINPUT_EXPORT __declspec(dllimport)
#endif 

namespace kismo { namespace io
{	
} /* END: io*/ } /*END: Kismo*/

#endif