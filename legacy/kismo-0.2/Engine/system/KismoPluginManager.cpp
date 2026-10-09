/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\system\KismoPluginManager.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
#include "KismoPluginManager.h"
#include "fileio\KismoFile.h"
#include "fileIO/KismoFileIO.h"


using namespace kismo::system;

namespace kismo
{
	namespace frx
	{
		template<> system::PluginManager* Singleton< system::PluginManager >::ms_instance = 0;
	}
		
	namespace system
	{
		PluginManager& PluginManager::getSingleton( void ) { assert( isValidPointer( ms_instance ) ); return ( *ms_instance ); }
		PluginManager* PluginManager::getSingletonPtr( void ) { return ms_instance;  }

		PluginManager::~PluginManager()
		{
			for ( PluginList::iterator iter = m_pluginList.begin(); iter != m_pluginList.end(); ++iter )
				delete iter->second;
		}


		Plugin* PluginManager::load( const k_String& filename )
		{
			PluginList::const_iterator iter = m_pluginList.find( filename );

			//First check that the plugin is not already loaded...
			if ( iter != m_pluginList.end() )
				return iter->second;

			//Create the new plugin instance
			fio::File::Ptr pluginFile( fio::FileIO::getSingleton().findPlugin( filename ) );
			Plugin* newPlugin = new Plugin( pluginFile );

			//Insert the new plugin into the plugin map
			m_pluginList[ filename ] = newPlugin;

			return newPlugin;
		}

		void PluginManager::unload( Plugin* plugin )
		{
			assert( isValidPointer( plugin ) );
			
			if ( !isValidPointer( plugin ) ) 
				return;

			//Check if the plugin exists... as it should!
			//PluginList::iterator iter = m_pluginList.find( plugin->filename() );

			//assert( iter != m_pluginList.end() );

			//Remove the plugin from the plugin list
			//if ( iter != m_pluginList.end() )
			//    m_pluginList.erase( iter );

			//Unload the plugin elegantly
			plugin->unload(); 
			
			//Free the memory
			//delete plugin; 
		}
	}
}