/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\Framework\Plugin.cpp
 * Status: superseded by kismo-core/_SOURCE_/KismoFramework/Plugin.cpp (same name, content differs)
 */
#include "Framework/Plugin.h"

namespace frx
{
	Plugin::Plugin( const String& filename ) :
		m_filename( filename ),
		m_info( 0 ),
		m_running( false ),
		m_loaded( false )
	{
	
	}

	//Destruction only from the PluginManager
	Plugin::~Plugin()
	{
		assert( !isLoaded() && !isRunning() );
	}


	void* Plugin::getProcess( const ePluginFunction commonFunc ) const
	{
		switch( commonFunc )
		{
		case PluginFunction_GetInfo:
			return getProcess( _T("dllGetInfo") );
			break;
		case PluginFunction_Start:
			return getProcess( _T("dllStartPlugin") );
			break;
		case PluginFunction_Stop:
			return getProcess( _T("dllStopPlugin") );
			break;
		default:
			assert( _T("Common function ID not valid") == 0 );
			break;
		}
		return 0;
	}


} //END: frx