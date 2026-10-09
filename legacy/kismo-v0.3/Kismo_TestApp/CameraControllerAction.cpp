/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\Kismo_TestApp\CameraControllerAction.cpp
 * Status: current (newest copy of this file name in the archive)
 */
#include "CameraControllerAction.h"
#include "Framework/Window.h"
#include "Gfx/Camera.h"

CameraControllerAction::CameraControllerAction( frx::IWindow* window, gfx::Camera* camera ) :
	EventAction( false ),
	m_camera( camera )
{
	assert( m_camera );
	window->onMouse().registerAction( this );
}

void CameraControllerAction::exec( frx::Event* event, Real tDelta, Real tApp )
{
	if ( event->getType() == frx::MouseEvent::TYPE )
	{
		frx::MouseEvent* mouse = (frx::MouseEvent*)event;
		
		math::Vector3 trans( 
			mouse->relativePos.x * (Real)0.05, 
			mouse->relativePos.y * (Real)0.05, 0 );

		math::clamp( trans.x, (Real)-100, (Real)100 );
		math::clamp( trans.y, (Real)-100, (Real)100 );

		if ( mouse->button[0] )
		{
			m_camera->localTransform().rotateY( trans.x * -5 , gfx::eSpace_World );
			m_camera->localTransform().rotateX( trans.y * -5 , gfx::eSpace_Local );
		}
		if ( mouse->button[1] )
			m_camera->localTransform().translate( math::Vector3( trans.x, -trans.y, 0 ), gfx::eSpace_InverseLocal );
		if ( mouse->button[2] )
			m_camera->localTransform().translate( math::Vector3( 0, 0, -trans.y ), gfx::eSpace_InverseLocal );
	}
}