/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\KismoAgeiaPhysicsPlugin\AgeiaMaterial.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
#include "AgeiaMaterial.h"
#include <NxMaterial.h>
#include "Framework/Log.h"

namespace phys
{
	AgeiaMaterial::AgeiaMaterial( Scene* owner, NxMaterialIndex manualIndex ) :
		Material( owner ),
		m_materialIndex( manualIndex ),
		m_material( 0 )
	{
		m_materialDescription.frictionCombineMode = NX_CM_MULTIPLY;
	}
	
	AgeiaMaterial::~AgeiaMaterial()
	{
		destroy();
	}

	Bool AgeiaMaterial::initialise()
	{
		if ( m_material )
		{
			assert( false );
			logWarning( "Material is already intiialised!" );
			return false;
		}

		if ( m_materialIndex != (NxMaterialIndex)-1 )
		{
			//Define the default material
			m_material = getNxScene()->getMaterialFromIndex( m_materialIndex );
			assert( m_material );
			m_material->loadFromDesc( m_materialDescription );
		}
		else
		{
			//We need to create the new material
			m_material = getNxScene()->createMaterial( m_materialDescription );
			assert( m_material );
			m_materialIndex = m_material->getMaterialIndex();
		}

		return true;
	}

	void AgeiaMaterial::destroy()
	{
		if ( m_material && m_materialIndex != 0 )
		{
			//Release the material
			getNxScene()->releaseMaterial( *m_material );
			m_materialIndex = (NxMaterialIndex)-1;
		}
		m_material = 0;
	}
	
	void AgeiaMaterial::setDynamicFriction( Real friction )
	{
		m_materialDescription.dynamicFriction = friction;
		m_materialDescription.dynamicFrictionV = friction;
		if ( m_material )
			m_material->setDynamicFriction( friction );
	}
		
	Real AgeiaMaterial::getDynamicFriction() const
	{
		if ( m_material )
			return m_material->getDynamicFriction();
		else
			return m_materialDescription.dynamicFriction;
	}
	
	void AgeiaMaterial::setStaticFriction( Real friction )
	{
		m_materialDescription.staticFriction = friction;
		m_materialDescription.staticFrictionV = friction;
		if ( m_material )
			m_material->setStaticFriction( friction );
	}
		
	Real AgeiaMaterial::getStaticFriction() const
	{
		if ( m_material )
			return m_material->getStaticFriction();
		else
			return m_materialDescription.staticFriction;
	}
	
	void AgeiaMaterial::setRestitution( Real bounciness )
	{
		m_materialDescription.restitution = bounciness;
		if ( m_material )
			m_material->setRestitution( bounciness );
	}
		
	Real AgeiaMaterial::getRestitution() const
	{
		if ( m_material )
			return m_material->getRestitution();
		else
			return m_materialDescription.restitution;
	}

} //END: phys