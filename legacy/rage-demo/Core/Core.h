/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\Core\Core.h
 * Status: duplicate of kismo-v0.3/Core/Core.h
 */
#ifndef CORE_H
#define CORE_H

#include "Platform/Platform.h"
#include "Framework/EventManager.h"
#include "Framework/Singleton.h"
#include "Gfx/MeshManager.h"
#include "Gfx/SceneManager.h"
#include "Io/DirectoryManager.h"

//Forward declarations
//TOOD: Pre-req header
namespace gfx { class Renderer; }
namespace phys { class Physics; }

#define _Core core::Core::getSingleton()
#define _CorePtr core::Core::getSingletonPtr()

/** \brief \namespace core
\brief
	The core namespace contains root program features such as GlobalSettings, 
	StateMachine etc which knit the application together.
*/
namespace core
{
	/** \brief COre holda the main components of the syetm in one easy place
	*/
	class _DllExport Core : public frx::Singleton< Core >
	{
	public:
		SINGLETON( Core );

		Core();

		/** \brief Get the directory manager which holds paths to resources
		*/
		io::DirectoryManager& directoryManager()
		{ return m_directoryManager; }

		/** \brief Gets the meshManager which manges file translators for loading geometry
		*/
		gfx::MeshManager& meshManager()
		{ return m_meshManager; }

		/** \brief Gets the sceneManager which manges file translators for loading scenes
		*/
		gfx::SceneManager& sceneManager()
		{ return m_sceneManager; }

		/** \brief Get the event manager which triggers inter-module actions
		*/
		frx::EventManager& eventManager()
		{ return m_eventManager; }

		//TODO: Proper interface implementation
		phys::Physics* physics;
		gfx::Renderer* renderer;

		Bool initialise()
		{ return true; }

		void terminate()
		{ 
			m_meshManager.removeAll();
			m_sceneManager.removeAll();
		}

	private:
		gfx::MeshManager m_meshManager; ///< Manages loading of meshes
		gfx::SceneManager m_sceneManager; ///< Manages loading of scenes
		io::DirectoryManager m_directoryManager; ///< Directory manager holds paths and enables file lookup
		frx::EventManager m_eventManager; //< The event manager
	};



}


#endif