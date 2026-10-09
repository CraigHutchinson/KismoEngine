/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoGFX\Window.h
 * Status: superseded by kismo-core/_SOURCE_/KismoGL/Window.h (same name, content differs)
 */
#ifndef KISMOGFX_WINDOW_H
#define	KISMOGFX_WINDOW_H

#include "KismoGFX/RenderTarget.h"
#include "KismoPlatform/Window.h"
#include "KismoFRX/Timer.h"
#include "KismoFRX/VectorN.h"

namespace kismo { namespace gfx
{
	//Forward declarations

	/**
		\brief Base window interface
		\remarks Derives from RenderTarget so the window can be used as a render target with a renderer
	*/
	class KISMOGFX_EXPORT Window : public RenderTarget, public kismo::platform::Window
	{
	public:

		/**
			\brief Defines the interface for receiving realRT callbacks
			\remarks False should be returned. If true is returned the inputEventHook wil not be passed onto the default implementation
		*/
		//struct KISMOGFX_EXPORT InputEventHook
		//{
		//	//virtual bool onDisplay( /*Window* window*/ ) { return true; }
		//	virtual bool onKeyboard( Window* pthis, bool down, eKey key, eKeyModifier modifiers, int x, int y) { return false; }
		//	virtual bool onMouse( Window* pthis, bool down, eButton buttonState, int x, int y) { return false; }
		//	virtual bool onMotion( Window* pthis, UInt32 buttonState, int x, int y, int rx, int ry ) { return false; }
		//};

			
		enum eContextSupport
		{
			ContextSupport_OpenGL, ///< Supports an OpenGL context
			ContextSupport_DirectX, ///< Supports an DirectX context
		};

	public:

		Window* create( eContextSupport contextSupport );
	
	//protected:	

		Window( eContextSupport contextSupport );
		virtual ~Window();

	public:

		///**
		//	\brief Adds a inputEventHook for the window to signal events etc
		//*/
		//bool addInputHook( InputEventHook* inputEventHook  )
		//{ 
		//	if ( !inputEventHook ) 
		//		return false;
		//	m_inputEventHooks.insert(inputEventHook);
		//	return true;
		//}

		///**
		//	\brief Removes a inputEventHook for the window to signal events etc
		//*/
		//bool removeInputHook( InputEventHook* inputEventHook  )
		//{ 
		//	if ( !inputEventHook ) return false;
		//	InputHookSet::iterator iFind = m_inputEventHooks.find(inputEventHook);
		//	if ( iFind == m_inputEventHooks.end() )
		//		return false;
		//	m_inputEventHooks.erase(iFind);
		//	return true;
		//}
		//
		virtual bool initialise() { return open();  }
		virtual void destroy() {  }

		virtual void bind() {   }
		virtual void unbind(){   }

		virtual bool setSize( const Size2s& size )
		{ 
			return platform::Window::setSize(size)
				&& RenderTarget::setSize(size); 
		}

		/**
			\brief Main render/update loop for the window
		*/
		//virtual bool update() = 0;
		//virtual bool show() = 0;
		//virtual bool hide() = 0;
		//virtual void present() = 0;
		//virtual void setFullscreen() = 0;
		//virtual bool setPosition( const Vector2i& pos ) = 0;
		//virtual bool setTitle( const String& title ) = 0;

		
		/**
			\brief Returns whether the window supports GL, DX or other context types for rendering
		*/
		//eContextSupport contextSupported() const
		//{ return m_contextSupport; }

	protected:

		////void onKeyboardEvent( bool down, eKey key, eKeyModifier modifiers, int x, int y);
		//void onMouseEvent(bool down, eButton buttonState, int x, int y) ;
		//void onMotionEvent( UInt32 buttonState, int x, int y, int rx = 0, int ry = 0);

	protected:

	private:
		//eContextSupport m_contextSupport; //, The type of renderer context supported
	};

} /* END: gfx*/ } /*END: Kismo*/

#endif
