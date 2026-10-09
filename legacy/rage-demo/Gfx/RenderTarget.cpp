/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\Gfx\RenderTarget.cpp
 * Status: superseded by kismo-v0.4/KismoGFX/RenderTarget.cpp (same name, content differs)
 */
#include "RenderTarget.h"
#include "Gfx/Renderer.h"
#include "Gfx/Node.h"
#include "Framework/Log.h"

namespace gfx
{
	IMPLEMENT_RTTI_BASE( gfx, RenderTarget );


	RenderTarget::RenderTarget( const String& name, Priority priority ) : 
		Named( name ),
		m_priority( priority ),
		m_enabled( true )
	{}

	RenderTarget::~RenderTarget()
	{		
		//Free up the viewports
		for (Viewport::PtrArray::iterator iViewport = m_viewports.begin(); iViewport != m_viewports.end(); ++iViewport )
		{
			frx::safeDelete( *iViewport );
		}
		m_viewports.clear();
	}

	Viewport* RenderTarget::addViewport( const String& name, Camera* camera, Int8 zOrder, Viewport::Rect rect )
	{
		Viewport* viewport = new Viewport( name, camera, rect );
	
		//TODO: Sort the viewports by depth etc...
		m_viewports.push_back( viewport );
		

		return viewport;
	}

	void RenderTarget::drawScene( Renderer& renderer, Node* scene )
	{
		PROFILE;

		if ( !isEnabled() )
		{
			return;
		}
		

		//TODO: Set this as the current render context if needed.
		renderer.setRenderTarget( this );
		
		renderer.beginFrame();//TODO: Should be done at the renderer level?

		for ( Viewport::PtrArray::const_iterator iViewport = m_viewports.begin(); iViewport != m_viewports.end(); ++iViewport )
		{
			if ( !(*iViewport)->isEnabled() )
				continue; ///< Don't draw if the viewport is disabled
			
			//Set the viewport we are going to render into
			renderer.setViewport( (*iViewport) );

			if ( iViewport != m_viewports.begin() )
				renderer.clearViewport( frx::Colour4::Red );

			renderer.setCamera( (*iViewport)->getCamera() );

			//Render the scene graph
			scene->draw( renderer );			
		}
		
		renderer.endFrame();//TODO: Should be done at the renderer level?
	}


} //END: gfx