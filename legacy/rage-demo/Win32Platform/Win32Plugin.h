/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\Win32Platform\Win32Plugin.h
 * Status: duplicate of kismo-v0.3/Win32Platform/Win32Plugin.h
 */
#ifndef WIN32_PLUGIN_H
#define WIN32_PLUGIN_H

#include "Framework/Plugin.h"

#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <windows.h>

namespace win32
{
	/** \brief Win32 implementation of frx::Plugin
	*/
	class Win32Plugin : public frx::Plugin
	{
	public:

		/** \brief \copydoc core::Plugin::Plugin
		*/
		Win32Plugin( const String& filename );

		/* \copydoc core::Plugin::~Plugin
		*/
		virtual ~Win32Plugin();

		/** \brief \copydoc core::Plugin::load
		*/
		virtual Bool load();			

		/** \brief \copydoc core::Plugin::unload
		*/
		virtual Bool unload();

		/** \brief \copydoc core::Plugin::start
		*/
		virtual Bool start();
	
		/** \brief \copydoc core::Plugin::stop
		*/
		virtual Bool stop();

		/** \brief \copydoc core::Plugin::getProcess
		*/
		virtual void* getProcess( const String& name ) const;

	protected:

		String getLastError();

	protected:
		
		HMODULE m_hwnd; ///< Handle to the plugin/Dll module

	};

} //END: win32

#endif