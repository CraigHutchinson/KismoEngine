/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\Gfx\WindowTarget.h
 * Status: current (newest copy of this file name in the archive)
 */
#ifndef GFX_WINDOWTARGET_H
#define GFX_WINDOWTARGET_H

#include "Framework/Window.h"
#include "Framework/EventManager.h"
#include "Gfx/RenderTarget.h"

namespace gfx
{
	/** \brief IWindow render target
		\remarks Specialises RenderTarget to allow rendring to a window
	*/
	class _DllExport IWindowTarget : public RenderTarget, public frx::EventAction
	{
		DECLARE_RTTI;
	public:

		/** \brief COnstructor
			\param[in] name The name used to identify the render target at a later time
			\param[in] priority The rendering priority - not used
		*/
		IWindowTarget( const String& name, Priority priority = 0  ) :
			RenderTarget( name, priority ),
			frx::EventAction( false ),
			m_window( 0 )
		{}

		/** \brief Sets the window this render target will use
			\remark
				This can only be called once!
		*/
		Bool setWindow( frx::IWindow* window )
		{
			if ( isValid() || !frx::isValidPointer( window ) )
				return false;
			else
			{
				m_size = window->getRect().getSize();
				m_window = window;

				//Register that we want to get resize events
				m_window->onResize().registerAction( this );
			}
			return true;
		}
		
		/** \brief Set the size fo the render target - virtual for specialisation
		*/
		virtual void setSize( const Size& size )
		{
			RenderTarget::setSize( size );
			m_window->setSize( size );
		}

		/** \brief \copydoc frx::IWindow::isValid
		*/
		virtual Bool isValid() const 
		{ return frx::isValidPointer( m_window ); }

		frx::IWindow* getWindow() const
		{ return m_window; }

		void exec( frx::Event* event, Real tDelta, Real tApp )
		{
			if ( event->isExactly( frx::ResizeEvent::TYPE ) )
			{
				frx::ResizeEvent* resizeEvent = (frx::ResizeEvent*)event;
				if ( resizeEvent->size != m_window->getRect().getSize() )
					RenderTarget::setSize( resizeEvent->size ); ///< If the size is different we set the render target size
			}
		}


	private:
		frx::IWindow* m_window; ///< The render windows target surface
	};


} //END: gfx

#endif