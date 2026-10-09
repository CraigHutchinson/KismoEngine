/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\Gfx\Viewport.cpp
 * Status: duplicate of kismo-v0.3/Gfx/Viewport.cpp
 */
#include "Viewport.h"
#include "Gfx/Camera.h"

namespace gfx
{
	Viewport::Viewport( const String& name, Camera* camera, const Rect& rect ) :
		frx::Object( name ),
		m_enabled( true ),
		m_camera( camera ),
		m_rect( rect )
	{
		assert( m_camera->getViewport() == 0 ); //< Should not leave errors about!
		m_camera->setViewport( this ); ///Add a reference in the camera to this viewport
	}

	Viewport::~Viewport()
	{
		if ( m_camera )
		{
			assert( m_camera->getViewport() == this );
			setCamera( 0 );
		}
	}

	void Viewport::setCamera( Camera* camera )
	{
		if ( m_camera == camera )
			return;

		if ( m_camera
			&& m_camera->getViewport() == this ) 
		{
			Camera* pcamera = m_camera;
			m_camera = 0;
			pcamera->setViewport( 0 );
		}

		m_camera = camera; 

		if ( m_camera && m_camera->getViewport() != this )
		{
			m_camera->setViewport( this );
		}
	}


} //END: gfx