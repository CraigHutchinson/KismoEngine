/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\KismoAgeiaPhysicsPlugin\AgeiaScene.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
#include "AgeiaScene.h"
#include "AgeiaPhysics.h"
#include "AgeiaMaterial.h"
#include "Framework/Log.h"
#include <NxMaterial.h>

#include "AgeiaActor.h"
#include "AgeiaBody.h"
#include "AgeiaShape.h"
#include "AgeiaMeshShape.h"
#include "AgeiaConstraint.h"
#include <NxUserContactReport.h>
#include <NxUserNotify.h>
#include <NxActor.h>

namespace phys
{
	/** \brief Ageia Wake/Sleep notifier 
	*/
	class AgeiaUserNotifyReport : public NxUserNotify
	{
	public:
		AgeiaUserNotifyReport( AgeiaScene* scene ) : m_scene( scene ) {}

		virtual bool onJointBreak(NxReal breakingImpulse, NxJoint& brokenJoint)
		{
			return false;
		}

		virtual void onWake(NxActor** actors, NxU32 count)
		{
			for ( Count i = 0; i < count; ++i )
			{
				assert( actors[ i ]->userData );
				((Actor*)actors[ i ]->userData)->_setIsAwake( true );
			}
		}
		virtual void onSleep(NxActor** actors, NxU32 count)
		{
			for ( Count i = 0; i < count; ++i )
			{
				assert( actors[ i ]->userData );
				((Actor*)actors[ i ]->userData)->_setIsAwake( false );
			}
		}

	protected:
		AgeiaScene* m_scene; ///< The scene the contact report belongs to
	};

	/** \brief Ageia Contact notifier 
	*/
	class AgeiaUserContactReport : public NxUserContactReport
	{
	public:
		AgeiaUserContactReport( AgeiaScene* scene ) : m_scene( scene ) {}

		virtual void  onContactNotify(NxContactPair& pair, NxU32 events)
		{
			//First go through clearing all the current contacts
			NxContactStreamIterator i1(pair.stream);
			while(i1.goNextPair())
			{
				((Actor*)i1.getShape( 0 )->getActor().userData)->clearContacts();
				((Actor*)i1.getShape( 1 )->getActor().userData)->clearContacts();
			}

			// Iterate through the contact points
			NxContactStreamIterator i(pair.stream);
			while(i.goNextPair())
			{
				//We attach the collision markers to the first shape involved in the collision
				Actor* actor = (Actor*)i.getShape( 0 )->getActor().userData;
				assert( actor );

				while(i.goNextPatch())
				{
					//If these contacts are being removed then we continue
					if ( i.getShapeFlags() != NX_TRIGGER_ON_LEAVE )
					{
						const NxVec3& contactNormal = i.getPatchNormal();
						while(i.goNextPoint())
						{
							const NxVec3& contactPoint = i.getPoint();
							actor->addContact( toVec3r( contactPoint ), toVec3r( contactNormal ) );
						}
					}
				}
			}

		}

	protected:
		AgeiaScene* m_scene; ///< The scene the contact report belongs to
	};

	/** Ageida scene implementation */
	AgeiaScene::AgeiaScene(  const String& name, Physics* owner ) :
		Scene( name, owner ),
		m_scene( 0 ),
		m_contactReport( 0 ),
		m_notifyReport( 0 )
	{			
	}

	AgeiaScene::~AgeiaScene()
	{
		destroy();
	}

	Bool AgeiaScene::initialise()
	{
		if ( !m_contactReport )
			m_contactReport = new AgeiaUserContactReport( this );
		assert( m_contactReport );

		if ( !m_notifyReport )
			m_notifyReport = new AgeiaUserNotifyReport( this );
		assert( m_notifyReport );

		if ( !m_defaultMaterial )
			m_defaultMaterial = new AgeiaMaterial( this, 0 );
		assert( m_defaultMaterial );
		
		// Create a scene
		NxSceneDesc sceneDesc;
		sceneDesc.gravity = NxVec3(0.0f, -9.81f, 0.0f);

		//Set the contact reporter to get collision points
		sceneDesc.userContactReport = m_contactReport;	
		
		//Set the reporter to receive the sleep and wake events
		sceneDesc.userNotify = m_notifyReport;			

		//sceneDesc.broadPhase = NX_BROADPHASE_COHERENT;
		//sceneDesc.collisionDetection = true;

		m_scene = getAgeiaPhysics()->getNxSDK()->createScene(sceneDesc);
		if( !m_scene ) 
		{
			logError("[PhysX] Error: Unable to create a PhysX scene, exiting the sample.");
			return false;
		}

		//Turn on collision notification between all actors in the scene
		m_scene->setActorGroupPairFlags( 0 ,0, 
			NX_NOTIFY_ON_START_TOUCH | NX_NOTIFY_ON_TOUCH | NX_NOTIFY_ON_END_TOUCH );

		// Set default material settings. these can be ovverriden later as requireed
		m_defaultMaterial->setRestitution(0.0f);
		m_defaultMaterial->setStaticFriction(0.5f);
		m_defaultMaterial->setDynamicFriction(0.5f);
		m_defaultMaterial->initialise();

		//Call the base initialise
		return Scene::initialise();
	}

	void AgeiaScene::destroy()
	{		
		if( m_scene ) 
		{
			if ( getAgeiaPhysics()->getNxSDK() )
				getAgeiaPhysics()->getNxSDK()->releaseScene( *m_scene );
			m_scene = 0;
		}
	}
	
	void AgeiaScene::updateSimulation( Real tDelta, Real tApp )
	{
		assert( m_scene );
		
		//Update the simulation
		m_scene->simulate( tDelta );

		// Fetch simulation results from the last frame
		m_scene->flushStream();
		m_scene->fetchResults( NX_ALL_FINISHED, true );

	}

	
	Body* AgeiaScene::createBodyImpl( const String& name )
	{ return new AgeiaBody( name, Ptr(this) ); }

	Actor* AgeiaScene::createActorImpl( const String& name )
	{ return new AgeiaActor( name, Ptr(this) ); }

	Shape* AgeiaScene::createShapeImpl(  const String& name )
	{ return new AgeiaShape( name, Ptr(this) ); }

	MeshShape* AgeiaScene::createMeshShapeImpl(  const String& name )
	{ return new AgeiaMeshShape( name, Ptr(this) ); }

	Constraint* AgeiaScene::createConstraintImpl(  const String& name )
	{ return new AgeiaConstraint( name, Ptr(this) ); }
	
	void AgeiaScene::releaseBodyImpl( Body* body )
	{ delete (AgeiaBody*)body; }

	void AgeiaScene::releaseActorImpl( Actor* actor )
	{ delete (AgeiaScene*)actor; }

	void AgeiaScene::releaseShapeImpl( Shape* shape )
	{ delete (AgeiaShape*)shape; }

} //END: phys