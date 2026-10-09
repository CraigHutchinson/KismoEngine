/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\RAGE_Demo\CameraControllerAction.h
 * Status: superseded by kismo-v0.3/Kismo_TestApp/CameraControllerAction.h (same name, content differs)
 */
#ifndef CAMERA_CONTROLLER_ACTION
#define CAMERA_CONTROLLER_ACTION

#include "Framework/EventManager.h"

// foreward declarations
namespace gfx { class Camera; }
namespace frx { class Window; }

/** \brief Defines a event action which hooks into the mouse events to control a camera wihtin the scene
*/
class CameraControllerAction : public frx::EventAction
{
public:

	/** \brief Camera controllerc onsstructor
		\param[in] window The window that has the mouse event to attach to
		\param[in] camera The camera that is to be controlled
	*/
	CameraControllerAction( frx::Window* window, gfx::Camera* camera );

	/** \brief The action callback which will occur when the mouse cursor is moved
	*/
	void exec( frx::Event* event, Real tDelta, Real tApp );

private:
	gfx::Camera* m_camera; ///< The camera that is being controlled

};

#endif