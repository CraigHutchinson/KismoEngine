/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\Gfx\Renderer.h
 * Status: superseded by kismo-v0.4/KismoGFX/Renderer.h (same name, content differs)
 */
#ifndef GFX_RENDERER_H
#define	GFX_RENDERER_H

//#include "gfx/TextureManager.h"
#include "Framework/Named.h"
#include "Framework/Rtti.h"
#include "Framework/Rect.h"
#include "Framework/Colour.h"
#include "Framework/Singleton.h"
#include "Gfx/Buffer.h"
#include "Gfx/Transform.h"

#define _Renderer gfx::Renderer::getSingleton()

/** \brief Graphics namespace contains all graphic/renderer classes
*/
namespace gfx
{
	//Forward declaration
	class RenderWindow;
	class RenderTarget;
	class Node;
	class Geometry;
	class Camera;
	class Viewport;
	class MaterialState;
	class RenderState;

	/** \brief Renderer base class
	\remarks 
		The renderer is singleton but allows multiple instances.
		The last created instance will become the singleton/active instance. By by calling 
		'makeCurrent()' the instance can be changed
	*/
	class _DllExport Renderer : public frx::Named, public frx::Singleton< Renderer, true >
	{		
	public:
		SINGLETON( Renderer );

		/** \brief Makes this renderer instance the current Singleton object which will be obtained through 
			'getInstance()'
		*/
		void makeCurrent();

		/** \brief Get a new buffer instance
		*/
		virtual Buffer::Ptr createBuffer( Buffer::ePurpose purpose, const BufferDefinition& definition, Size elementCount, const Byte* data = 0, Buffer::eUsage usage = Buffer::Usage_Default ) const = 0;

		/** \brief Renderer constructor
		\param[in] name The name of the renderer used to identify its type
		*/
		Renderer( const String& name );

		/** \brief Virtual destructor
		*/
		virtual ~Renderer();

		//Gets a list of all available options and their current values for the renderer
		//TODO: Option list
		//virtual void getConfigOptions() = 0;

		//Sets a configurable option for this renderer
		//virtual void setConfigOption(const String& name, const String& value) = 0;

		/** \brief Create a render target of the specified type
		*/
		virtual RenderTarget* createTarget( const frx::Rtti& type, const String& name, Priority priority = 0 ) = 0;

		/** \brief Initialises & creates the device
			\remarks Uses the first RenderTarget of type 'WindowTarget' to create the device.
				This device target will therefore be classed primary and should not be destroid
		*/
		virtual Bool initialise() = 0;
		
		/** \brief Reinitialise the renderer if config has changed
		*/
		virtual Bool reinitialise() = 0;
		
		/** \brief Deletes the device instance freeing memory
		*/
		virtual void destroy() = 0;

		/** \brief Add a new render target for the renderers use 
		*/
		Bool addRenderTarget( RenderTarget* target );

		//virtual Void setAmbientLight( Float32 r, Float32 g, Float32 b );
		//virtual Void setLightingEnabled( Bool enabled );
		
		/** \brief Begins the frame - clears the screen
		*/
		virtual Bool beginFrame() = 0;

		/** \brief Ends the frame & presents the image to the screen
		*/
		virtual void endFrame() = 0;	
		
		/** \brief Check if the renderer is initialised and is worth updating
			\returns True if initialised
		*/
		Bool initialised()
		{ return m_initialised; }
		
		/** \brief Set the render target that will be rendered to
		*/
		virtual void setRenderTarget( RenderTarget* target ) = 0;

		/** \brief Set the camera that will be used for setting up the projection matrix
		*/
		virtual void setCamera( Camera* camera ) = 0;
	
		/** \brief Set the viewport which will be rendererd into
		*/
		virtual void setViewport( Viewport* viewport ) = 0;

		/** \brief Draws a scene graph
		*/
		void drawScene( Node* scene );

		/** \brief Takes an array of buffers and binds them as the current rendering source
			and then calls the drawBuffers() implimentation.
		*/
		virtual void draw( const gfx::Geometry* geometry ) = 0;

		/** \brief Draw a sphere
		*/
		virtual void drawSphere( Real radius ) = 0;
		
		/** \brief Draw a box
		*/
		virtual void drawBox( Real width, Real height, Real length ) = 0;

		/** \brief Set the current material to use when rendering
		*/
		virtual void setMaterialState( gfx::MaterialState* material ) = 0;

		/** \brief Draws a 2D quad over the specified viewport coordinates in the specified colour
		*/
		virtual void draw2DQuad( const frx::Rect2r& rect, const frx::Colour4& colour ) = 0;

		/** \brief Clears the active viewport by drawing a quad over the surface
		*/
		void clearViewport( const frx::Colour4& colour )
		{ draw2DQuad( frx::Rect2r( 0, 0, 1, 1 ), colour ); }
		
		/** \brief Set the transform
		*/
		virtual void setTransform( const gfx::TransformR& transform, bool forward = true ) = 0;

		/** \brief set the vertical sync of the current render context
		*/
		virtual void setVSync( UInt32 frameInterval ) = 0;

		/** \brief Push the state attribute of the renderer
		*/
		virtual void pushRenderState() = 0;

		/** \brief Set the render mode
			\remarks Wireframe, solid, lighting, texturing etc
		*/
		virtual void setRenderState( RenderState* state ) = 0;

		/** \brief Push the transform state
		*/
		virtual void pushTransform( const gfx::TransformR& transform ) = 0;
	
		/** \brief pop the transform state
		*/
		virtual void popTransform() = 0;

		/** \brief Pop the state attribute of the renderer
		*/
		virtual void popRenderState() = 0;

	protected:

		typedef Map< String, RenderTarget* > RenderTargetMap;
		RenderTargetMap m_renderTargets; ///< The render targets

		typedef Map< Priority, RenderTarget* > RenderTargetPriorityMap;
		RenderTargetPriorityMap m_prioritisedRenderTargets; ///< The render targets ordered by priority
		
		RenderTarget* m_activeRenderTarget; ///< The active render target
		Viewport* m_activeViewport; ///< The active viewport
		Camera* m_activeCamera; ///< The active camera


	private:
		Bool m_initialised; ///< Whether the renderer is initialised
	};

} // END: gfx

#endif