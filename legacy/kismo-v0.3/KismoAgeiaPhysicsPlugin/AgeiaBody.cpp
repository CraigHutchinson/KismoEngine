/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\KismoAgeiaPhysicsPlugin\AgeiaBody.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
#include "AgeiaBody.h"
#include "AgeiaScene.h"
#include "Utilities.h"
#include "AgeiaActor.h"
#include <NxBodyDesc.h>
#include <NxActor.h>

namespace phys
{
	AgeiaBody::AgeiaBody( const String& name, const Scene::Ptr& owner ) :
		Body( name, owner ) 
	{
		m_bodyDescription = new NxBodyDesc;
		m_bodyDescription->setToDefault(); ///< Set to the defaults

		
		//m_bodyDescription->angularDamping	= 0.5f;
		//m_bodyDescription->linearDamping	= 0.5f;
		m_bodyDescription->mass	= 1.0f;
	}

	AgeiaBody::~AgeiaBody() 
	{
		delete m_bodyDescription;
	}

	NxScene* AgeiaBody::getNxScene() const
	{ return ((AgeiaScene*)getScene())->getNxScene(); }

	void AgeiaBody::setMass( Real mass )
	{
		m_bodyDescription->mass = mass;
		if ( getActor() && ((AgeiaActor*)getActor())->getNxActor() )
			((AgeiaActor*)getActor())->getNxActor()->setMass( mass );
	}

	Real AgeiaBody::getMass() const
	{ return m_bodyDescription->mass; }

	void AgeiaBody::setAngularDamping( Real damp )
	{ 
		m_bodyDescription->angularDamping = damp; 
		if ( getActor() && ((AgeiaActor*)getActor())->getNxActor() )
			((AgeiaActor*)getActor())->getNxActor()->setAngularDamping( damp );
	}

	Real AgeiaBody::getAngularDamping() const
	{ return m_bodyDescription->angularDamping; }

	void AgeiaBody::setLinearDamping( Real damp )
	{
		m_bodyDescription->linearDamping = damp; 
		if ( getActor() && ((AgeiaActor*)getActor())->getNxActor() )
			((AgeiaActor*)getActor())->getNxActor()->setLinearDamping( damp );
	}

	Real AgeiaBody::getLinearDamping() const
	{ return m_bodyDescription->linearDamping; }

	void AgeiaBody::setCentreOfMass( const frx::Vector3r& pos )
	{
		m_bodyDescription->massLocalPose.t = toNxVec3( pos );		
		if ( getActor() && ((AgeiaActor*)getActor())->getNxActor() )
			((AgeiaActor*)getActor())->getNxActor()->setCMassOffsetLocalPosition( m_bodyDescription->massLocalPose.t ) ;
	}
	
	frx::Vector3r AgeiaBody::getCentreOfMass() const
	{
		return toVec3r( m_bodyDescription->massLocalPose.t );
	}
	
	void AgeiaBody::setInertiaTensor( const frx::Vector3r& it )
	{
		m_bodyDescription->massSpaceInertia = toNxVec3( it ); 
		if ( getActor() && ((AgeiaActor*)getActor())->getNxActor() )
			((AgeiaActor*)getActor())->getNxActor()->setMassSpaceInertiaTensor( m_bodyDescription->massSpaceInertia ) ;
	}

	frx::Vector3r AgeiaBody::getInertiaTensor() const
	{		
		if ( getActor() && ((AgeiaActor*)getActor())->getNxActor() )
			m_bodyDescription->massSpaceInertia = ((AgeiaActor*)getActor())->getNxActor()->getMassSpaceInertiaTensor() ;

		return toVec3r( m_bodyDescription->massSpaceInertia );
	}

} //END; phys