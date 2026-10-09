/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoPlugin_OpenGLRenderer\dllmain.cpp
 * Status: superseded by kismo-v0.4/KismoGFX/dllmain.cpp (same name, content differs)
 */
#include "KismoPlatform/Common.h"
#include <windows.h>

// Defines the entry point for the DLL application.
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

