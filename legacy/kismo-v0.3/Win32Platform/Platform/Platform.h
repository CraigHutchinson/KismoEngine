/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\Win32Platform\Platform\Platform.h
 * Status: superseded by kismo-core/_SOURCE_/KismoFramework/Platform.h (same name, content differs)
 */
#ifndef PLATFORM_H
#define PLATFORM_H

#include "Framework/BaseTypes.h"
//#include "Math/Rect.h"

#if defined STATIC_BUILD
#	define _DllExport
#	define _DllImport
#else
#	if defined( NONCLIENT_BUILD )
#		define _DllExport __declspec( dllexport )
#		define _DllImport __declspec( dllimport )
#	else
#		define _DllExport __declspec( dllimport )
#		define _DllImport __declspec( dllexport )
#	endif
#endif

// disable: "<type> needs to have dll-interface to be used by clients'
// Happens on STL member variables which are not public therefore is ok
#   pragma warning (disable : 4251)

// disable: "no suitable definition provided for explicit template
// instantiation request" Occurs in VC.net for no justifiable reason on all
// #includes of Template class with static member (eg. Singleton)
#   pragma warning( disable: 4661)

#if defined(_M_X64) || defined(__amd64__)
#	define X64
#endif

namespace frx //forward declarations
{
	class Mutex;
	class Thread;
	class Window;
	class Plugin;
}

/** \brief Platform namespace contains any platform specific functions and classes
*/
namespace platform
{
	_DllExport void outputDebugString( const Char* str );
	_DllExport Bool fileExists( const Char* str );
	_DllExport void sleep( Count milliseconds );

	/** \brief Special initialise to disable the Dos Console close button
	*/
	_DllExport void initialise();
};

#endif