/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\Phys\Scene.cpp
 * Status: superseded by kismo-v0.3/Gfx/Scene.cpp (same name, content differs)
 */
#include "Scene.h"
#include "Gfx/MeshManager.h"
#include "Gfx/Renderer.h"
#include "Phys/Body.h"
#include "Phys/Shape.h"
#include "Phys/Actor.h"
#include "Phys/Physics.h"
#include "Framework/Log.h"

namespace phys
{
	IMPLEMENT_RTTI( phys, Scene, gfx::Spatial );

	Scene::Scene( const String& name, Physics* owner ) :
		gfx::Spatial( name ),
		m_owner( owner ),
		m_defaultMaterial( 0 )
	{
		assert( owner ); 
	}

		
	Scene::~Scene()
	{}

	Bool Scene::initialise()
	{
		
		gfx::Mesh::Ptr contactMarkerMesh = _MeshManager.create( _T("PointMarker.rmf") );
		//Check if the marker mesh loaded OK
		if ( !contactMarkerMesh.isValid() )
		{
			logError( "Point marker mesh not found for collision reports!" );
			return false;
		}

		m_contactMarker = new gfx::MeshNode( _T("ContactMarker") );
		m_contactMarker->setMesh(contactMarkerMesh );

		return true;
	}

	Body* Scene::createBody( const String& name )
	{
		Body* newBody = createBodyImpl( name );
		return newBody;
	}

	Actor* Scene::createActor( const String& name )
	{
		Actor* newActor = createActorImpl( name );

		//Store the new actor in the lookup map
		m_actors[ name ] = newActor;

		return newActor;
	}

	Shape* Scene::createShape( const String& name )
	{
		Shape* newShape = createShapeImpl( name );
		return newShape;
	}

	MeshShape* Scene::createMeshShape( const String& name )
	{
		MeshShape* newMeshShape = createMeshShapeImpl( name );
		return newMeshShape;
	}
	
	Constraint* Scene::createConstraint( const String& name )
	{
		Constraint* newConstraint = createConstraintImpl( name );
		
		//Store the new constraint in the lookup map
		m_constraints[ name ] = newConstraint;

		return newConstraint;
	}

	void Scene::releaseBody( Body* body )
	{
		assert( body->getReferenceCount() == 0 );
		releaseBodyImpl( body );
	}

	void Scene::releaseActor( Actor* actor )
	{
		assert( actor->getReferenceCount() == 0 );
		releaseActorImpl( actor );
	}

	void Scene::releaseShape( Shape* shape )
	{
		assert( shape->getReferenceCount() == 0 );
		releaseShapeImpl( shape );
	}

	
	void Scene::doDraw( gfx::Renderer& renderer ) const
	{
		if ( m_contactMarker.isNull() )
		{
			assert( false );
			return; //Can't draw without a marker to use!
		}
	}

	
} //END: phys