/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\Win32Platform\Win32ConsoleListener.cpp
 * Status: superseded by kismo-v0.4/KismoAI/Win32ConsoleListener.cpp (same name, content differs)
 */
#include "Win32ConsoleListener.h"



namespace win32
{

	void Win32ConsoleListener::write( const frx::Log::LogMessage* msg, frx::Log* origin )
	{
#ifdef UNICODE //DOS console does not have unicode suport so we have to convert :S
		String8 ts = String( msg->str().c_str() ).asString8();
		std::cout << ts << '\n';
#else
		std::cout << msg->str().c_str() << '\n';
#endif
	}


} //END; win32