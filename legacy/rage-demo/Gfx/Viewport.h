/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\Gfx\Viewport.h
 * Status: superseded by kismo-v0.3/Gfx/Viewport.h (same name, content differs)
 */
#ifndef GFX_VIEWPORT_H
#define	GFX_VIEWPORT_H

#include "framework/Common.h"
#include "Framework/Rect.h"
#include "Framework/Object.h"

namespace gfx
{
	//Forward declaration
	class Camera;
	class RenderTarget;

	/** \brief A viewport with 1 camera and 1 render target
		\remarks Is an object so we can find and reference it as needed
	*/
	class _DllExport Viewport : public frx::Object
	{
		friend class RenderTarget; ///< The render target is allowed to create instances only
	public:

		typedef Vector< Viewport* > PtrArray;
		typedef frx::Rect2< Real > Rect;

		//Friend functions for depth sorting viewports using the STL
		friend const bool sortGreater( Viewport* a, Viewport* b ) 
		{ return a->m_zOrder > b->m_zOrder; }

		friend const bool sortLess( Viewport* a, Viewport* b ) 
		{ return a->m_zOrder < b->m_zOrder; }

	protected:

		/** \brief Protected constructor can only be called by a render target. i.e. target->addViewport( ... )
		*/
		Viewport( const String& name, Camera* camera, const Rect& rect );

	public:

		~Viewport();

		/** \brief Set the rectangle of the viewport
		*/
		void setRect( const Rect& rect )
		{ m_rect = rect; }

		/** \brief Get the rect of the viewport
		*/
		const Rect& getRect() const
		{ return m_rect; }

		/** \brief Set the camera assigned to the viewport
		*/
		void setCamera( Camera* camera );

		/** \brief get the camera assigned ot the viewport
		*/
		Camera* getCamera() const
		{ return m_camera; }
		
		/** \brief Gets whether the viewport is enabled/visible
		*/
		Bool isEnabled() const 
		{ return m_enabled; }

		/** \brief Sets whether the viewport is enabled
		*/
		virtual void setEnabled( Bool enabled )
		{ m_enabled = enabled; }

	protected:
		Camera* m_camera; ///< The camera that is assigned to the viewport
		Rect m_rect; ///< The rect assigned to the viewport
		Bool m_enabled;	///< Whether the viewport is enabled and should be drawn to
		Int8 m_zOrder; ///< The z order of the viewport
	};

} // END: gfx


#endif