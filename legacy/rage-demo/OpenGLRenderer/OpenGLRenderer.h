/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\OpenGLRenderer\OpenGLRenderer.h
 * Status: superseded by kismo-v0.3/OpenGLRenderer/OpenGLRenderer.h (same name, content differs)
 */
#ifndef OPENGL_RENDERER_H
#define OPENGL_RENDERER_H

#include "Platform/Platform.h"
#include "Gfx/Renderer.h"
#include "Gfx/RenderTarget.h"
#include "Gfx/WindowTarget.h"
#include "Gfx/Buffer.h"
#include "Gfx/Mesh.h"

#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <windows.h> //Need windows to use opengl!

namespace gfx
{
	/** \brief OpenGL implementation of gfx::Renderer
	*/
	class OpenGLRenderer : public gfx::Renderer
	{
	public:
		
		/** \brief \copydoc gfx::Renderer::createBuffer
		*/
		virtual gfx::Buffer::Ptr createBuffer( gfx::Buffer::ePurpose purpose, const gfx::BufferDefinition& definition, Size elementCount, const Byte* data = 0, gfx::Buffer::eUsage usage = gfx::Buffer::Usage_Default ) const;

		/** \brief OpenFGL renderer consturctur
		*/
		OpenGLRenderer();

		/** \brief \copydoc gfx::Renderer::~Renderer
		*/
		virtual ~OpenGLRenderer();

		/** \brief \copydoc gfx::Renderer::createTarget
		*/
		virtual gfx::RenderTarget* createTarget( const frx::Rtti& type, const String& name, Priority priority = 0 );

		/** \brief \copydoc gfx::Renderer::initialise
		*/
		virtual Bool initialise();
		
		/** \brief \copydoc gfx::Renderer::reinitialise
		*/
		virtual Bool reinitialise();
		
		/** \brief \copydoc gfx::Renderer::destroy
		*/
		virtual void destroy();

		/** \brief \copydoc gfx::Renderer::beginFrame
		*/
		virtual Bool beginFrame();

		/** \brief \copydoc gfx::Renderer::endFrame
		*/
		virtual void endFrame();	
	
		/** \brief Set the render target that will be rendered to
		*/
		virtual void setRenderTarget( gfx::RenderTarget* target );

		/** \brief Set the camera that will be used for setting up the projection matrix
		*/
		virtual void setCamera( gfx::Camera* camera );
	
		/** \brief Set the viewport which will be rendererd into
		*/
		virtual void setViewport( gfx::Viewport* viewport );

		virtual void draw( const gfx::Geometry* geometry );
		
		virtual void drawSphere( Real radius );
		virtual void drawBox( Real width, Real height, Real length );

		virtual void draw2DQuad( const frx::Rect2r& rect, const frx::Colour4& colour );

		/** \brief Set the current material to use when rendering
		*/
		virtual void setMaterialState( gfx::MaterialState* material );

		/** \brief Set the transform
		*/
		virtual void setTransform( const gfx::TransformR& transform, bool forward = true );

		/** \brief set the vertical sync of the current render context
		*/
		virtual void setVSync( UInt32 frameInterval );

		/** \brief Push the state attribute of the renderer
		*/
		virtual void pushRenderState();

		/** \brief Set the render mode
			\remarks Wireframe, solid, lighting, texturing etc
		*/
		virtual void setRenderState( gfx::RenderState* state );

		virtual void pushTransform( const gfx::TransformR& transform );
	
		/** \brief pop the transform state
		*/
		virtual void popTransform();


		/** \brief Pop the state attribute of the renderer
		*/
		virtual void popRenderState();
	protected:
		
		Bool setupPixelFormat();

	private:
				
		HDC m_hDC; ///< A handle to the device context
		HGLRC m_hRC; ///< A handle to the renering context

		Bool m_frameStarted; ///< Stores whether we are between the startFrame and endFrame calls
	};



}


#endif