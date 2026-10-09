/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\KismoAgeiaPhysicsPlugin\AgeiaScene.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef PHYS_AGEIA_SCENE_H
#define PHYS_AGEIA_SCENE_H

#include "Platform/Platform.h"
#include "Phys/Scene.h"
#include "Utilities.h"
#include <NxScene.h>

//Forward declarations
class NxScene;
class NxUserContactReport;

namespace phys
{
	//Forward declarations
	class AgeiaPhysics;

	class AgeiaScene : public Scene
	{
	public:

		AgeiaScene( const String& name, Physics* owner );

		virtual ~AgeiaScene();

		/** Set the attributes for the spatial node
		*/
		virtual bool setAttributes( const AttributeMap& attributes ) 
		{ return Scene::setAttributes( attributes ); }


		/** Get a pointer to the PhysX scene instance
		*/
		NxScene* getNxScene()
		{ return m_scene; }

		AgeiaPhysics* getAgeiaPhysics()
		{ return (AgeiaPhysics*)getPhysics(); }

		/** Initialise and create the scene
		*/
		virtual Bool initialise();
		
		/** Destroy the scene
		*/
		virtual void destroy();
		
		/** Advance the simulation by a delta time step
		*/
		void updateSimulation( Real tDelta, Real tApp );

		/** Set the value of gravity constant in metres per second
		*/
		virtual void setGravity( const frx::Vector3r& gravity ) 
		{ m_scene->setGravity( toNxVec3( gravity ) ); }

		/** Get the gravitational constant in metres per second
		*/
		virtual frx::Vector3r getGravity() const
		{ 
			NxVec3 tv; m_scene->getGravity(tv);
			return toVec3r( tv ); 
		}

	protected:


		virtual Body* createBodyImpl( const String& name );
		virtual Actor* createActorImpl( const String& name );
		virtual Shape* createShapeImpl( const String& name );
		virtual MeshShape* createMeshShapeImpl( const String& name );
		virtual Constraint* createConstraintImpl( const String& name );
		
		virtual void releaseBodyImpl( Body* body );
		virtual void releaseActorImpl( Actor* actor );
		virtual void releaseShapeImpl( Shape* shape );

	private:
				
		NxScene* m_scene; ///< The ageia scene
		NxUserContactReport* m_contactReport; ///< The contact recieveing object
		NxUserNotify* m_notifyReport; ///< The wake/sleep recieveing object
		
	};



} //END: phys


#endif