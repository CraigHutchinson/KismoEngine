/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\system\KismoPlugin.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
#include "KismoPlugin.h"

#include "KismoPluginManager.h"

#include "fileIO/KismoFileIO.h"
#include "framework/KismoLog.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace kismo
{
	using fio::FileIO;
	using fio::File;
	using fio::Directory;

	namespace system
	{
		//Construction only from the PluginManager
		Plugin::Plugin( fio::File::Ptr file ) :
			m_hwnd( 0 ),
			m_file( file ),
			m_info( 0 ),
			m_running( false )
		{
		
		}

		//Destruction only from the PluginManager
		Plugin::~Plugin()
		{
			unload();

			safeDelete( m_info );
		}

		//Load the library
		k_Bool Plugin::load()
		{	
			if ( m_hwnd )
				return true;

			m_running = false;
			
			if ( !m_file )
			{
				LOG_ERROR( _T("Plugin \"") + m_file->name() + _T("\" could not be located in any plugin directories!") );
				return false;
			}

			m_hwnd = PLUGIN_LOAD( m_file->fullPath().c_str() );

			if ( isValidHandle( m_hwnd ) )
			{
				//Get information about the plugin
				kf_dllGetInfo dllGetInfo = (kf_dllGetInfo)getProcess( kc_PluginFunction_GetInfo );        
				if ( dllGetInfo ) //Check the infor function was found
					m_info = dllGetInfo(); //Get a pointer to the information structure
				else
					m_info = 0;

				LOG_SUCCESS( _T("Plugin \"") << m_file->fullPath() << _T("\" loaded successfully." DEBUG_ONLY( << _T('\n') << *m_info )) );
				return true;
			}
			else
			{
				m_hwnd = 0; //Set to invalid
				LOG_ERROR( _T("Plugin \"") + m_file->fullPath() + _T("\" failed to load. Detailed error:\n") + getLastError() );
				return false;
			}

		}			

		//Unload the library
		k_Bool Plugin::unload()
		{
			m_info = 0;


			if ( isLoaded() )
			{
				stop(); //Stop the plugin

				bool b = PLUGIN_UNLOAD( m_hwnd );
				m_hwnd = 0;
				return b;
			}

			m_hwnd = 0;
			return true;
		}

		//Run the start process
		k_Bool Plugin::start()
		{
			if ( isLoaded() && !isThreadRunning() )
			{
				m_running = true;

				kf_dllStartPlugin process = (Plugin::kf_dllStartPlugin)getProcess( Plugin::kc_PluginFunction_Start );
				if ( isValidPointer( process ) )
					return process();
			}
			return false;
		}

		//Run the start process
		k_Bool Plugin::stop()
		{
			if ( isLoaded() && isThreadRunning() )
			{
				kf_dllStopPlugin process = (Plugin::kf_dllStopPlugin)getProcess( Plugin::kc_PluginFunction_Stop );
				if ( isValidPointer( process ) )
					process();

				m_running = false;
			}
			return true;
		}
		//Get a proces address
		void* Plugin::getProcess( const k_String& name ) const
		{
#ifdef UNICODE
			char buffer[256];
			WideCharToMultiByte(CP_ACP, 0, name.c_str(), -1, buffer, 256, NULL, NULL);
			buffer[ name.length() ] = 0;
			return PLUGIN_GET_PROCESS( m_hwnd, buffer );
#else
			return PLUGIN_GET_PROCESS( m_hwnd, name.c_str() );
#endif			
		}

		void* Plugin::getProcess( const ke_PluginFunction& commonFunc ) const
		{
			switch( commonFunc )
			{
			case kc_PluginFunction_GetInfo:
				return getProcess( _T("dllGetInfo") );
				break;
			case kc_PluginFunction_Start:
				return getProcess( _T("dllStartPlugin") );
				break;
			case kc_PluginFunction_Stop:
				return getProcess( _T("dllStopPlugin") );
				break;
			default:
				assert( _T("Common function ID not valid") == 0 );
				break;
			}
			return 0;
		}

		k_String Plugin::getLastError() 
		{
			LPVOID lpMsgBuf; 
			FormatMessage( 
				FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, 
				GetLastError(), 
				MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ), 
				(LPTSTR) &lpMsgBuf, 0, NULL ); 

			k_String ret;//( (k_Char)lpMsgBuf );

			// Free the buffer.
			LocalFree( lpMsgBuf );
			return ret;
		}
	}
}