/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\Gfx\Camera.h
 * Status: duplicate of kismo-v0.3/Gfx/Camera.h
 */
#ifndef GFX_CAMERA_H
#define	GFX_CAMERA_H

#include "framework/Common.h"
#include "Gfx/Spatial.h"
#include "Gfx/Viewport.h"

namespace gfx
{
	/** \brief Camera
		\remarks A camera can be assigned to multiple viewports
	*/
	class _DllExport Camera : public Spatial
	{
		DECLARE_RTTI;
	public:

		typedef frx::SmartPointer< Camera > Ptr;
	public:

		Camera( const String& name );
		virtual ~Camera();

		/** \brief Get the field of view of the camera
		*/
		Real getFieldOfView() const
		{ return m_fieldOfView; }

		/** \brief Set the cameras field of view
		*/
		void setFieldOfView( Real deg )
		{ m_fieldOfView = deg; }

		/** \brief get the position of the near clipping plane
		*/
		Real getNearPlane() const
		{ return m_nearClip; }

		/** \brief Set the distance to the near clip plane
		*/
		void setNearPlane( Real nearPlane )
		{ m_nearClip = nearPlane; }

		/** \brief get the position of the far clipping plane
		*/
		Real getFarPlane() const
		{ return m_farClip; }

		/** \brief Set the distance to the far clip plane
		*/
		void setFarPlane( Real farPlane )
		{ m_farClip = farPlane; }

		/** \brief Set the distance to the far clip plane
		*/
		Viewport* getViewport() const
		{ return m_viewport; }

		/** \brief Set the viewport the camera draws to
		*/
		void setViewport( Viewport* viewport );

	protected:

		/** \brief Updates the bounding volume
		*/
		virtual void updateWorldBound() 
		{}

		virtual void updateState( RenderStateStack& states, LightStack& lights );

		/** \brief Performs the actual drawing of this node
		*/
		virtual void doDraw( Renderer& renderer ) const;

	private:

		Real m_fieldOfView; //< The field of view of the camera
		Real m_nearClip; ///< The position of the near clip plane
		Real m_farClip; ///< The position of the far clip plane
		Viewport* m_viewport; //< The viewport the camera uses
	};

} // END: gfx

#endif