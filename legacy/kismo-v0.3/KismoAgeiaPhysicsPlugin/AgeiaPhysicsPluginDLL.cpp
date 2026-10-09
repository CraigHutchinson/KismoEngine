/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\KismoAgeiaPhysicsPlugin\AgeiaPhysicsPluginDLL.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
#include "AgeiaPhysics.h"

#include "Framework/Plugin.h"
#include "Framework/Vector.h"
#include "Core/Core.h"

namespace phys
{
	AgeiaPhysics* physicsPlugin = 0;		
	const frx::Plugin::Info info = 
	{
		{ 0, 1, 0 }, //Version & Build number of plugin
        _T("Craig Hutchinson"), //Author of the plugin
		_T("Kismo Ageia PhysX Physics plugin"), //name/title of the plugin
		_T("Implimentation of Ageia PhysX physics for the Kismo (Realtime Annihlation of Geometry Engine)"), //Detailed description of the plugin
		frx::DateTime(), //Date & time the plugin was created
		frx::Plugin::PluginType_Physics, //Bitmask defining type of plugin e.g. renderer, effect etc..  			
	};

	extern "C"
	{
		const frx::Plugin::Info* dllGetInfo(void)
		{
			return &info;
		}

		Bool dllStartPlugin(void) throw()
		{
			if ( !frx::isValidPointer( physicsPlugin ) )
				physicsPlugin = new AgeiaPhysics();
			
			_Core.physics = physicsPlugin;

			return frx::isValidPointer( physicsPlugin );
		}

		void dllStopPlugin(void)
		{ 
			assert( _Core.physics == physicsPlugin );
			_Core.physics = 0;
			frx::safeDelete( physicsPlugin );
		}
	}


} // END: phys
