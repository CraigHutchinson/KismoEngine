/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\KismoCore\_SOURCE_\KismoFramework\Plugin.cpp
 * Status: current (newest copy of this file name in the archive)
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



#include <windows.h>
#include <stdio.h>

#include "KismoFramework/Plugin.h"
#include "KismoFramework/Path.h"
#include "KismoFramework/Log.h"

namespace kismo
{
	Plugin::Plugin( Log* log, const String& filename ) :
		m_log(log),
		m_loaded( false ),
		m_filename( filename ),
		m_fpGetDetails( 0 ),
		m_fpRegisterPlugin( 0 ),
		m_fpUnregisterPlugin( 0 )
	{

	}

	Plugin::~Plugin()
	{
		if ( isLoaded() )
		{
//				Assert( rendererCount() == 0 );
				//deleteRenderer();

			unload();
		}
	}

	
	void Plugin::clearLastError() 
	{ m_lastError = _T(""); } //clear the last error

	//Loads a dll plugin file
	//Will return false if load fails or if the plugin is already loaded!
	bool Plugin::load()
	{
		if ( isLoaded() )
			return true;

		Char buff[512];
		GetFullPathName( m_filename.c_str(), 512, buff, 0 );
		String pluginDirectory = Path(buff).directory();

		//Set the dll search directory to the path fo the dll
		//SetDllDirectory( pluginDirectory.c_str() );

		//Set the default error mode so we get messages for missing dependencies
		SetErrorMode(0);

		// Load DLL file
		m_hinstLib = (int)LoadLibraryEx ( m_filename.c_str(), 0, LOAD_WITH_ALTERED_SEARCH_PATH );

		//Restore the dll search directory to the default
		//SetDllDirectory(0);

		//If the load failed we report the error
		if ( !m_hinstLib ) 
		{      
			m_loaded = false;

			//Get the error message
			Char* messageBuffer = 0;
			DWORD dwBufferLength = 0;
			if( dwBufferLength = FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM , 0, // module to get message from (NULL == system)
				GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // default language 
				messageBuffer, 0, 	NULL  ))
			{
				m_lastError.reserve( Size16(28 + dwBufferLength) );
				m_lastError = _T("ERROR: unable to load DLL");
				m_lastError += messageBuffer;
				m_lastError += _T('\n');

				if ( m_log ) logError( m_log, m_lastError );
			
				// Free the buffer allocated by the system.
				LocalFree(messageBuffer);
			}
			else
			{
				m_lastError = _T("ERROR: unable to load DLL");
			}
			return false;
		}

		m_loaded = true;

		//Get the address of plugin functions
		m_fpGetDetails = (FpGetDetails)getProcess( _T("getDetails") );
		m_fpRegisterPlugin = (FpRegisterPlugin)getProcess( _T("registerConstructor") );
		m_fpUnregisterPlugin = (FpUnregisterPlugin)getProcess( _T("unregisterConstructor") );

		m_lastError = _T("No Create method was retrieved from the Plugin-DLL");
		return m_loaded;
	}

	//Gets details of the plugin, returns false if failed or no plugin loaded
	bool Plugin::getDetails( PluginInfo& info )
	{
		if ( m_fpGetDetails )
		{
			return m_fpGetDetails( info );
		}

		return false;
	}


	//Gets a process address of a method inside the DLL
	void* Plugin::getProcess( const String& processName )
	{
		if ( !isLoaded() )
			return 0;
		
		void* process = (void*)GetProcAddress( (HMODULE)m_hinstLib, ConvertString<String,String8>( processName ).str().c_str() );
		//TODO: Add getLastError windows method to find problems easier

		if ( !process )
		{
			//m_lastError = _T("ERROR: unable to find DLL function '") + processName + _T('\'');
		}

		return process;
	}


	//Unloads the plugin from memory
	bool Plugin::unload()
	{
		if ( !isLoaded() )
			return false;

		if ( m_fpUnregisterPlugin )
			return m_fpUnregisterPlugin( *this );

		m_lastError = _T("No UnregisterPlugin method was retrieved from the Plugin-DLL");

		/*if ( rendererCount() != 0 )
		{
			Assert( !"All the IRenderers that have been created have not been deleted via the Plugin interface. This may cause fatal errors!" );
			//deleteRenderer();
		}*/

		//remove hanging pointers
		m_fpGetDetails = 0;
		m_fpRegisterPlugin = 0;
		m_fpUnregisterPlugin = 0;

		// Unload DLL file
		return (FreeLibrary( (HMODULE)m_hinstLib ) != 0 );
		//TODO: Add getLastError windows method to find problems easier
	}

	//Get the last error message
	const String& Plugin::getLastError()
	{
		return m_lastError;
	}


	//void* Plugin::newPluginInstance( const String& id, plugin::eType type )
	//{
	//	//Check if the plugin is not already loaded

	//	//Find a constructor with the ID that was supplied and the correct type
	//	ConstructorID constructorID = { id, type };
	//	ConstructorLookup::const_iterator iFind = m_constructors.find( constructorID );
	//	if ( iFind == m_constructors.end() )
	//	{
	//		Assert( false );
	//		return false;
	//	}

	//	//get the constructor
	//	plugin::IConstructor* constructor= iFind->second;
	//	Assert(constructor);

	//	//Create the new renderer instance
	//	void* newInstance= constructor->newInstance();
	//	Assert( newInstance );

	//	//Store a lookup for the instances constructor so we can delete it when we shut down
	//	m_constructedInstances[newInstance] = constructor;

	//	return newInstance;
	//}
	//
	//bool Plugin::deletePluginInstance( void* instance )
	//{
	//	ConstructedInstanceLookup::iterator iFind = m_constructedInstances.find( instance );

	//	if ( iFind == m_constructedInstances.end() )
	//	{
	//		//TODO: error
	//		Assert( false );
	//		return false;
	//	}

	//	//Call the plugins delete function
	//	Assert( iFind->second );
	//	bool ret = iFind->second->deleteInstance( instance );
	//	Assert( ret );

	//	m_constructedInstances.erase( iFind );
	//	return true;		
	//}

} //END: kismo