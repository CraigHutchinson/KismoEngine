/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\KismoCore\_SOURCE_\KismoFramework\Plugin.h
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

#ifndef KISMOFRAMEWORK_PLUGIN_H
#define KISMOFRAMEWORK_PLUGIN_H

//#include "Platform/Platform.h"
#include "KismoFramework/DateTime.h"
#include "KismoFramework\Common.h"
#include "KismoFramework/SmartPointer.h"


namespace kismo
{
	/** \brief Version number
	*/
	struct Version 
	{ 
		UInt8 major;
		UInt8 minor; 
		UInt16 build;
	};

	/** \brief Structure containing information about the loaded plugin
	*/
	struct PluginInfo
	{			
		Version version; ///< Version & Build number of plugin
		String author; ///< Author of the plugin
		String title; ///< name/title of the plugin
		String description; ///< Detailed description of the plugin
		kismo::DateTime dateBuilt; ///< Date & time the plugin was created
		UInt16 type; ///< Bitmask defining type of plugin e.g. renderer, effect etc.. 
	};
		
	/** \brief		Wrapper class for Dynamic linked libraries (DLL)
		\remarks	Handles run-time dynamic linkinf of DLLs. IPluginInstance instantiations should be automatically
					created by the plugins in DLLmain() in normal use but exported functiosn are also accessible
					through this interface to obtain plugin information and/or support on demand registration of plugin
					functionality
	*/
	class Plugin
	{
	public:
		
		//Plugin function typedefs
		typedef bool (*FpGetDetails)( PluginInfo& details );
		typedef bool (*FpRegisterPlugin)( Plugin& plugin );
		typedef bool (*FpUnregisterPlugin)( Plugin& plugin );

	public:
		Plugin( Log* log = 0, const String& filename = String() );
		virtual ~Plugin();

		//---- Initialisation -----
		
		bool load( const String& filename )
		{
			if ( m_filename != filename )
				 unload();
			m_filename = filename; 
			return load();
		}

		bool load();//Loads a dll plugin file - Will return false if load fails or if the plugin is already loaded!

		//Return whether the plugin is loaded
		bool isLoaded() { return m_loaded; }

		//Gets details of the plugin, returns false if failed or no plugin loaded - calls GetDetails
		bool getDetails( PluginInfo& info );
	
		//Unloads the plugin from memory
		bool unload(); 

		//Get the last error message
		const String& getLastError();
		void clearLastError();

		//------------ Helper functions ------------
		/**
			\brief		Gets a process address of a method inside the DLL
			\returns	Pointer to the DLL function. It is down the the caller to know the functions correct definition
						and usage
		*/
		void* getProcess( const String& processName );

	private:

		String m_filename; ///< Filename for the plugin		
		bool m_loaded; //< Whether the plugin is loaded
		int/*HINSTANCE*/ m_hinstLib; //< Handle to the DLL plugin

		String m_lastError; //Last error message

		//----- Plugin functions ----- //Stored to make runtime faster
		FpGetDetails m_fpGetDetails;
		FpRegisterPlugin m_fpRegisterPlugin;
		FpUnregisterPlugin m_fpUnregisterPlugin;

		Log* m_log;
	};

} //END: kismo

#endif