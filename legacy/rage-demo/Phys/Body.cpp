/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\Phys\Body.cpp
 * Status: duplicate of kismo-v0.3/Phys/Body.cpp
 */
#include "Body.h"
#include "phys/Actor.h"
#include "phys/Scene.h"

namespace phys
{
	IMPLEMENT_RTTI( phys, Body, gfx::Spatial );

	Body::Body( const String& name, const Scene::Ptr& owner ) :
		Spatial( name  ),
		m_owner( owner ),
		m_actor( 0 )
	{}
		
	Body::~Body()
	{}

	void Body::setActor( Actor* actor )
	{ 
		if ( m_actor == actor )
			return;

		m_actor = actor; 
		if ( m_actor && m_actor->getBody() != this )
			m_actor->setBody( this );
	}
	
	bool Body::setAttributes( const AttributeMap& attributes )
	{
		//Call the base spatial set attributes
		if ( !Spatial::setAttributes( attributes ) )
			return false;

		AttributeMap::const_iterator iFind;
				
		//Check for the centre of mass type attribute
		if ( (iFind = attributes.find( _T("centreOfMass") )) != attributes.end() )
			setCentreOfMass( iFind->second );

		if ( (iFind = attributes.find( _T("mass") )) != attributes.end() )
			setMass( iFind->second.asReal() );

		if ( (iFind = attributes.find( _T("damping") )) != attributes.end() )
		{
			setAngularDamping( iFind->second.asReal() );
			setLinearDamping( iFind->second.asReal() );
		}

		if ( (iFind = attributes.find( _T("angularDamping") )) != attributes.end() )
			setAngularDamping( iFind->second.asReal() );

		if ( (iFind = attributes.find( _T("linearDamping") )) != attributes.end() )
			setLinearDamping( iFind->second.asReal() );

		return true;
	}
		
	

} //END: phys