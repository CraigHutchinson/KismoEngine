/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\Gfx\Renderer.cpp
 * Status: current (newest copy of this file name in the archive)
 */
#include "Renderer.h"
#include "Gfx/RenderTarget.h"
#include "Gfx/Node.h"
#include "Framework/Log.h"

IMPL_SINGLETON_EX( gfx::Renderer, true )

namespace gfx
{
	Renderer::Renderer( const String& name ) :
		Singleton(),
		Named( name ),
		m_activeRenderTarget( 0 ),
		m_activeViewport( 0 ),
		m_activeCamera( 0 )

	{
	}

	Renderer::~Renderer()
	{
		for ( RenderTargetMap::iterator iTarget = m_renderTargets.begin(); iTarget != m_renderTargets.end(); ++iTarget )
		{
			frx::safeDelete( iTarget->second );
		}
		m_renderTargets.clear();
	}

	
	void Renderer::makeCurrent()
	{	
		ms_instance = static_cast< gfx::Renderer* >( this );
	}

	Bool Renderer::addRenderTarget( RenderTarget* target )
	{
		//TODO: Finish this 

		//Insert the target into the Name lookup map
		m_renderTargets.insert( RenderTargetMap::value_type( target->getName(), target ) );

		//Insert the target into the priority lookup map
		m_prioritisedRenderTargets.insert( RenderTargetPriorityMap::value_type( target->getPriority(), target ) );

		return true;
	}


	void Renderer::drawScene( Node* scene )
	{
		PROFILE;

		if ( scene )
		{
			if ( m_prioritisedRenderTargets.empty() )
			{
				logError( "drawScene( Node* scene ) : m_prioritisedRenderTargets is empty" );
			}

			//TODO: Priority based rendering of ender targets
			for ( RenderTargetPriorityMap::const_iterator iTarget = m_prioritisedRenderTargets.begin(); iTarget != m_prioritisedRenderTargets.end(); ++iTarget )
			{
				assert( iTarget->second );

				//Draw the scene
				iTarget->second->drawScene( *this, scene );
		
				//TODO: Draw defered stuff

			}
		}
		else
		{
			logError( "drawScene( Node* scene ) : Scene is NUll!" );
		}

	}

} //END: gfx

