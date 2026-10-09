/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoGFX\RenderTarget.h
 * Status: current (newest copy of this file name in the archive)
 */
#ifndef KISMOGFX_RENDERTARGET_H
#define KISMOGFX_RENDERTARGET_H

#include "KismoGFX/Common.h"
#include "KismoFRX/SizeN.h"

namespace kismo { namespace gfx
{
	/**
		\brief base interface for an output target for a renderer
		\remarks Derives to Window for Window support and IRenderBuffer for GBuffer support
	*/
	class KISMOGFX_EXPORT RenderTarget
	{
	public:

		/**
			\brief Defines the interface for receiving kismoGFX callbacks
			\returns Whether the message has been processed. If it has the callback will not be passed onto the default implementation.
			\remarks It is recommended that you return False
		*/
		struct KISMOGFX_EXPORT RenderEventHook
		{
			virtual bool onResize( RenderTarget* pthis, const Size2s& size ) { return false; }
			virtual bool onIdle( RenderTarget* pthis ) { return false; }		
		};
	
	public:

		virtual bool initialise() = 0;
		virtual void destroy() = 0;

		virtual void bind() = 0;
		virtual void unbind() = 0;
		virtual bool setSize( const Size2s& size ) = 0 
		{ 
			m_size = size; 
			onResizeEvent(size);
			return true; 
		}

		/**
			\brief Adds a callback for the window to signal events etc
		*/
		//bool addRenderHook( RenderEventHook* callback  );

		/**
			\brief Removes a callback for the window to signal events etc
		*/
		//bool removeRenderHook( RenderEventHook* callback  );

		UInt16 width() const
		{ return m_size.w; }
		
		UInt16 height() const
		{ return m_size.h; }
	
	protected:
		RenderTarget();

		void onResizeEvent( const Size2s& size );
		void onIdleEvent();

	protected:

		Size2s m_size; ///< the size of the window

	private:		

		//typedef std::set<RenderEventHook*> RenderHookSet;
		//RenderHookSet m_renderHooks; ///< The callback receiver
	};

} /* END: gfx*/ } /*END: Kismo*/

#endif