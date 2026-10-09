/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\KismoCore\KismoFramework\Platform.h
 * Status: duplicate of kismo-core/_SOURCE_/KismoFramework/Platform.h
 */
#ifndef KISMOFRAMEWORK_PLATFORM_H
#define KISMOFRAMEWORK_PLATFORM_H

#include "KismoFramework/Types.h"

/** All possible platform defines */
#define KISMO_PLATFORM_WINDOWS 1
#define KISMO_PLATFORM_LINUX 2

/** All possible architecture defines */
#define KISMO_ARCHITECTURE_X86 1
#define KISMO_ARCHITECTURE_X64 1

namespace kismo 
{ 
	enum ePlatform
	{
		Platform_Windows = KISMO_PLATFORM_WINDOWS,
		Platform_Linux = KISMO_PLATFORM_LINUX,
		//Platform_PS3,
		//Platform_Wii,
		//Platform_WiiU,
		//Platform_XBox360,
	};

	enum eArchitecture
	{
		Architecture_x86 = KISMO_ARCHITECTURE_X86,
		Architecture_x64 = KISMO_ARCHITECTURE_X64,
		//Architecture_PowerPC???,
		//Architecture_CELL???,
	};

	namespace platform	{} /* END: Platform*/ 

} /*END: Kismo*/


#if defined _WIN64 //< Windows OS
	#define KISMO_PLATFORM KISMO_PLATFORM_WINDOWS
	#define KISMO_ARCHITECTURE KISMO_ARCHITECTURE_X64
#elif defined _WIN32 //< Windows OS
	#define KISMO_PLATFORM KISMO_PLATFORM_WINDOWS
	#define KISMO_ARCHITECTURE KISMO_ARCHITECTURE_X86
#elif __unix // all unices  // Unix
#elif __posix // POSIX
#elif __linux //< Any linux OS
	#define KISMO_PLATFORM KISMO_PLATFORM_LINUX
	#define KISMO_ARCHITECTURE KISMO_ARCHITECTURE_X86
#elif __APPLE__
    #include "TargetConditionals.h"
    #ifdef TARGET_OS_IPHONE
         // iOS
    #elif TARGET_IPHONE_SIMULATOR
        // iOS Simulator
    #elif TARGET_OS_MAC
        // Other kinds of Mac OS
    #else
        // Unsupported platform
    #endif
#endif

#ifndef KISMO_PLATFORM
#error Platform unsupported
#endif

#ifndef KISMO_ARCHITECTURE
#error Architecture unsupported
#endif

#if KISMO_PLATFORM == KISMO_PLATFORM_WINDOWS
	// The following macros define the minimum required win32 platform.
	#ifndef WINVER                          // Specifies that the minimum required platform is Windows Vista.
	#define WINVER 0x0600           // Change this to the appropriate value to target other versions of Windows.
	#endif

	#ifndef _WIN32_WINNT            // Specifies that the minimum required platform is Windows Vista.
	#define _WIN32_WINNT 0x0600     // Change this to the appropriate value to target other versions of Windows.
	#endif
#endif

#endif