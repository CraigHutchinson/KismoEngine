/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\KismoAgeiaPhysicsPlugin\AgeiaPhysics.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef OPENGL_RENDERER_H
#define OPENGL_RENDERER_H

#include "Platform/Platform.h"
#include "Phys/Physics.h"
#include "Phys/Scene.h"
#include "ErrorHandler.h"

//Forward declarations
class NxPhysicsSDK;

namespace phys
{

	class AgeiaPhysics : public Physics
	{
	public:

		AgeiaPhysics();

		virtual ~AgeiaPhysics();
		
		virtual Bool initialise();
		
		/** Reinitialise the Physics if the config has changed. This will reset the scene
		*/
		virtual Bool reinitialise();
		
		/** Deletes the physics scene and free's any memory used
		*/
		virtual void destroy();

		/** Get a pointer to the PhysX SDK instance
		*/
		NxPhysicsSDK* getNxSDK()
		{ return m_physicsSDK; }
		
	protected:

		virtual Scene::Ptr createSceneImpl( const String& name );

		virtual void releaseSceneImpl( Scene* scene );

	private:
				
		NxPhysicsSDK* m_physicsSDK; ///< THe physics core sdk
		ErrorHandler m_errorHandler; //< error handler for dealing with errors from the Physics SDK

	};



} //END: phys


#endif