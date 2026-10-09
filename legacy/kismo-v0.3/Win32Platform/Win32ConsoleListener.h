/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\Win32Platform\Win32ConsoleListener.h
 * Status: superseded by kismo-v0.4/KismoAI/Win32ConsoleListener.h (same name, content differs)
 */
#ifndef PLATFORM_WIN32_CONSOLE
#define PLATFORM_WIN32_CONSOLE

#include "Framework/Log.h"

namespace win32
{
	/** \brief LogListener writes log emssages to Win32 console
	*/
	class Win32ConsoleListener : public frx::Log::LogListener
	{		
	public:
		virtual void write( const frx::Log::LogMessage* msg, frx::Log* origin );
	};


} //END: win32

#endif