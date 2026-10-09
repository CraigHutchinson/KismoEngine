/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\KismoAgeiaPhysicsPlugin\AgeiaActor.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
#include "AgeiaActor.h"
#include "AgeiaMaterial.h"
#include <NxActor.h>
#include <NxActorDesc.h>
#include <NxPlaneShapeDesc.h>
#include <NxBoxShapeDesc.h>
#include "Utilities.h"
#include "AgeiaBody.h"
#include "AgeiaMeshShape.h"
#include "AgeiaShape.h"
#include "phys/Body.h"
#include "phys/Shape.h"
#include "Framework/Log.h"

namespace phys
{
	
	AgeiaActor::AgeiaActor( const String& name, const Scene::Ptr& owner ) :
		Actor( name, owner ),
		m_actor( 0 ),
		m_forceMode( NX_FORCE )
	{
		m_actorDescription = new NxActorDesc();	
		setMaterial( new AgeiaMaterial( owner ) );
	}

	AgeiaActor::~AgeiaActor()
	{
		destroy();
		delete m_actorDescription;
	}

	void AgeiaActor::addForceAtPos( const frx::Vector3r& worldForce, const frx::Vector3r& worldPos )
	{ m_actor->addForceAtPos( toNxVec3( worldForce ), toNxVec3( worldPos ), (NxForceMode)m_forceMode ); }

	void AgeiaActor::addForceAtLocalPos( const frx::Vector3r& worldForce, const frx::Vector3r& localPos )
	{ m_actor->addForceAtLocalPos( toNxVec3( worldForce ), toNxVec3( localPos ), (NxForceMode)m_forceMode ); }

	void AgeiaActor::addLocalForceAtPos( const frx::Vector3r& localForce, const frx::Vector3r& worldPos )
	{ m_actor->addLocalForceAtPos( toNxVec3( localForce ), toNxVec3( worldPos ), (NxForceMode)m_forceMode ); }

	void AgeiaActor::addLocalForceAtLocalPos( const frx::Vector3r& localForce, const frx::Vector3r& localPos )
	{ m_actor->addLocalForceAtLocalPos( toNxVec3( localForce ), toNxVec3( localPos ), (NxForceMode)m_forceMode ); }

	void AgeiaActor::addForce( const frx::Vector3r& worldForce )
	{ m_actor->addForce( toNxVec3( worldForce ), (NxForceMode)m_forceMode ); }

	void AgeiaActor::addLocalForce( const frx::Vector3r& localForce )
	{ m_actor->addLocalForce( toNxVec3( localForce ), (NxForceMode)m_forceMode ); }

	void AgeiaActor::addTorque( const frx::Vector3r& worldTorque )
	{ m_actor->addTorque( toNxVec3( worldTorque ), (NxForceMode)m_forceMode ); }

	void AgeiaActor::addLocalTorque( const frx::Vector3r& localTorque )
	{ m_actor->addLocalTorque( toNxVec3( localTorque ), (NxForceMode)m_forceMode ); }

	Bool AgeiaActor::initialise()
	{		
		if ( m_actor )
		{
			assert( false );
			return false;
		}

		//Get the child body. This will retrun false if the actor is passive
		getChildBody();

		// Get the child shapes of the actor
		getChildShapes();

		if ( !getShapeCount() )
		{
			logRecoverable( "The actor '" << getName() << "' has no shapes. Defaulting to sibling meshes where possible" );

			//logError( _T("No collision shape specified for actor '") << getName() << _T("'") );
			//return false;

			//If there are no defined shapes we use the sibling meshes to create the mesh shapes automatically
			//Create shapes using the actors sibling meshes
			if ( !createSiblingMeshShapes() )
			{
				logError( "Failed to set a collision shape for '" << getName() << "'" );
				return false;
			}

			//get the shapes of the new meshes
			if ( !getChildShapes() )
			{
				return false;
			}
		}		

		//Initialise the material which we use for all the shapes for now
		assert( getMaterial() );
		if ( !getMaterial()->initialise() )
		{
			logError( "Failed to create material for actor '" << getName() << "'" );
			return false;
		}

		//Build the actors shape list
		m_actorDescription->shapes.clear();
		for ( const_shape_iterator iShape = shapeBegin(); iShape != shapeEnd(); ++iShape )
		{
			Shape* shape = (*iShape);
			NxShapeDesc* nxShape = 0;
			if ( shape->isDerived( phys::MeshShape::TYPE ) )
			{
				//If we have a mesh shape it needs to be initialised to construct the geometry
				if ( !((AgeiaMeshShape*)shape)->initialise() )
					return false;
				nxShape = ((AgeiaMeshShape*)shape)->getNxShape();
			}
			else
			{
				//Get the shape of the shape object
				nxShape = ((AgeiaShape*)shape)->getNxShape() ;
			}

			assert( nxShape );
			//Set the material for the shape
			nxShape->materialIndex = ((AgeiaMaterial*)getMaterial())->getMaterialindex();
			m_actorDescription->shapes.pushBack( nxShape );
		}

		//Set the inital pose of the actor
		m_actorDescription->globalPose.t = toNxVec3( worldTransform().getTranslate() );
		
		//If the actor has an orientation we need to rotate its initial pose
		if ( worldTransform().isRSMatrix() )
			m_actorDescription->globalPose.M = toNxMat33( worldTransform().getRotate() );
		
		//Create the actor
		NxScene* scene = ((AgeiaScene*)getScene())->getNxScene();

		//Verify that there are shapes for this actor
		//TODO: Its valid not to have a shape so we need to take this into account
		if ( m_actorDescription->shapes.isEmpty() )
		{
			logError( "No shapes were created for the actor '" << getName() << " and is therefore invalid!" );
			return false;
		}

		//Make sure the description fo the actor is valid
		if ( !m_actorDescription->isValid() )
		{
			logError( "The actor description for '" << getName() << " is invalid!" );
			return false;
		}

		assert( scene );
		//use the scene to create the actor
		m_actor = scene->createActor( *m_actorDescription );

		if ( m_actor != 0 )
		{
			// Store the pointer to this so we can display contacts when they occur
			m_actor->userData = this;

			if ( m_actorDescription->body )
			{
				getBody()->setCentreOfMass( toVec3r( m_actorDescription->body->massLocalPose.t ) );
			}
			return true;
		}
		else
			return false;
	}

