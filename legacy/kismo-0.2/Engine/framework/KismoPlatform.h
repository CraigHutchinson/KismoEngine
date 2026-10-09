/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\framework\KismoPlatform.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef _KISMO_PLATFORM_H
#define _KISMO_PLATFORM_H

#if defined KISMO_STATIC_BUILD
#	define _KismoExport
#	define _DllExport
#else
#	if defined( KISMO_NONCLIENT_BUILD )
#		define _KismoExport __declspec( dllexport )
#		define _DllExport __declspec( dllimport )
#	else
#		define _KismoExport __declspec( dllimport )
#		define _DllExport __declspec( dllexport )
#	endif
#endif

// disable: "<type> needs to have dll-interface to be used by clients'
// Happens on STL member variables which are not public therefore is ok
#   pragma warning (disable : 4251)

// disable: "no suitable definition provided for explicit template
// instantiation request" Occurs in VC7 for no justifiable reason on all
// #includes of Singleton
#   pragma warning( disable: 4661)

#endif