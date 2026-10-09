/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\Gfx\Camera.cpp
 * Status: duplicate of kismo-v0.3/Gfx/Camera.cpp
 */
#include "Camera.h"

namespace gfx
{	
	IMPLEMENT_RTTI( _T("gfx"), Camera, Spatial );

	Camera::Camera( const String& name ) :
		Spatial( name ),
		m_viewport( 0 ),
		m_fieldOfView( 45 ),
		m_farClip( 1000 ),
		m_nearClip( 1 )
	{
	}

	Camera::~Camera()
	{
		if ( m_viewport )
		{
			assert( m_viewport->getCamera() == this );
			m_viewport->setCamera( 0 );
		}
	}

	void Camera::setViewport( Viewport* viewport )
	{
		if ( m_viewport == viewport  )
			return;

		if ( m_viewport
			&& m_viewport->getCamera() == this )
		{
			Viewport* pviewport = m_viewport;
			m_viewport = 0;
			pviewport->setCamera( 0 );
		}

		m_viewport = viewport; 

		if ( m_viewport && m_viewport->getCamera() != this )
			m_viewport->setCamera( this );
	}


	void Camera::updateState( RenderStateStack& states, LightStack& lights )
	{
	}

	void Camera::doDraw( Renderer& renderer ) const
	{
	}

} //END: gfx