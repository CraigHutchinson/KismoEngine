/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\Gfx\Geometry.cpp
 * Status: current (newest copy of this file name in the archive)
 */
#include "Geometry.h"
#include "Light.h"

namespace gfx
{	
	IMPLEMENT_RTTI( _T("gfx"), Geometry, Spatial );

	Geometry::Geometry( const String& name ) :
		Spatial( name )
	{
	}

	Geometry::~Geometry()
	{
	}
	
	void Geometry::updateGeometryState( Real tDelta, Real tApp )
	{
	}

	void Geometry::updateModelBound()
	{
	}

	void Geometry::updateWorldBound()
	{
		//modelBound()->transformBy( getWorldTransform(), worldBound() );
	}
	
	void Geometry::updateState( RenderStateStack& states, LightStack& lights )
	{	
		//Get the top states of the stack and store them for when we need to render
		for ( RenderStateStack::const_iterator iState = states.begin(); iState != states.end(); ++iState )
		{
			if ( iState->second.empty() ) continue;
			m_renderStates[ iState->first ] = iState->second.top();
			assert( m_renderStates[ iState->first ] );
		}
		
		//Empty the current list
		m_lights.clear();
		//Store a pointer to all the lights for this geometry node
		for ( LightStack::const_iterator iLight = lights.begin(); iLight != lights.end(); ++iLight ) 
		{
			m_lights.push_back( (Light::Ptr)(*iLight) );
		}
	}

} //END: gfx