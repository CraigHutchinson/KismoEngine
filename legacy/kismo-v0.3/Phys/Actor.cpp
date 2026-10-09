/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\Phys\Actor.cpp
 * Status: current (newest copy of this file name in the archive)
 */
#include "Actor.h"
#include "phys/Body.h"
#include "Phys/Scene.h"
#include "Phys/Material.h"
#include "Gfx/Node.h"
#include "Gfx/Renderer.h"

namespace phys
{
	IMPLEMENT_RTTI( phys, Actor, gfx::Node );

	Actor::Actor( const String& name, const Scene::Ptr& owner ) :
		gfx::Node( name ),
		m_owner( owner ),
		m_body( 0 ),
		m_firstUpdate( true ),
		m_material( 0 ),
		m_isAwake( true )
	{

		m_wireframeState = new gfx::WireframeState( _T("ActorWireframe") );
		m_wireframeState->colourOverride = true;

		//Set the node render state so that all child nodes are in wireframe mode
		setRenderState( m_wireframeState );
	}

	Actor::~Actor()
	{
	}

	void Actor::setBody( Body* body )
	{ 
		if ( m_body == body )
			return;

		m_body = body; 
		if ( m_body  && m_body->getActor() != this )
			m_body->setActor( this );
	}
	
	void Actor::addShape( Shape* shape )
	{
		if ( !hasShape( shape ) )
		{
			m_shapes.insert( Shape::Ptr(shape) );
			if ( shape && shape->getActor() != this )
				shape->setActor( this );
		}
	}
	
	Bool Actor::hasShape( Shape* shape )
	{
		return m_shapes.find( shape ) != m_shapes.end();
	}

	bool Actor::getChildShapes( gfx::Spatial* node )
	{	
		if ( !node )
			node = this;

		//Check child nodes for the actor properties for the body and shapes
		for ( iterator iChild = node->childBegin(); iChild != node->childEnd(); ++iChild )
		{
			if ( (*iChild)->isDerived( phys::Shape::TYPE ) )
			{
				addShape( (Shape*)(*iChild).getPtr() );
			}
			else
				getChildShapes( (*iChild) );
		}
		return getShapeCount() != 0;
	}

	bool Actor::getChildBody()
	{		
		//Check child nodes for the actor properties for the body and shapes
		for ( iterator iChild = childBegin(); iChild != childEnd(); ++iChild )
		{
			if ( (*iChild)->isDerived( phys::Body::TYPE ) )
			{
				setBody( (Body*)(*iChild).getPtr() );
				return true;
			}
		}
		
		return false;
	}

	void Actor::updateWorldData( Real tDelta, Real tApp )
	{
		if ( m_firstUpdate )
		{
			Spatial::updateWorldData( tDelta, tApp ); //Update the world pose to get the position etc
			initialise(); //< initialise the actor
			m_firstUpdate = false;
		}
		else
		{
			assert( getParent() );
			if ( !getParent() )
				return;
			
			getParent()->setWorldIsCurrent( true );
			getParent()->worldTransform() = worldTransform();
		}

		//Update the shapes for drawing their collision shapes
		for ( Shape::PtrLookup::const_iterator iShape = m_shapes.begin();
			iShape != m_shapes.end();
			++iShape )
		{
			(*iShape)->updateGeometryState( tDelta, tApp, false );
		}

	}

	bool Actor::setAttributes( const AttributeMap& attributes )
	{
		//Call the base spatial set attributes
		if ( !Spatial::setAttributes( attributes ) )
			return false;

		AttributeMap::const_iterator iFind;
				
		//Initialsie the material attributes
		assert( m_material );
		if ( (iFind = attributes.find( _T("dynamicFriction") )) != attributes.end() )
			m_material->setDynamicFriction( iFind->second.asReal() );

		if ( (iFind = attributes.find( _T("staticFriction") )) != attributes.end() )
			m_material->setStaticFriction( iFind->second.asReal() );

		if ( (iFind = attributes.find( _T("bounciness") )) != attributes.end() )
			m_material->setRestitution( iFind->second.asReal() );

		//TODO: Collision layers
		//if ( (iFind = attributes.find( _T("collisionLayer") )) != attributes.end() )
		//	setCollisionLayer( iFind->second.asInt32() );

		return true;
	}


	void Actor::doDraw( gfx::Renderer& renderer ) const
	{
		if ( !getBody() )
			m_wireframeState->colour = frx::Colour4::Blue;
		else
		if ( isAwake() )
			m_wireframeState->colour = frx::Colour4::Green;
		else
			m_wireframeState->colour = frx::Colour4::Red;
		
		for ( Shape::PtrLookup::const_iterator iShape = m_shapes.begin();
			iShape != m_shapes.end();
			++iShape )
		{
			(*iShape)->doDraw( renderer );
		}

		for ( ContactArray::const_iterator iContact = m_contacts.begin();
			iContact != m_contacts.end();
			++iContact )
		{
			//Set the position of the contact marker
			getScene()->getContactMarker()->worldTransform().setTranslate( iContact->position );
			//TODO: Rotate to match the normal of the contact
			
			//Draw the marker
			renderer.draw( getScene()->getContactMarker() );
		}
	}

	void Actor::clearContacts()
	{
		m_contacts.clear();
	}

	void Actor::addContact( const math::Vector3& position, const math::Vector3& normal )
	{
		Contact contact = { position, normal };
		m_contacts.push_back( contact );
	}


} // END: phys