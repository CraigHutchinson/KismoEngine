/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\KismoCore\_SOURCE_\KismoFramework\PluginRegistry.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
/*^**************************************************************************
*	
*	Kismo Engine 
*	© 2008-2013 Craig Hutchinson (craig@craighutchinson.co.uk)
*	All right reserved
*
*	This file is subject to the terms and conditions defined in
*	file 'license.txt', which is part of this source code package.
*
***************************************************************************^*/


#include "PluginRegistry.h"
#include "PluginInstance.h"

namespace kismo
{
	IPluginInstance* PluginRegistry::findPluginInstance( const Char* pluginID )
	{
		if ( m_plugins.empty() )
			return 0;

		for ( PluginInstanceLookup::const_iterator iPlugin = m_plugins.begin(); iPlugin != m_plugins.end(); ++iPlugin )
		{
			if ( compare<false>( (*iPlugin)->getID(), pluginID ) == 0 )
				return (*iPlugin);
		}

		return 0;
	}

	bool PluginRegistry::registerPluginInstance( IPluginInstance* instance )
	{
		m_plugins.insert(instance);
		return true;
	}

	bool PluginRegistry::unregisterPluginInstance( IPluginInstance* instance )
	{
		PluginInstanceLookup::iterator iFind = m_plugins.find(instance);
		if ( iFind == m_plugins.end() )
			return false;
		m_plugins.erase(iFind );
		return true;			
	}

} //END: kismo