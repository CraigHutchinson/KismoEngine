/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\system\KismoCore.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef _KISMO_CORE_H
#define _KISMO_CORE_H

#include "framework/KismoPlatform.h"
#include "framework/KismoSingleton.h"
#include "framework/KismoCommon.h"
#include "framework/KismoTimer.h"
#include "framework/KismoLog.h"

#include "system/KismoPluginManager.h"

#include "fileIO/KismoFileIO.h"

#include "gui/KismoGUI.h"
#include "gui/KismoConsole.h"

#include "gfx/KismoGfx.h"
#include "sound/KismoSound.h"


#define _Core kismo::Core::getSingleton()

namespace kismo
{
	class _KismoExport Core : public Singleton< Core >
	{
	public:
		Core( frx::Log& log );
		~Core();

		static Core& getSingleton( void );
		static Core* getSingletonPtr( void );

		/** \brief Get a reference to the plugin manager */
		system::PluginManager& pluginManager() { return m_pluginManager; }	
		/** \brief Load a plugin
		\param filename The filename for the plugin excluding the file extension
		\param start Whether the plugin should be started if it loads successfully
		\return A pointer to the plugin. If load (or Start) fail then the pointer will be 0 */
		system::Plugin* loadPlugin( const k_String& filename, bool start = true );
		/** \brief Get a reference to the GUI core */
		bool unloadPlugin( system::Plugin* plugin );

		/** \brief Get a reference to the GUI core */
		gui::GUI& gui() { return m_gui; }
		/** \brief Get a reference to the graphics core */
		gfx::Gfx& gfx() { return m_gfx; }		
		/** \brief Get a reference to the sound core */
		sound::Sound& sound() { return m_sound; }			
		/** \brief Get a reference to the file manager */
		fio::FileIO& fileManager() { return m_fileIO; }
		/** \brief Get a reference to the console */
		gui::Console& console() { return m_console; }
		/** \brief Get a reference to the console */
		const frx::SystemTimer& systemTimer() const { return m_systemTimer; }
	protected:
		
		gui::Console m_console; //Console 

		frx::Log& m_log; //Log reference

		
		/** \brief The system timer instance */
		const frx::SystemTimer m_systemTimer;
		/** \brief The GUI core */
		gui::GUI m_gui;
		/** \brief The graphics core */
		gfx::Gfx m_gfx;
		/** \brief The sound core */
		sound::Sound m_sound;
		
		/** \brief Plugin manager instance */
		system::PluginManager m_pluginManager;

		/** \brief File manager instance */
		fio::FileIO m_fileIO;

	};

} //END: kismo

#endif