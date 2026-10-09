/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\system\KismoPlugin.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef _KISMO_PLUGIN_H
#define _KISMO_PLUGIN_H

#include "framework/KismoPlatform.h"
#include "framework/KismoDateTime.h"
#include "framework/KismoCommon.h"
#include "fileio\KismoFile.h"


#define PLUGIN_HANDLE pluginHwnd
#define PLUGIN_LOAD( aFilename ) LoadLibrary( aFilename )
#define PLUGIN_GET_PROCESS( aHwnd, bName ) GetProcAddress( aHwnd, bName  )
#define PLUGIN_UNLOAD( a ) FreeLibrary( a ) != 0

//Forward declarations
struct HINSTANCE__;
typedef struct HINSTANCE__* pluginHwnd;

namespace kismo
{
	using frx::Version;
	using frx::DateTime;

	namespace system
	{

		//Forward declaration
		class PluginManager;

		class _KismoExport Plugin
		{
			friend class PluginManager;

		public:

			enum ke_PluginType //Plugin type as a bitmask as a plugin can contain more than one interface
			{
				kc_PluginType_Renderer		= 1 << 0,
				kc_PluginType_GUI			= 1 << 1,
				kc_PluginType_Effect		= 1 << 2,
				kc_PluginType_Addon			= 1 << 14,
				kc_PluginType_Other			= 1 << 15,
			};			

			enum ke_PluginFunction //Enumerator for common plugin functions 
			{
				kc_PluginFunction_GetInfo,
				kc_PluginFunction_Start,
				kc_PluginFunction_Stop,
			};

			struct Info
			{			
				Version version; //Version & Build number of plugin
				k_String author; //Author of the plugin
				k_String title; //name/title of the plugin
				k_String description; //Detailed description of the plugin
				DateTime dateBuilt; //Date & time the plugin was created
				k_UInt16 type; //Bitmask defining type of plugin e.g. renderer, effect etc..   

				friend k_String& operator << (k_String& o, const Info& info )
				{
					o.grow( 128 + info.author.length() + info.title.length() + info.description.length() ); //Reserve some space so we don't have to keep growing the string in small bits
					o << "version: " << info.version
						<< ", author: " << info.author.c_str()
						<< ", title: " << info.title.c_str()
						<< ", description: " << info.description.c_str()
						<< ", dateBuilt: " << info.dateBuilt
						<< ", type: " << info.type;
					return o;
				}
			};
			
			typedef const Plugin::Info* (*kf_dllGetInfo) (void);
			typedef k_Bool (*kf_dllStartPlugin) (void);
			typedef void (*kf_dllStopPlugin) (void);

		protected:

			//Construction only from the PluginManager
			Plugin( fio::File::Ptr file );

			//Destruction only from the PluginManager
			~Plugin();

		public:

			//Get the name of the library
			const Plugin::Info& info() { return *m_info; }
			const k_String& filename() { return m_file->name(); }

			k_Bool isLoaded() { return ( m_hwnd != 0 ); }	
			k_Bool isThreadRunning() { return m_running; }

			//Load the library
			k_Bool load();			

			//Unload the library
			k_Bool unload();

			//Run the start process
			k_Bool start();
		
			//Run the stop process
			k_Bool stop();

			//Get a proces address
			void* getProcess( const k_String& name ) const;
			void* getProcess( const ke_PluginFunction& commonFunc ) const;
		protected:

			k_String getLastError();

		protected:
			
			//Plugin handle
			PLUGIN_HANDLE m_hwnd;

			//Plugin info
			const Plugin::Info* m_info;

			//The filename of the plugin
			fio::File::Ptr m_file; 

			//has the start been called?
			k_Bool m_running;

		};

	} //END: system

} //END: kismo

#endif