/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\Phys\Shape.cpp
 * Status: current (newest copy of this file name in the archive)
 */
#include "Shape.h"
#include "Phys/Actor.h"
#include "Phys/Scene.h"
#include "Phys/Physics.h"
#include "Gfx/Renderer.h"

namespace phys
{
	IMPLEMENT_RTTI( phys, Shape, gfx::Spatial );

	Shape::Shape( const String& name, const Scene::Ptr& owner ) :
		gfx::Spatial( name ),
		m_owner( owner ),
		m_actor( 0 ),
		m_shapeType( Shape_Invalid )
	{
	}

	Shape::~Shape()
	{
	}

	void Shape::setActor( Actor* actor )
	{ 
		if ( m_actor == actor )
			return;

		m_actor = actor; 
		if ( m_actor && !m_actor->hasShape( this ) )
			m_actor->addShape( this );
	}

	bool Shape::setAttributes( const AttributeMap& attributes )
	{
		//Call the base spatial set attributes
		if ( !Spatial::setAttributes( attributes ) )
			return false;

		AttributeMap::const_iterator iFind;
				
		//Check for the shape type attribute
		if ( (iFind = attributes.find( _T("type") )) != attributes.end() )
		{
			//Set the type of shape volume
			if ( iFind->second == _T("plane") )
				setShape( Shape_Plane );
			else
			if ( iFind->second == _T("sphere") )
				setShape( Shape_Sphere );
			else
			if ( iFind->second == _T("box") )
				setShape( Shape_Box );
			else
				setShape( Shape_Invalid );
		}

		//Call the shape specific setAttrributes
		switch( getShapeType() )
		{
		case Shape_Plane:
			return setPlaneAttributes( attributes );
		case Shape_Sphere:
			return setSphereAttributes( attributes );
		case Shape_Box:
			return setBoxAttributes( attributes );
		default:
			assert( false );
			return false;
		}
	}

	bool Shape::setPlaneAttributes( const AttributeMap& attributes )
	{
		assert( m_shapeType == Shape_Plane );
		return true;
	}

	bool Shape::setBoxAttributes( const AttributeMap& attributes )
	{
		assert( m_shapeType == Shape_Box );
		return true;
	}

	bool Shape::setSphereAttributes( const AttributeMap& attributes )
	{
		assert( m_shapeType == Shape_Sphere );

		AttributeMap::const_iterator iFind;
		if ( (iFind = attributes.find( _T("radius") )) != attributes.end() )
		{
			setRadius( attributes.find( _T("radius") )->second.asReal() );
		}

		return true;
	}

	void Shape::setShape( eShape shapeType )
	{
		if ( m_shapeType == shapeType )
			return;

		m_shapeType = shapeType;
		setShapeImpl();
	}

	void Shape::updateState( RenderStateStack& states, LightStack& lights )
	{	
		//Get the top states of the stack and store them for when we need to render
		for ( RenderStateStack::const_iterator iState = states.begin(); iState != states.end(); ++iState )
		{
			if ( iState->second.empty() ) continue;
			m_renderStates[ iState->first ] = iState->second.top();
			assert( m_renderStates[ iState->first ] );
		}
	}

	
	void Shape::doDraw( gfx::Renderer& renderer ) const
	{
		if ( !getPhysics()->getWireframe() )
			return;

		//Check if we need to set a new render state
		if ( m_renderStates.size() )
		{
			renderer.pushRenderState();

			for ( RenderStateMap::const_iterator iState = m_renderStates.begin();
				iState != m_renderStates.end();
				++iState )
			{
				renderer.setRenderState( iState->second );
			}
			
			doDrawShape( renderer );

			renderer.popRenderState();
		}
		else
		{				
			doDrawShape( renderer );
		}
	}

	void Shape::doDrawShape( gfx::Renderer& renderer ) const
	{
		//Set the shape transform
		renderer.pushTransform( getParent()->worldTransform() );
		
		switch( getShapeType() )
		{
		case Shape_Plane:
			{				
				//TODO:Drawing a plane shape
				break;
			}
		case Shape_Sphere:
			{
				renderer.drawSphere( m_s1 );
				break;
			}
		case Shape_Box:
			{
				renderer.drawBox( m_s1, m_s2, m_s3 );
				break;
			}
		default:
			{
				assert( false );
				break;
			}
		}

		renderer.popTransform();
	}

} //END: phys