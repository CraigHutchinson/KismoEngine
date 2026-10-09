/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\KismoAgeiaPhysicsPlugin\AgeiaActor.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef PHYS_AGEIA_ACTOR_H
#define	PHYS_AGEIA_ACTOR_H

#include "phys/Actor.h"
#include "AgeiaScene.h"

//forward declaration
class NxActor;
class NxActorDesc;

namespace phys
{
	/** Ageia Actor
	*/
	class AgeiaActor : public Actor
	{		
	public:

		AgeiaActor( const String& name, const Scene::Ptr& owner );

		virtual ~AgeiaActor();

		/** Geta pointer to the actor
		*/
		NxActor* getNxActor() const
		{ return m_actor; }
		
		/** Get a pointer to the PhysX scene instance
		*/
		NxScene* getNxScene() const
		{ return ((AgeiaScene*)getScene())->getNxScene(); }

		/** Initialise and create the scene
		*/
		virtual Bool initialise();
		
		/** Destroy the scene
		*/
		virtual void destroy();

		/** Apply a force to the actor in world coordinates
		*/
		virtual void addForceAtPos( const frx::Vector3r& worldForce, const frx::Vector3r& worldPos );

		/** Apply a force to the actor whose direction is in world coordinates but the position is local
		*/
		virtual void addForceAtLocalPos( const frx::Vector3r& worldForce, const frx::Vector3r& localPos );

		/** Apply a force to the actor whose direction is in local coordinates but positio is in the world space
		*/
		virtual void addLocalForceAtPos( const frx::Vector3r& localForce, const frx::Vector3r& worldPos );

		/** Apply a force to the actor in local coordinates at a local position
		*/
		virtual void addLocalForceAtLocalPos( const frx::Vector3r& localForce, const frx::Vector3r& localPos );

		/** Apply a force to the actor in world coordinates through the center of the actor
		*/
		virtual void addForce( const frx::Vector3r& worldForce );

		/** Apply a force to the actor in local coordinates through the center of the actor
		*/
		virtual void addLocalForce( const frx::Vector3r& localForce );

		/** Apply a force to the actor in world coordinates
		*/
		virtual void addTorque( const frx::Vector3r& worldTorque );

		/** Apply a force to the actor in local coordinates
		*/
		virtual void addLocalTorque( const frx::Vector3r& localTorque );

		/** set the body that belongs to this actor
		*/
		virtual void setBody( Body* body );

		/** Set the current position position
		*/
		virtual void setCurrentPose();

		/** get the angular velocity of the actor
		*/
		virtual frx::Vector3r getAngularVelocity() const;
		

		/** Set the angular velocity of the actor
		*/
		virtual void setAngularVelocity( const frx::Vector3r& angVel );
		
		/** get the linear velocity of the actor
		*/
		virtual frx::Vector3r getLinearVelocity() const;

		/** Set the linear velocity of the actor
		*/
		virtual void setLinearVelocity( const frx::Vector3r& linVel );

		/** Set the iterator count.
			\remarks Higher values increase accuracy but have a performance hit
			\param[in] count Value between 1 and 255
		*/
		virtual void setIterationCount( Count count );
	protected:

		void updateWorldData( Real tDelta, Real tApp );

		/** Creates MeshShapes using the meshes of the search node that is supplied
			\remarks Used if no collision shape has been scpecified for the actor
		*/
		bool createSiblingMeshShapes( gfx::Spatial* searchNode = 0 );


	private:
		int m_forceMode; //< The ageia force mode used when applying forces (default NX_FORCE)

		NxActorDesc* m_actorDescription; ///< The actor descriotpion used to create the actor
		NxActor* m_actor; ///< The physics actor		
		
	};

} // END: phys

#endif