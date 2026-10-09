/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\Gfx\Scene.cpp
 * Status: duplicate of kismo-v0.3/Gfx/Scene.cpp
 */
#include "Scene.h"
#include "Gfx/Scenetranslator.h"
#include "Gfx/Mesh.h"

//TODO: Create a Scene node factory so no reference to node implementations is needed
#include "Phys/Physics.h"
#include "Phys/Scene.h"
#include "Phys/Actor.h"
#include "Phys/Body.h"
#include "Phys/Shape.h"

namespace gfx
{
	IMPLEMENT_RTTI( gfx, Scene, Node );
	IMPLEMENT_RTTI( gfx, SceneResource, frx::Resource );

	SceneResource::SceneResource( frx::ResourceManager* owner, const String& sceneID, SceneTranslator* loader, const String& filename, Renderer* renderer ) :
		frx::Resource( owner, sceneID, loader, filename )
	{
		m_scene = new Scene( sceneID, renderer );
	}
		
	Bool SceneResource::load()
	{
		if ( frx::isValidPointer( m_loader ) )
		{
			assert( m_loader->isDerived( SceneTranslator::TYPE ) );
			InFStream istream( m_origin.c_str() );
			logMessage( "SceneResource laoding from '" << m_origin.c_str() << "'" );
			return m_loader->reader( this, istream );
		}		
		else
		{
			logError( "No sceneTranslator was specified for loading '" << m_origin.c_str() << "'" );
			return false;
		}
	}

	void SceneResource::unload()
	{

	}

	Scene::Scene( const String& sceneID, Renderer* renderer ) :
		Node( sceneID ),
		m_renderer( renderer )
	{
	}

	Scene::~Scene()
	{
	}


	Spatial* Scene::createChild( const String& id, const String& typeName, const AttributeMap& attributes, Spatial* parentNode )
	{
		Spatial* newSpatial = createChild( id, typeName, parentNode );
		if ( newSpatial )
		{
			if ( newSpatial->setAttributes( attributes ) )
			{
				logSuccess( "Spatial node '" << id << "' created" );
			}
			else
			{
				//We failed, to keep things workign we continue but log an error
				logError( "Failed to set the attributes for the Spatial node '" << id << "'." );
				//frx::safeDelete( newSpatial ); 
			}
		}
		else
		{
			if ( typeName.left( 5 ) == _T("phys:") && !_PhysicsPtr )
			{	
				logWarning( "Node with an id of '" << id << "' and type of '" << typeName << "' is a physics object. A physics plugin must be loaded for the node to be created. Loading of this node had been ignored!" );
			}
			else
			{
				logError( "Node with an id of '" << id << "' and type of '" << typeName << "' could not be created!" );
			}
		}

		return newSpatial;
	}

	Spatial* Scene::createChild( const String& id, const String& typeName, Spatial* parentNode )
	{
		if ( !id.isNull() && Spatial::findObject( id ) )
		{
			logWarning( "An object already exists with the name '" << id << "'! Object created but accessor may return invalid objects." );
			//assert( false );
		}

		Spatial* newSpatial = 0;
		
		//Create the ndoe of the required type
		//TODO:Spatial node factory
		if ( typeName == _T("node") )
			newSpatial = new gfx::Node( id ); //Create the new node
		else
		if ( typeName == _T("mesh") )
			newSpatial = new gfx::MeshNode( id ); //Create the new node
		else
		if ( typeName.left( 5 ) == _T("phys:") && _PhysicsPtr )
		{
			String physType = typeName.right( 5 );
			
			if ( physType == _T("scene") )
				newSpatial = (gfx::Spatial*)_Physics.createScene( id, Ptr( this) ); //Create the new node
			else
			{
				phys::Scene::Ptr scene = _Physics.getScene( this );
				if ( scene.isValid() )
				{
					if ( physType == _T("actor") )
						newSpatial = (gfx::Spatial*)scene->createActor( id ); //Create the new node
					else
					if ( physType == _T("body") )
						newSpatial = (gfx::Spatial*)scene->createBody( id ); //Create the new node
					else
					if ( physType == _T("shape") )
						newSpatial = (gfx::Spatial*)scene->createShape( id ); //Create the new node
					else
					if ( physType == _T("mesh") )
						newSpatial = (gfx::Spatial*)scene->createMeshShape( id ); //Create the new node
					else
					if ( physType == _T("constraint") )
						newSpatial = (gfx::Spatial*)scene->createConstraint( id ); //Create the new node
				}
			}
		}

		if( !newSpatial )
			return 0;
		
		if ( parentNode )
		{
			parentNode->addChild( newSpatial );
		}
		else
		{
			addChild( newSpatial ); //< Add to this
		}

		return newSpatial; //Return to creator
	}

	Node* Scene::createChildNode( const String& id, Node* parentNode )
	{ return (Node*)createChild( id, _T("node"), parentNode ); }

	void Scene::releaseNode( Spatial* node )
	{
		if ( node->isDerived( phys::Scene::TYPE ) )
			_Physics.releaseScene( (phys::Scene*)node );
		else
		if ( node->isDerived( phys::Actor::TYPE ) )
			((phys::Actor*)node)->getScene()->releaseActor( ((phys::Actor*)node) );
		else
		if ( node->isDerived( phys::Body::TYPE ) )
			((phys::Body*)node)->getScene()->releaseBody( ((phys::Body*)node) );
		else
		if ( node->isDerived( phys::Shape::TYPE ) )
			((phys::Shape*)node)->getScene()->releaseShape( ((phys::Shape*)node) );
		else
			delete node; //< Just delete it as we assume we created it

	}

} //END: gfx