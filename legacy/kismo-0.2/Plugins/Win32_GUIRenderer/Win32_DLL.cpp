/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Plugins\Win32_GUIRenderer\Win32_DLL.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
#include "Win32_GUIRenderer.h"

#include "system/KismoCore.h"
using namespace kismo::system;


BOOL WINAPI DllMain(
    HINSTANCE hinstDLL,  // handle to DLL module
    DWORD fdwReason,     // reason for calling function
    LPVOID lpReserved )  // reserved
{
    // Perform actions based on the reason for calling.
    switch( fdwReason ) 
    { 
        case DLL_PROCESS_ATTACH:
			//InitCommonControls();

         // Initialize once for each new process.
         // Return FALSE to fail DLL load.
            break;

        case DLL_THREAD_ATTACH:
         // Do thread-specific initialization.
            break;

        case DLL_THREAD_DETACH:
         // Do thread-specific cleanup.
            break;

        case DLL_PROCESS_DETACH:
         // Perform any necessary cleanup.
            break;
    }
    return TRUE;  // Successful DLL_PROCESS_ATTACH.
}

namespace kismo
{
	namespace gui
	{
		GUIRenderer* guiRenderPlugin = 0;		
		const Plugin::Info info = 
		{
			{ 0, 1, 0 }, //Version & Build number of plugin
            _T("Craig Hutchinson"), //Author of the plugin
			_T("Kismo Win32 GUI"), //name/title of the plugin
			_T("Win32 user interface rendering"), //Detailed description of the plugin
			DateTime(), //Date & time the plugin was created
			Plugin::kc_PluginType_GUI, //Bitmask defining type of plugin e.g. renderer, effect etc..  			
		};

		extern "C" const Plugin::Info* dllGetInfo(void)
		{
			return &info;
		}

		extern "C" k_Bool dllStartPlugin(void) throw()
		{
			//

			if ( !isValidPointer( guiRenderPlugin ) )
				guiRenderPlugin = new Win32_GUIRenderer();
			
			return isValidPointer( guiRenderPlugin );
		}

		extern "C" void dllStopPlugin(void)
		{ 
			if ( _Gui.isThreadRunning() )
				_Gui.sendMessage( Thread::wait( new GUIMsg_RemoveRenderer( guiRenderPlugin ) ) ); //Post a to destroy & remove this renderer
			
			safeDelete( guiRenderPlugin );
		}


	} // END: gui

} // END: kismo
