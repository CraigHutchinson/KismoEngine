/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\Gfx\Spatial.cpp
 * Status: superseded by kismo-v0.3/Gfx/Spatial.cpp (same name, content differs)
 */
#include "Spatial.h"
#include "framework/CoreMath.h"

namespace gfx
{	
	IMPLEMENT_RTTI( gfx, Spatial, Object );

	Spatial::Spatial( const String& name ) :
		Object( name ),
		m_worldIsCurrent( false )
	{
	}

	Spatial::~Spatial()
	{}
	
	void Spatial::updateGeometryState( Real tDelta, Real tApp, Bool initiator )
	{
		updateWorldData( tDelta, tApp );
		updateWorldBound();
		if ( initiator )
			propagateBoundToRoot();
	}

	void Spatial::updateBoundState()
	{
	}

	
	gfx::Spatial* Spatial::getChildOfType( const frx::Rtti& type )
	{
		for ( const_iterator iChild = childBegin(); iChild != childEnd(); ++iChild )
		{
			if ( (*iChild)->isDerived( type ) )
				return *iChild;
		}
		return 0;
	}

	void Spatial::updateWorldData( Real tDelta, Real tApp )
	{
		//update stuff

		if ( !m_worldIsCurrent )
		{ 
			if ( getParent() )
			{
				m_worldTransform.product( getParent()->worldTransform(), m_localTransform );
			}
			else
			{
				m_worldTransform = m_localTransform;
			}
		}

	}
 
	void Spatial::propagateBoundToRoot()
	{
		if ( getParent() )
		{
			getParent()->updateWorldBound();
			getParent()->propagateBoundToRoot();
		}
	}


	void Spatial::setRenderState( RenderState* state )
	{
		m_renderStates[ &state->getType() ] = state;
	}

	void Spatial::updateRenderState()
	{
		RenderStateStack states;
		LightStack lights;

		//get the state from the parent nodes
		propagateRenderStateFromRoot( states, lights, true );

		DEBUG_ONLY( Count stateCount = (Count)states.size() );
		DEBUG_ONLY( Count lightCount = (Count)lights.size() );

		//Call the update state implementation
		updateRenderState( states, lights );

		//assert( stateCount == states.size() );
		assert( lightCount == lights.size() );
	}

	void Spatial::updateRenderState( RenderStateStack& states, LightStack& lights )
	{		
		/// Push this nodes state onto the stacks
		pushState( states, lights );

		//Propagate the state to the children
		updateState( states, lights );

		/// Pop this nodes state from the stacks
		popState( states, lights );
	}

	void Spatial::propagateRenderStateFromRoot( RenderStateStack& states, LightStack& lights, bool initiator )
	{
		if ( getParent() )
			getParent()->propagateRenderStateFromRoot( states, lights );

		if (!initiator)
			pushState( states, lights ); //Push the state of this node onto the stack
	}

	void Spatial::pushState( RenderStateStack& states, LightStack& lights )
	{
		//Add all the states for this node to the stacks
		for ( RenderStateMap::const_iterator iState = m_renderStates.begin(); iState != m_renderStates.end(); ++iState )
		{
			states[ iState->first ].push( iState->second );
		}
		
		//Add all the local lights to the light stack
		for ( LightArray::const_iterator iLight = m_lights.begin(); iLight != m_lights.end(); ++iLight )
		{
			lights.push_back( *iLight );
		}
	}

	void Spatial::popState( RenderStateStack& states, LightStack& lights )
	{
		//remove all the states for this node from the state stack
		for ( RenderStateMap::const_iterator iState = m_renderStates.begin(); iState != m_renderStates.end(); ++iState )
		{			
			RenderStateStack::mapped_type& stateStack = states[ iState->first ];
			if ( stateStack.empty() ) continue;
			stateStack.pop();
		}
		
		//remove all the local lights from the light stack
		Count count = (Count)m_lights.size();
		while ( count && --count )
			lights.pop_back();
	}

	void Spatial::draw( Renderer& renderer )
	{
		//Camera* cam....
		// Culling etc
		//if ( !culled )
			doDraw( renderer ); //< Draw the node
	}

	bool Spatial::setAttributes( const AttributeMap& attributes )
	{
		AttributeMap::const_iterator iFind;
				
		if ( (iFind = attributes.find( _T("pos") )) != attributes.end() )
			m_localTransform.setTranslate( iFind->second );

		if ( (iFind = attributes.find( _T("pivot") )) != attributes.end() )
			m_localTransform.setPivot( iFind->second );
		
		if ( (iFind = attributes.find( _T("euler") )) != attributes.end() )
		{
			frx::Vector3r euler = iFind->second; //Construct the rotation vector fo the euler angles
			m_localTransform.rotate( TransformR::Matrix_T::rotation( euler.x, euler.y, euler.z ), gfx::eSpace_World );
		}

		return true;
	}

} //END: gfx