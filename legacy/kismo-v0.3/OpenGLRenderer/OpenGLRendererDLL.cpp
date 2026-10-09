/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\OpenGLRenderer\OpenGLRendererDLL.cpp
 * Status: current (newest copy of this file name in the archive)
 */
#include "OpenGLRenderer.h"

#include "Framework/Plugin.h"
#include "Math/Vector.h"
#include "Core/Core.h"

namespace gfx
{
	OpenGLRenderer* oglRenderPlugin = 0;		
	const frx::Plugin::Info info = 
	{
		{ 0, 1, 0 }, //Version & Build number of plugin
        _T("Craig Hutchinson"), //Author of the plugin
		_T("Kismo OpenGL Renderer"), //name/title of the plugin
		_T("Implimentation of an OpenGL renderer for the Kismo (Realtime Annihlation of Geometric environments)"), //Detailed description of the plugin
		frx::DateTime(), //Date & time the plugin was created
		frx::Plugin::PluginType_GUI, //Bitmask defining type of plugin e.g. renderer, effect etc..  			
	};

	extern "C"
	{
		const frx::Plugin::Info* dllGetInfo(void)
		{
			return &info;
		}

		Bool dllStartPlugin(void) throw()
		{
			if ( !frx::isValidPointer( oglRenderPlugin ) )
				oglRenderPlugin = new OpenGLRenderer();
			
			_Core.renderer = oglRenderPlugin;

			return frx::isValidPointer( oglRenderPlugin );
		}

		void dllStopPlugin(void)
		{ 
			frx::safeDelete( oglRenderPlugin );
		}
	}


} // END: ogl
