/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\system\KismoCore.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
#include "kismoCore.h"

namespace kismo
{	
	template<> Core* Singleton< Core >::ms_instance = 0;

	Core& Core::getSingleton( void )
	{	
		assert( isValidPointer( ms_instance ) );  
		return ( *ms_instance );
	}

	Core* Core::getSingletonPtr( void )
	{	 
		return ms_instance; 
	}

	Core::Core( frx::Log& log ) : 
		m_log( log )
	{
	}

	Core::~Core()
	{
		m_gui.stop();
	}

	//Load a plugin
	system::Plugin* Core::loadPlugin( const k_String& filename, bool start )
	{
		system::Plugin* plugin = m_pluginManager.load( filename );
		
		if ( !isValidPointer( plugin ) )
			return 0;

		if ( plugin->load() && (!start || plugin->start()) )
				return plugin;

		m_pluginManager.unload( plugin );
		return 0;
	}

	//Unload a plugin
	bool Core::unloadPlugin( system::Plugin* plugin )
	{
		if ( !isValidPointer( plugin ) )
			return false;

		bool b = plugin->stop();

		m_pluginManager.unload( plugin );

		return b;
	}		
}
