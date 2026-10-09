/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoFRX\Plugin.cpp
 * Status: superseded by kismo-core/_SOURCE_/KismoFramework/Plugin.cpp (same name, content differs)
 */
#include <windows.h>
#include <stdio.h>

#include "KismoFRX/Plugin.h"
#include "KismoFRX/Path.h"

namespace kismo
{
	Plugin::Plugin( const String& filename ) :
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
//				assert( rendererCount() == 0 );
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

		char buff[512];
		GetFullPathName( m_filename.c_str(), 512, buff, 0 );
		String pluginDirectory = Path(buff).directory();

		//Set the dll search directory to the path fo the dll
		SetDllDirectory( pluginDirectory.c_str() );

		//Set the default error mode so we get messages for missing dependencies
		SetErrorMode(0);

		// Load DLL file
		m_hinstLib = LoadLibraryEx ( m_filename.c_str(), 0, LOAD_WITH_ALTERED_SEARCH_PATH );

		//Restore the dll search directory to the default
		SetDllDirectory(0);

		//If the load failed we report the error
		if ( !m_hinstLib ) 
		{      
			m_loaded = false;

			//Get the error message
			LPSTR MessageBuffer;
			DWORD dwBufferLength;
			if( dwBufferLength = FormatMessageA( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM , 0, // module to get message from (NULL == system)
				GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // default language 
				(LPSTR) &MessageBuffer, 0, 	NULL  ))
			{
				m_lastError.reserve( 28 + dwBufferLength );
				m_lastError = "ERROR: unable to load DLL";
				m_lastError += MessageBuffer;
				m_lastError += "\n";

				// Free the buffer allocated by the system.
				LocalFree(MessageBuffer);
			}
			else
			{
				m_lastError = "ERROR: unable to load DLL\n";
			}
			return false;
		}

		m_loaded = true;

		//Get the address of plugin functions
		m_fpGetDetails = (FpGetDetails)getProcess( "getDetails" );
		m_fpRegisterPlugin = (FpRegisterPlugin)getProcess( "registerConstructor" );
		m_fpUnregisterPlugin = (FpUnregisterPlugin)getProcess( "unregisterConstructor" );

		assert(m_fpRegisterPlugin);
		if ( m_fpRegisterPlugin )
			return m_fpRegisterPlugin( *this );

		m_lastError = "No Create method was retrieved from the Plugin-DLL";
		return false;
	}

	//Gets details of the plugin, returns false if failed or no plugin loaded
	bool Plugin::getDetails( PluginDetails& info )
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

		void* process = (void*)GetProcAddress( m_hinstLib, processName.c_str() );
		//TODO: Add getLastError windows method to find problems easier

		if ( !process )
		{
			m_lastError = "ERROR: unable to find DLL function '" + processName + "'";
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

		m_lastError = "No UnregisterPlugin method was retrieved from the Plugin-DLL";

		/*if ( rendererCount() != 0 )
		{
			assert( !"All the IRenderers that have been created have not been deleted via the Plugin interface. This may cause fatal errors!" );
			//deleteRenderer();
		}*/

		//remove hanging pointers
		m_fpGetDetails = 0;
		m_fpRegisterPlugin = 0;
		m_fpUnregisterPlugin = 0;

		// Unload DLL file
		return (FreeLibrary( m_hinstLib ) != 0 );
		//TODO: Add getLastError windows method to find problems easier
	}

	//Get the last error message
	const String& Plugin::getLastError()
	{
		return m_lastError;
	}

	bool Plugin::registerConstructor( const String& id, plugin::IConstructor* constructor )
	{ 
		if ( !constructor || id.empty() )
		{
			assert( false );
			return false;
		}

		ConstructorID constructorID = { id, constructor->type };
		ConstructorLookup::_Pairib iInsertion = m_constructors.insert( ConstructorLookup::value_type( constructorID, constructor )  );
		if ( !iInsertion.second ) //, if insertion fails then there is already a constructor with mayching ConstructorID
		{
			assert(false);
			return iInsertion.first->second == constructor; //, Ensure its the same constructor as we alreayd have
		}

		return  true;
	}

	bool Plugin::unregisterConstructor( const String& id, plugin::IConstructor* constructor )
	{ 
		return true;
	}

	//void* Plugin::newPluginInstance( const String& id, plugin::eType type )
	//{
	//	//Check if the plugin is not already loaded

	//	//Find a constructor with the ID that was supplied and the correct type
	//	ConstructorID constructorID = { id, type };
	//	ConstructorLookup::const_iterator iFind = m_constructors.find( constructorID );
	//	if ( iFind == m_constructors.end() )
	//	{
	//		assert( false );
	//		return false;
	//	}

	//	//get the constructor
	//	plugin::IConstructor* constructor= iFind->second;
	//	assert(constructor);

	//	//Create the new renderer instance
	//	void* newInstance= constructor->newInstance();
	//	assert( newInstance );

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
	//		assert( false );
	//		return false;
	//	}

	//	//Call the plugins delete function
	//	assert( iFind->second );
	//	bool ret = iFind->second->deleteInstance( instance );
	//	assert( ret );

	//	m_constructedInstances.erase( iFind );
	//	return true;		
	//}

} //END: kismo