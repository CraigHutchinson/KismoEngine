/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\Phys\Physics.cpp
 * Status: duplicate of kismo-v0.3/Phys/Physics.cpp
 */
#include "Physics.h"
#include "gfx/Spatial.h"
#include "Framework/Log.h"
#include "Phys/Scene.h"

IMPL_SINGLETON( phys::Physics )

namespace phys
{

	Physics::Physics( const String& name ) :
		frx::Named( name ),
		m_paused( false ),
		m_wireframe( false )
	{
	}

	Physics::~Physics()
	{
	}

	bool Physics::initialise()
	{
		return true;
	}


	Scene::Ptr Physics::createScene( const String& name, const gfx::Scene::Ptr& gfxScene )
	{
		SceneMap::const_iterator iFind = m_sceneGfxLookup.find( gfxScene );
		if ( iFind != m_sceneGfxLookup.end() )
		{
			logError( _T("A physics scene has already been added to the scene '") << ((gfx::Spatial*)gfxScene)->getName() << _T("'") );
			return 0;
		}

		Scene::Ptr scene = createSceneImpl( name );
		m_scenes.insert( scene );
		m_sceneGfxLookup[ gfxScene ] = scene;
		return scene;
	}

	void Physics::releaseScene( Scene* scene )
	{
		SceneLookup::iterator iScene = m_scenes.find( scene );
		if ( iScene != m_scenes.end() )
		{
			m_scenes.erase( iScene );
		}

		for ( SceneMap::iterator iScene = m_sceneGfxLookup.begin(); iScene != m_sceneGfxLookup.end(); ++iScene )
		{
			if ( iScene->second = scene )
			{
				m_sceneGfxLookup.erase(  iScene );
				break;
			}
		}

		assert( scene->getReferenceCount() == 0 );
		releaseSceneImpl( scene );		
	}
	
	Scene::Ptr Physics::getScene( gfx::Scene* gfxScene )
	{
		SceneMap::const_iterator iFind = m_sceneGfxLookup.find( gfxScene );
		if ( iFind != m_sceneGfxLookup.end() )
		{
			return iFind->second;
		}
		else
		{
			return 0;
		}

	}

	void Physics::update( Real tDelta, Real tApp )
	{
		if ( m_paused )
			return;

		for ( SceneLookup::const_iterator iScene = m_scenes.begin(); iScene != m_scenes.end(); ++iScene )
		{
			(*iScene)->updateSimulation( tDelta, tApp );
		}
	}

} //END: phys