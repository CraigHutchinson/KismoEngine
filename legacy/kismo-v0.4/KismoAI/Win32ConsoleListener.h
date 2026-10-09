/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoAI\Win32ConsoleListener.h
 * Status: current (newest copy of this file name in the archive)
 */
/** \author Craig Hutchinson \file 
	\brief 
*/
#ifndef PLATFORM_WIN32_CONSOLE
#define PLATFORM_WIN32_CONSOLE

#include "Log.h"

namespace win32
{
	/** \brief LogListener writes log emssages to Win32 console
	*/
	class Win32ConsoleListener : public frx::Log::LogListener
	{		
	public:
		Win32ConsoleListener();
		virtual void write( const frx::Log::LogMessage& msg, frx::Log* origin );
	};


} //END: win32

#endif