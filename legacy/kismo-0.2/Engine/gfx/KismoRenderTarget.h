/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\gfx\KismoRenderTarget.h
 * Status: unique (no other copy of this file name in the archive)
 */
//Base render target class//
#ifndef _KISMO_RENDER_TARGET_H
#define	_KISMO_RENDER_TARGET_H

/*
	RenderTarget can be derived for a window, texture, buffer etc...
*/

#include "gfx\KismoViewport.h"
#include "framework\KismoRect.h"

namespace kismo
{
	namespace gfx
	{
		//-----------------------------------------------------
		// RenderTarget type
		//-----------------------------------------------------
		class _KismoExport RenderTarget
		{
		public:
			RenderTarget( const k_String& name ) : m_name( name ) {}
			virtual ~RenderTarget(){};

			virtual const k_String& getName() const{ return m_name;} //Get the name of the target

			//Get the render targets dimensions and colour depth
			virtual void getMetrics( k_UInt32& width, k_UInt32& height, k_UInt32& colourDepth){}
			virtual k_UInt32 width() const{return 0;}
			virtual k_UInt32 height() const{return 0;}
			virtual k_UInt32 colourDepth() const{return 0;}

			//Force a redraw of the render target
			virtual void update(){};

			//Adds a viewport to the render target rendering from a specified camera.
			//	- Dimensions are relative as in x = [0.0 to 1.0], y = [0.0 to 1.0]
			//	- ZOrder specifies the render order for overlapping viewports
			// *Defaults to full screen
			// *Duplicate Z-orders are not allowed 
			virtual Viewport* addViewport( Camera* cam, k_Int32 ZOrder = 0, frx::Rect2r rect = frx::Rect2r(0,0,1,1) ){return 0;}

			virtual k_Count numberOfViewports() const{return 0;} //Returns the number of viewports
			virtual Viewport* getViewport( k_Index index){return 0;} //Get s pointer to a viewport
			virtual void removeViewport( k_Int32 ZOrder ){} //Removes a viewports at a given z-order
			virtual void removeAllViewports(void){} //Removes ALL viewports


			virtual void setPriority( k_Priority priority ) { m_priority = priority; }
			virtual k_Priority getPriority() const { return m_priority; }

			virtual bool isEnabled() const; //Gets whether the viewport is enabled/visible
			virtual void setEnabled( bool b ); //Sets whether the viewport is enabled

			//Writes the render targets contents to file
			virtual void writeContentsToFile( const k_String& filename ) = 0;

			//Returns whether this window is the primary one which contains
			//	the main device context and is closed on Kismo Shutdown
			virtual bool isPrimary(void) const;

		protected:
			
			k_String m_name; //The name of this render target			
			k_Priority m_priority; //The priority of the render target

			k_UInt32 m_width;
			k_UInt32 m_height;
			k_UInt32 m_colourDepth;
			bool m_hasDepthBuffer;

			bool m_enabled;	
			
			Viewport::VECTOR m_viewports; //Vector of viewports

		};

	} // END: gfx

} // END: kismo


#endif