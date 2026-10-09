/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\Gfx\SceneManager.h
 * Status: current (newest copy of this file name in the archive)
 */
#ifndef GFX_SCENEMANAGER_H
#define GFX_SCENEMANAGER_H

#include "Framework/Singleton.h"
#include "Framework/ResourceManager.h"
#include "Gfx/Scene.h"
#include "Platform/Platform.h"

#define _SceneManager gfx::SceneManager::getSingleton()

namespace gfx
{
	class _DllExport SceneManager : public frx::ResourceManager, public frx::Singleton< SceneManager >
	{
		friend class Scene;
	public:
		SINGLETON( SceneManager );

		/** \brief Create a resource
		\param[in] sceneID The name of the resource
		*/
		Scene::Ptr create( const String& sceneID, Renderer* renderer );
		
		SceneTranslator* getTranslator( const String& filename ) const
		{ return (SceneTranslator*)ResourceManager::getTranslator( filename ); }

	protected:
		
	};

} //END: gfx

#endif