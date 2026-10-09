/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\Gfx\RenderTarget.h
 * Status: superseded by kismo-v0.4/KismoGFX/RenderTarget.h (same name, content differs)
 */
#ifndef GFX_RENDERTARGET_H
#define	GFX_RENDERTARGET_H

/*
	RenderTarget can be derived for a window, texture, buffer etc...
*/

#include "Framework/Rect.h"
#include "Framework/Named.h"
#include "Framework/Rtti.h"
#include "Gfx/Viewport.h"

namespace gfx
{
	//Forward declarations
	class Renderer;
	class Node;

	/** \brief RenderTarget base type
	*/
	class _DllExport RenderTarget : public frx::Named
	{
		DECLARE_RTTI_BASE; ///< Impliment Rtti so the type or fender target  can be detected
	public:

		typedef frx::Rect2s::Size Size;

		/** \brief Render target base constructor
			\param[in] name Name used to identify the render target
			\param[in] priority The priority value where 0 is normal, Priority_MAX is maximum, 
				and Priority_MIN is minimum
		*/
		RenderTarget( const String& name, Priority priority = 0 );
		virtual ~RenderTarget();

		/** \brief Get the size fo the render target 
		*/
		const Size& getSize() const
		{ return m_size; }

		/** \brief Set the size fo the render target - virtual for specialisation
		*/
		virtual void setSize( const Size& size )
		{ m_size = size; }

		/** \brief gets the width of the render target 
		*/
		UInt16 getWidth() const 
		{ return m_size.width; }
		
		/** \brief gets the height of the render target 
		*/
		UInt16 getHeight() const 
		{ return m_size.height; }

		/** \brief Get the aspect ratio of the rneder target
		*/
		Real getAspect() const
		{ return (Real)m_size.width / (Real)m_size.height; }

		/** \brief gets the colour-depth of the render target 
		*/
		UInt8 getColourDepth() const
		{ return m_colourDepth; }

		/** \brief Force an update of the render target
		*/
		virtual Bool update() = 0;

		/** \brief Adds a viewport to the render target rendering from a specified camera.
			\param[in] zOrder Specifies the render order for overlapping viewports
			\param[in] rect Specifies the dimensions relative the target area with x = [0.0 to 1.0], y = [0.0 to 1.0]
			\remarks 
				Duplicate Z-orders are allowed but draw order will be undefined
		*/
		Viewport* addViewport( const String& name, Camera* camera, Int8 zOrder = 0, Viewport::Rect rect = Viewport::Rect(0,0,1,1) );

		/** \brief Returns the number of viewports on this render target 
		*/
		Count numberOfViewports() const
		{ return (Count)m_viewports.size(); }

		/** \brief Get a pointer to a viewport
			\remarks if the index is invalid 0 is returned
		*/
		Viewport* getViewport( Index index )
		{ assert( index < m_viewports.size() ); return m_viewports[ index ]; }

		/** \brief Removes a viewport at a given z-order
		*/
		void removeViewport( Int8 ZOrder )
		{}

		/** \brief Removes ALL viewports
		*/
		void removeAllViewports(void)
		{}

		
		/** \brief Gets the priority given to the updating and rendering of this surface
		*/
		Priority getPriority() const 
		{ return m_priority; }

		/** \brief Gets whether the viewport is enabled/visible
		*/
		Bool isEnabled() const 
		{ return m_enabled; }

		/** \brief Sets whether the viewport is enabled
		*/
		virtual void setEnabled( Bool enabled )
		{ m_enabled = enabled; }

		/** \brief Writes the render targets contents to file
			\remarks This could eb good for screen shots. A 'delayed' flag should write the frame asyncronous
			using a buffer to maintain a nice frame rate where possible
		*/
		//virtual void writeContentsToFile( const String& filename ) = 0;

		/** \brief Returns whether this target is the primary one which contains
			the main device context and is closed on Kismo Shutdown 
		*/
		//virtual Bool isPrimary() const;

		/** \brief Is the render target valid
		*/
		virtual Bool isValid() const = 0;

		/** \brief Draws a scene graph
		*/
		void drawScene( Renderer& renderer, Node* scene );

	protected:
				
		Priority m_priority; ///< The priority of the render target

		Size m_size; ///< The dimensions of the render target
		UInt8 m_colourDepth; ///< The number of bits per colour pixel
		Bool m_hasDepthBuffer; ///< Whether the target has a depth buffer

		Bool m_enabled;	///< Whether the target is enabled and should be drawn to
		
		Viewport::PtrArray m_viewports; ///< Vector of viewports

	};

} // END: gfx

#endif