	bool AgeiaActor::createSiblingMeshShapes( gfx::Spatial* searchNode )
	{
		if ( searchNode == 0 )
			searchNode = getParent();

		assert( searchNode );
		if ( !searchNode )
			return false;

		for ( const_iterator iChild = searchNode->childBegin(); iChild != searchNode->childEnd(); ++iChild )
		{
			assert( (*iChild) );
			if ( (*iChild)->isDerived( gfx::MeshNode::TYPE ) )
			{
				gfx::MeshNode::Ptr meshNode( (gfx::MeshNode*)(*iChild) );

				AgeiaMeshShape::Ptr newMeshShape( new AgeiaMeshShape( meshNode->getName() + _T("_AutoMeshShape"), getScene() ) );
				if ( newMeshShape->setMesh( meshNode->getMesh() ) )
				{
					//Add the new mesh shape to this actor
					addChild( newMeshShape );
				}
				else
				{
					logError( "Failed to create a mesh shape from actor '" << getName() << "' siblings" );
					//We failed!!
					return false;
				}
			}
			else
			if ( (*iChild) != this ) ///Make sure we arent' searching within 'this'
			{
				//Search teh transdform nodes
				//TODO: transofrmm for the shape
				createSiblingMeshShapes( (*iChild) );
			}
		}

		return true;
	}

	void AgeiaActor::destroy()
	{
		if ( m_actor )
			getNxScene()->releaseActor( *m_actor );
	}

	void AgeiaActor::setBody( Body* body )
	{
		if ( getBody() == body )
			return;

		//Set the body
		Actor::setBody( body );

		if ( !getBody() )
			m_actorDescription->body = 0;
		else
			m_actorDescription->body = ((AgeiaBody*)getBody())->getNxBody();
	}

	void AgeiaActor::updateWorldData( Real tDelta, Real tApp )
	{
		if ( !m_firstUpdate )
		{
			if ( !m_actor )
				return;

			NxMat34 mat = m_actor->getGlobalPose();
			worldTransform().setTranslate( toVec3r( mat ) );
			worldTransform().setRotate( toRotateMatrix4r( mat ) );
		}
		Actor::updateWorldData( tDelta, tApp );
	}

	void AgeiaActor::setCurrentPose()
	{
		assert( m_actor );
		m_actor->setGlobalPosition( toNxVec3( worldTransform().getTranslate() ) );
		
		if ( worldTransform().isRSMatrix() )
			m_actor->setGlobalOrientation( toNxMat33( worldTransform().getRotate() ) );
		
	}


	frx::Vector3r AgeiaActor::getAngularVelocity() const
	{
		assert( m_actor ); assert( getBody() ); ///< Must be dynamic
		return toVec3r( m_actor->getAngularVelocity() );
	}
	
	void AgeiaActor::setAngularVelocity( const frx::Vector3r& angVel )
	{
		assert( m_actor ); assert( getBody() ); ///< Must be dynamic
		m_actor->setAngularVelocity( toNxVec3( angVel ) );
	}

	frx::Vector3r AgeiaActor::getLinearVelocity() const
	{
		assert( m_actor ); assert( getBody() ); ///< Must be dynamic
		return toVec3r( m_actor->getLinearVelocity() );
	}

	void AgeiaActor::setLinearVelocity( const frx::Vector3r& linVel )
	{
		assert( m_actor ); assert( getBody() ); ///< Must be dynamic
		m_actor->setLinearVelocity( toNxVec3( linVel ) );
	}

	void AgeiaActor::setIterationCount( Count count )
	{
		assert( m_actor ); assert( getBody() ); ///< Must be dynamic
		m_actor->setSolverIterationCount( (NxU32)count );
	}

} //END; phys
