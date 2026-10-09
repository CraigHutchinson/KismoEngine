/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\Win32Platform\Platform\Platform.cpp
 * Status: duplicate of kismo-v0.3/Win32Platform/Platform/Platform.cpp
 */
#include "Platform.h"
#include "Framework/Common.h"

#include "Win32Window.h"
#include "Win32Plugin.h"
#include "Win32Mutex.h"
#include "Win32Thread.h"
#include "Win32ConsoleListener.h"

#include <windows.h>

win32::Win32ConsoleListener lSonsoleListener; ///< Log listerner for putting output to the console

namespace platform
{


	void outputDebugString( const Char* str )
	{	
		OutputDebugString( str );
	}
	
	Bool fileExists( const Char* filename )
	{
		FILE* pFile = 0;
#ifdef WIN32
		_tfopen_s( &pFile, filename, _T("r") );
#else
		pFile = _tfopen( (const char*)filename, _T("r") );
#endif

		if ( pFile )
		{
			fclose( pFile );
			return true;
		}
		else		 
			return false;
	}

	void sleep( Count milliseconds )
	{
		Sleep( milliseconds );
	}


	//Function to get the handle of the win32 console
	//http://support.microsoft.com/default.aspx?scid=kb;en-us;124103
	HWND GetConsoleHwnd(void)
	{
	   const Count bufferSize = 1024; // Buffer size for console window titles.
	   HWND hwndFound;         // This is what is returned to the caller.
	   Char pszNewWindowTitle[bufferSize]; // Contains fabricated
										   // WindowTitle.
	   Char pszOldWindowTitle[bufferSize]; // Contains original
										   // WindowTitle.

	   // Fetch current window title.
	   GetConsoleTitle( pszOldWindowTitle, bufferSize );

	   // Format a "unique" NewWindowTitle.
	   _stprintf_s( pszNewWindowTitle,_T("%d/%d"),
				   GetTickCount(),
				   GetCurrentProcessId());

	   // Change current window title.
	   SetConsoleTitle(pszNewWindowTitle);

	   // Ensure window title has been updated.
	   Sleep(40);

	   // Look for NewWindowTitle.
	   hwndFound=FindWindow(NULL, pszNewWindowTitle);

	   // Restore original window title.
	   SetConsoleTitle(pszOldWindowTitle);

	   return(hwndFound);
   }

	_DllExport void initialise()
	{
		HWND hwndConsole = GetConsoleHwnd();

		HMENU hMenu = GetSystemMenu( hwndConsole, FALSE );
		DeleteMenu( hMenu, 6, MF_BYPOSITION);

		lSonsoleListener.listenToLog( _GlobalLogPtr );
	}

}
