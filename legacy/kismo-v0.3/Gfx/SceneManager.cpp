/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\Gfx\SceneManager.cpp
 * Status: current (newest copy of this file name in the archive)
 */
#include "SceneManager.h"
#include "IO/DirectoryManager.h"
#include "Gfx/SceneTranslator.h"

IMPL_SINGLETON( gfx::SceneManager )

namespace gfx
{
	
	Scene::Ptr SceneManager::create( const String& sceneID, Renderer* renderer )
	{
		String fullName = _DirectoryManager.getFullPath( sceneID, Scene::TYPE );
		SceneResource* newSceneResource = new SceneResource( this, sceneID, (gfx::SceneTranslator *)getTranslator( fullName ), fullName, renderer );
		
		bool success = false;
		if ( newSceneResource )
		{
			success = newSceneResource->load();	
		}
		
		if ( !success )
			safeDelete( newSceneResource ); //Delete and bail!
		else
			addImpl( newSceneResource );
		
		return (newSceneResource? newSceneResource->getScene() : 0 );
	}
} //END: gfx