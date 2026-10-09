/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoFRX\Plugin.h
 * Status: superseded by kismo-core/_SOURCE_/KismoFramework/Plugin.h (same name, content differs)
 */
#ifndef KISMOFRX_PLUGIN_H
#define KISMOFRX_PLUGIN_H

//#include "Platform/Platform.h"
#include "KismoFRX/DateTime.h"
#include "KismoFRX/Common.h"
#include "KismoFRX/SmartPointer.h"


namespace kismo
{
	/** \brief Plugin type as a bitmask as a plugin can contain more than one interface
	*/
	enum ePluginType
	{
		PluginType_Renderer			= 1 << 0,
		PluginType_GUI				= 1 << 1,
		PluginType_Effect			= 1 << 2,
		PluginType_MeshTranslator	= 1 << 3,
		PluginType_SceneTranslator	= 1 << 4,
		PluginType_Physics			= 1 << 5,
		PluginType_Addon			= 1 << 14,
		PluginType_Other			= 1 << 15,
	};			

	/** \brief Enumerator for common plugin functions 
	*/
	enum ePluginFunction
	{
		PluginFunction_GetInfo,
		PluginFunction_Start,
		PluginFunction_Stop,
	};

	/** \brief Version number
	*/
	struct Version 
	{ 
		UInt8 major;
		UInt8 minor; 
		UInt16 build;
	};

	/** \brief Structure containing information about the oaded plugin
	*/
	struct Info
	{			
		Version version; ///< Version & Build number of plugin
		String author; ///< Author of the plugin
		String title; ///< name/title of the plugin
		String description; ///< Detailed description of the plugin
		kismo::DateTime dateBuilt; ///< Date & time the plugin was created
		UInt16 type; ///< Bitmask defining type of plugin e.g. renderer, effect etc.. 
	};
		
	/** \brief Wrapper class for Dynamic linked libraries (DLL)
	*/
	class KISMOFRX_EXPORT Plugin
	{
	public:
		
		//Plugin function typedefs
		typedef bool (*FpGetDetails)( PluginDetails& details );
		typedef bool (*FpRegisterPlugin)( Plugin& plugin );
		typedef bool (*FpUnregisterPlugin)( Plugin& plugin );

		/**
			\brief A plugin can construct multiple different objects
		*/
		struct ConstructorID
		{
			String id; //, Id of the plugin constructor
			plugin::eType type; //, type of the plugin
			bool operator < ( const ConstructorID& rhs ) const
			{ return type < rhs.type || ( type == rhs.type && id < rhs.id); }
		};
		typedef std::map< ConstructorID, plugin::IConstructor* > ConstructorLookup;
		typedef std::map< void*, plugin::IConstructor* > ConstructedInstanceLookup;
		typedef ConstructorLookup::iterator iterator;

	public:
		Plugin( const String& filename = String() );
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
		bool getDetails( PluginDetails& info );
	
		//Removes itself from XPDS - calls Destroy
		bool deleteRenderer( IRenderer* );

		//Unloads the plugin from memory
		bool unload(); 

		//Get the last error message
		const String& getLastError();
		void clearLastError();

		//------------ Helper functions ------------
		//Gets a process address of a method inside the DLL
		void* getProcess( const String& processName );


		iterator begin()
		{ return m_constructors.begin(); }
		
		iterator begin( plugin::eType type )
		{
			iterator iPlugin = m_constructors.begin();
			while ( iPlugin != end() && iPlugin->first.type != type )
				++iPlugin;
			return iPlugin;
		}

		iterator next( iterator iPlugin, plugin::eType type )
		{
			++iPlugin;
			while ( iPlugin != end() && iPlugin->first.type != type )
				++iPlugin;
			return iPlugin;
		}

		iterator end()
		{ return m_constructors.end(); }

		//TODO; probably not needed?
		//void* newPluginInstance( const String& id, plugin::eType type );
		//bool deletePluginInstance( void* instance );

		/**
			\brief Register a constructor for a Renderer/FileTranslator/SceneNode
			\remarks Normally called from within a plugin. This registers an implementation it can create
			\par IConstructor contains a 'type()' field for identifying what the constructor creates
			\param[in] id The id is used to identify the type of renderer to construct for a call to newRenderer
		*/
		bool registerConstructor( const String& id, plugin::IConstructor* constructor );			
		bool unregisterConstructor( const String& id, plugin::IConstructor* constructor );

	private:

		String m_filename; //Filename for the plugin		
		bool m_loaded; //Whether the plugin is loaded
		HINSTANCE m_hinstLib; //Handle to the DLL plugin

		ConstructorLookup m_constructors; //, map of all the constructors for tyopes that can eb created
		ConstructedInstanceLookup m_constructedInstances; //, Set of instances and their IConstructor* so they can be unloaded cleanly

		String m_lastError; //Last error message


		//----- Plugin functions ----- //Stored to make runtime faster
		FpGetDetails m_fpGetDetails;
		FpRegisterPlugin m_fpRegisterPlugin;
		FpUnregisterPlugin m_fpUnregisterPlugin;
	};

} //END: kismo

#endif