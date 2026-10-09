/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\Framework\ss.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef FRX_PLUGIN_H
#define FRX_PLUGIN_H

#include "Platform/Platform.h"
#include "Framework/DateTime.h"
#include "Framework/Common.h"
#include "Framework/SmartPointer.h"


namespace frx
{
	/** \brief Wrapper class for Dynamic linked libraries (DLL)
	*/
	class _DllExport Plugin : public frx::SmartReferenced
	{
	public:

		static Plugin* create( const Char* filename );

		/** \brief Plugin pointer for automatic lifetie managment
		*/
		typedef frx::SmartPointer< Plugin > Ptr;

	public:

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
			frx::DateTime dateBuilt; ///< Date & time the plugin was created
			UInt16 type; ///< Bitmask defining type of plugin e.g. renderer, effect etc.. 
		};
		
		typedef const Plugin::Info* (*fDllGetInfo) (void);
		typedef Bool (*fDllStartPlugin) (void);
		typedef void (*fDllStopPlugin) (void);

	public:
	
		/** \brief Constructor takes the filename of a dynamic link library to load
			\param[in] filename The relative or full path to the plugin
		*/
		Plugin( const String& filename );

		/* virtual destructor
		*/
		virtual ~Plugin();

		/** \brief Get information about the library
		*/
		const Plugin::Info& info() const
		{ return *m_info; }

		/** \brief get the filename for the plugin
		*/
		const String& filename() const
		{ return m_filename; }

		/** \brief Returns whether the plugin is loaded
		*/
		Bool isLoaded() const
		{ return m_loaded; }

		/** \brief Returns whether the plugin is started or stopped
		*/
		Bool isRunning() const
		{ return m_running; }

		/** \brief Load the library
		*/
		virtual Bool load() = 0;			

		/** \brief Unload the library
		*/
		virtual Bool unload() = 0;

		/** \brief Run the start process
		*/
		virtual Bool start() = 0;
	
		/** \brief Run the stop process
		*/
		virtual Bool stop() = 0;

		/** \brief Get a proces address by name
		*/
		virtual void* getProcess( const String& name ) const = 0;

		/** \brief Get a proces address by its common function definition
		*/
		void* getProcess( const ePluginFunction commonFunc ) const;

	protected:
				
		const Plugin::Info* m_info; ///< Plugin info
		String m_filename; ///< The filename of the plugin
		Bool m_running; ///< has the start been called?
		Bool m_loaded; ///< has the plugin loaded okay?

	};

} //END: frx

#endif