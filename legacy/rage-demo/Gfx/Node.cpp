/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\Gfx\Node.cpp
 * Status: duplicate of kismo-v0.3/Gfx/Node.cpp
 */
#include "Node.h"
#include "Geometry.h"

namespace gfx
{	
	IMPLEMENT_RTTI( _T("gfx"), Node, Spatial );

	Node::Node( const String& name ) :
		Spatial( name )
	{
	}

	Node::~Node()
	{
	}

	void Node::updateWorldData( Real tDelta, Real tApp )
	{
		Spatial::updateWorldData( tDelta, tApp );

		for ( const_iterator iChild = childBegin(); iChild != childEnd(); ++iChild )
		{
			assert( (*iChild) );
			////if ( (*iChild)->isDerived ( gfx::Geometry::TYPE ) )
			//	(*iChild)->localTransform().rotateY( tDelta * 10, gfx::eSpace_World );
			(*iChild)->updateGeometryState( tDelta, tApp, false );
		}
	} 

	void Node::updateWorldBound()
	{
		if ( !isWorldBoundCurrent() )
		{
			const_iterator iChild = childBegin();
			if ( iChild != childEnd() )
			{
				assert( (*iChild) );
				//worldBound() =$ iChild->worldBound();

				for ( ; iChild != childEnd(); ++iChild )
				{
					assert( (*iChild) );
					//worldBound()->grow( iChild->worldBound() );
				}
			}
		}
	}

	void Node::updateState( RenderStateStack& states, LightStack& lights )
	{		
		for ( const_iterator iChild = childBegin(); iChild != childEnd(); ++iChild )
		{
			assert( *iChild );
			(*iChild)->updateRenderState( states, lights );
		}
	}

	void Node::doDraw( Renderer& renderer ) const
	{
		for ( const_iterator iChild = childBegin(); iChild != childEnd(); ++iChild )
		{
			assert( *iChild );
			(*iChild)->doDraw( renderer );
		}
	}

} //END: gfx