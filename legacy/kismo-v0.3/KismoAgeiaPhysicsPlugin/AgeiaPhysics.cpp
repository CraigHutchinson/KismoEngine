/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\KismoAgeiaPhysicsPlugin\AgeiaPhysics.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
#include "AgeiaPhysics.h"
#include "AgeiaScene.h"
#include "ErrorHandler.h"
#include "Utilities.h"
#include "Framework/Log.h"
#include "Gfx/Scene.h"
#include <NxPhysics.h>
#include <NxCooking.h>

namespace phys
{
	AgeiaPhysics::AgeiaPhysics() :
		Physics( _T("ageia_physx") ),			
		m_physicsSDK( 0 )
	{
	}

	AgeiaPhysics::~AgeiaPhysics()
	{
		destroy();
	}


	bool AgeiaPhysics::initialise()
	{
		//Initialse the Ageia physics
		NxPhysicsSDKDesc desc;
		NxSDKCreateError errorCode = NXCE_NO_ERROR;
		m_physicsSDK = NxCreatePhysicsSDK(
			NX_PHYSICS_SDK_VERSION, NULL, &m_errorHandler, desc, &errorCode );
		
		if(m_physicsSDK == NULL) 
		{
			logCritical( "[PhysX] SDK create error " 
				<< errorCode << getNxSDKCreateError( errorCode ) 
				<< " - Unable to initialize the PhysX SDK, exiting the sample" );
			return false;
		}

		m_physicsSDK->setParameter( NX_SKIN_WIDTH, 0.005f );

		
		//initialise the cooking utensils :P
		NxInitCooking();

		//Perform the base intialisation
		return Physics::initialise();
	}


	Bool AgeiaPhysics::reinitialise()
	{
		destroy();
		return initialise();
	}

	void AgeiaPhysics::destroy()
	{		
		if( m_physicsSDK )
		{
			NxReleasePhysicsSDK( m_physicsSDK );
			m_physicsSDK = 0;
		}
	}


	Scene::Ptr AgeiaPhysics::createSceneImpl( const String& name )
	{ return Scene::Ptr( new AgeiaScene( name, this ) ); }

	void AgeiaPhysics::releaseSceneImpl( Scene* scene )
	{ delete (AgeiaScene*)scene; }


} //END: phys