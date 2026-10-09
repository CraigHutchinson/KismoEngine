/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\RAGE_Demo\DeformerControllerAction.h
 * Status: superseded by kismo-v0.3/Kismo_TestApp/DeformerControllerAction.h (same name, content differs)
 */
#ifndef DEFORMER_CONTROLLER_ACTION
#define DEFORMER_CONTROLLER_ACTION

#include "Framework/EventManager.h"
#include "Gfx/GeometryDeformer.h"
#include "Gfx/Geometry.h"

// foreward declarations
namespace gfx { class Camera; }
namespace frx { class Window; }

/** \brief Defines a event action which hooks into the mouse events to control a camera wihtin the scene
*/
class DeformerControllerAction : public frx::EventAction
{
public:

	/** \brief Camera controllerc onsstructor
		\param[in] window The window that has the mouse event to attach to
		\param[in] camera The camera that is used to construct the deformation orientation
		\param[in] geometry The geometry that the deformer should be applied to
	*/
	DeformerControllerAction( frx::Window* window, gfx::Camera* camera, gfx::Geometry* geometry );

	/** \brief destructor */
	~DeformerControllerAction();

	/** \brief The action callback which will occur when the mouse cursor is moved
	*/
	void exec( frx::Event* event, Real tDelta, Real tApp );
	
	/** \brief Update event must be called every frame to update the visual buffers with completed deformers
	*/
	void update( Real tDelta, Real tApp );

	/** terminate the processing
	*/
	void terminate();

private:
	bool m_on; //< Whether the deformer is activated
	gfx::Camera* m_camera; ///< The camera that is used to get the orientation and location of a deform
	gfx::Geometry* m_geometry; ///< the geometry object we are deforming
	gfx::Deformer::PtrArray m_deformers; ///< Vector of all the deformers that have been created
	gfx::Deformer::PtrLFQueue m_deformerResultQueue; ///< The queue which is propagated with completed queries when using the threaded model
	frx::Thread::Ptr m_deformerThread; ///< The thread that is being used to attach queries to
};

#endif