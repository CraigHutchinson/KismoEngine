/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\Win32Platform\Win32Plugin.cpp
 * Status: current (newest copy of this file name in the archive)
 */
#include "Win32Plugin.h"

#include "Framework/Log.h"

frx::Plugin* frx::Plugin::create( const Char* filename )
{ return new win32::Win32Plugin( filename ); }

namespace win32
{
	//Construction only from the PluginManager
	Win32Plugin::Win32Plugin( const String& filename ) :
		frx::Plugin( filename ),
		m_hwnd( 0 )
	{
	
	}

	//Destruction only from the PluginManager
	Win32Plugin::~Win32Plugin()
	{
		unload();
	}

	//Load the library
	Bool Win32Plugin::load()
	{	
		m_loaded = ( m_hwnd!= 0 );
		
		if ( m_loaded )
			return true;

		m_running = false;
		
		if ( !m_filename )
		{
			logError( _T("Win32Plugin filename was not specified!") );
			return false;
		}

		m_hwnd = LoadLibrary( m_filename.c_str() );

		m_loaded = frx::isValidHandle( m_hwnd );
		if ( m_loaded )
		{
			//Get information about the plugin
			fDllGetInfo dllGetInfo = (fDllGetInfo)Plugin::getProcess( PluginFunction_GetInfo );        
			if ( dllGetInfo ) //Check the infor function was found
				m_info = dllGetInfo(); //Get a pointer to the information structure
			else
			{
				logMessage( "No plugin 'GetInfo' was found." );
				m_info = 0;
			}

			logSuccess( _T("Win32Plugin \"") << m_filename.c_str() << _T("\" loaded successfully.") /* DEBUG_ONLY( << _T('\n') << *m_info )*/ );
			return true;
		}
		else
		{
			logError( _T("Win32Plugin \"") << m_filename.c_str() << _T("\" failed to load. Detailed error:\n") << getLastError().c_str() );
			return false;
		}

	}			

	//Unload the library
	Bool Win32Plugin::unload()
	{
		m_info = 0;

		if ( isLoaded() )
		{
			stop(); //Stop the plugin

			//We must make sure that any log message locations don't become invalidated  when the plughin is unloaded
			_GlobalLog.pollMessageQueue();

			bool b = (FreeLibrary( m_hwnd ) != 0);
			m_hwnd = 0;
			m_loaded = false;
			return b;
		}

		m_hwnd = 0;
		return true;
	}

	//Run the start process
	Bool Win32Plugin::start()
	{
		if ( !isLoaded() )
		{
			if ( !load() )
				return false;
		}

		if ( !isRunning() )
		{
			m_running = true;


			fDllStartPlugin process = (Plugin::fDllStartPlugin)Plugin::getProcess( Plugin::PluginFunction_Start );
			if ( frx::isValidPointer( process ) )
			{
				bool ret = process();
				if ( ret )
				{
					logSuccess( _T("Win32Plugin \"") << m_filename.c_str() << _T("\" started successfully.") );
				}
				else
				{
					logError( _T("Win32Plugin \"") << m_filename.c_str() << _T("\" returned failure from start().")  );
				}
				return ret;
			}	
			logError( _T("Win32Plugin \"") << m_filename.c_str() << _T("\" has no dllStartPlugin() method!") );

		}
		return false;
	}

	//Run the start process
	Bool Win32Plugin::stop()
	{
		if ( isLoaded() && isRunning() )
		{
			fDllStopPlugin process = (Plugin::fDllStopPlugin)Plugin::getProcess( Plugin::PluginFunction_Stop );
			if ( frx::isValidPointer( process ) )
			{
				process();
			}
			else
			{
				logWarning( _T("Win32Plugin \"") << m_filename.c_str() << _T("\" has no dllStopPlugin() method!") );
			}


			m_running = false;
		}
		return true;
	}

	void* Win32Plugin::getProcess( const String& name ) const
	{
#ifdef UNICODE
		frx::String8 charString = name.asString8();
		return GetProcAddress( m_hwnd, charString.c_str() );
#else
		return GetProcAddress( m_hwnd, name.c_str() );
#endif			
	}


	String Win32Plugin::getLastError() 
	{
		LPVOID lpMsgBuf; 
		FormatMessage( 
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, 
			GetLastError(), 
			MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ), 
			(LPTSTR) &lpMsgBuf, 0, NULL ); 

		String ret( (Char*)lpMsgBuf );

		// Free the buffer.
		LocalFree( lpMsgBuf );
		return ret;
	}

} //END: win32