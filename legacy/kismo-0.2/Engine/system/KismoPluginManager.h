/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\system\KismoPluginManager.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef _KISMO_PLUGINMANAGR_H
#define _KISMO_PLUGINMANAGR_H

#include "framework/KismoPlatform.h"
#include "framework/KismoSingleton.h"
#include "system\KismoPlugin.h"

/*
DLL functions:
----------------------------

Renderer* PluginInstance;
const PluginInfo info = {...};

extern "C" const PluginInfo* dllGetInfo(void) { return &info; }

extern "C" k_Bool dllStartPlugin(void)
{
	create stuff....;
	maybe use Core::getSingleton()...;
	return succeeded?;
}

extern "C" void dllStopPlugin(void) { clear stuff up...; }

*/

namespace kismo
{
	namespace system
	{

		class _KismoExport PluginManager : public frx::Singleton< PluginManager >
		{
		public:

			typedef k_Map< k_String, Plugin* > PluginList;

		public:

			virtual ~PluginManager();

			static PluginManager& getSingleton( void );
			static PluginManager* getSingletonPtr( void );


			//Load a plugin and returns a pointer to it
			// NOTE: If the plugin is alreay loaded the previous plugin pointer will be returned
			Plugin* load( const k_String& filename );

			//Unload the plugin
			void unload( Plugin* plugin );

		protected:

			PluginList m_pluginList; //Keep a list of all plugins loaded


		};

	} //END: system

} //END: kismo

#endif