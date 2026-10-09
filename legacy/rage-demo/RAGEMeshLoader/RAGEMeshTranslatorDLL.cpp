/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\RAGEMeshLoader\RAGEMeshTranslatorDLL.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
#include "RAGEMeshTranslator.h"
#include "RAGESceneTranslator.h"

#include "Framework/Plugin.h"
#include "Core/Core.h"

namespace rage
{
	RAGEMeshTranslator meshTranslatorInstance;	
	RAGESceneTranslator sceneTranslatorInstance;		
	const frx::Plugin::Info info = 
	{
		{ 0, 1, 0 }, //Version & Build number of plugin
        _T("Craig Hutchinson"), //Author of the plugin
		_T("RAGE Mesh/Scene translator"), //name/title of the plugin
		_T("RAGE mesh & scene format loader. Write method not implemented yet."), //Detailed description of the plugin
		frx::DateTime(), //Date & time the plugin was created
		frx::Plugin::PluginType_MeshTranslator
		| frx::Plugin::PluginType_SceneTranslator, //Bitmask defining type of plugin e.g. renderer, effect etc..  			
	};

	extern "C"
	{
		const frx::Plugin::Info* dllGetInfo(void)
		{
			return &info;
		}

		Bool dllStartPlugin(void) throw()
		{			
			//Register the instances
			_Core.meshManager().registerTranslator( &meshTranslatorInstance );
			_Core.sceneManager().registerTranslator( &sceneTranslatorInstance );

			return true;
		}

		void dllStopPlugin(void)
		{ 
			//Remove the instances
			_Core.meshManager().removeTranslator( &meshTranslatorInstance );
			_Core.sceneManager().removeTranslator( &sceneTranslatorInstance );
		}
	}


} // END: rage
