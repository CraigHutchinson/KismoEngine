/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Plugins\OGL_Renderer\OGL_DLL.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
#include "OGL_Renderer.h"

#include "system\KismoCore.h"
using namespace kismo::system;

namespace kismo
{
	namespace gfx
	{
		Renderer* renderPlugin = 0;		
		const Plugin::Info info = 
		{
			{ 0, 1, 0 }, //Version & Build number of plugin
            _T("Craig Hutchinson"), //Author of the plugin
			_T("Kismo OpenGL Renderer"), //name/title of the plugin
			_T("3D/2D OpenGL renderer for Kismo"), //Detailed description of the plugin
			DateTime(), //Date & time the plugin was created
			Plugin::kc_PluginType_Renderer, //Bitmask defining type of plugin e.g. renderer, effect etc..  			
		};

		extern "C" const Plugin::Info* dllGetInfo(void)
		{
			return &info;
		}

		extern "C" k_Bool dllStartPlugin(void) throw()
		{
			if ( !isValidPointer( renderPlugin ) )
				renderPlugin = new OGL_Renderer();
			
			return isValidPointer( renderPlugin );
		}

		extern "C" void dllStopPlugin(void)
		{
			
			//if ( _Gfx.isThreadRunning() )
			///	_Gfx.sendMessage( Thread::wait( new GfxMsg_RemoveRenderer( guiRenderPlugin ) ) ); //Post a to destroy & remove this renderer
			
			safeDelete( renderPlugin );
		}


	} // END: gfx

} // END: kismo
