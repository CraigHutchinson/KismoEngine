/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoPlatform\Common.h
 * Status: superseded by kismo-core/_SOURCE_/KismoGL/Common.h (same name, content differs)
 */
#ifndef KISMOPLATFORM_COMMON_H
#define KISMOPLATFORM_COMMON_H

#include "KismoFRX/Types.h"

// Dll export interface
#ifdef KISMOPLATFORM_EXPORTS	
#	define KISMOPLATFORM_EXPORT __declspec(dllexport)
#else	
#	define KISMOPLATFORM_EXPORT __declspec(dllimport)
#endif 

namespace kismo { namespace platform
{	
} /* END: Platform*/ } /*END: Kismo*/

// The following macros define the minimum required platform.
#ifndef WINVER                          // Specifies that the minimum required platform is Windows Vista.
#define WINVER 0x0600           // Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINNT            // Specifies that the minimum required platform is Windows Vista.
#define _WIN32_WINNT 0x0600     // Change this to the appropriate value to target other versions of Windows.
#endif

#endif