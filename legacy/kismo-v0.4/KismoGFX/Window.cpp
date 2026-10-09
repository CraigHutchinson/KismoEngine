/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoGFX\Window.cpp
 * Status: current (newest copy of this file name in the archive)
 */
#include "KismoGFX/Window.h"

namespace kismo { namespace gfx
{
	Window::Window( eContextSupport contextSupport ) : 
		RenderTarget(),
			platform::Window( platform::Window::Context_OpenGL/*contextSupport*/ )
		//m_contextSupport( contextSupport ),
	{
		m_timer.start();
	}

	Window::~Window() 
	{
	}

	//void Window::onKeyboardEvent( bool down, eKey key, eKeyModifier modifiers, int x, int y)
	//{ 
	//	//for ( InputHookSet::const_iterator iEvent = m_inputEventHooks.begin(); iEvent != m_inputEventHooks.end(); ++iEvent ) \
	//	//	if ( (*iEvent)->onKeyboard( this, down, key, modifiers, x, y ) ) return /*true*/; 
	//}

	//void Window::onMouseEvent(bool down, eButton buttonState, int x, int y) 
	//{ 
	//	//for ( InputHookSet::const_iterator iEvent = m_inputEventHooks.begin(); iEvent != m_inputEventHooks.end(); ++iEvent ) \
	//	//	if ( (*iEvent)->onMouse( this, down, buttonState, x, y ) ) return /*true*/; 
	//}

	//void Window::onMotionEvent( UInt32 buttonState, int x, int y, int rx, int ry ) 
	//{ 			
	//	//for ( InputHookSet::const_iterator iEvent = m_inputEventHooks.begin(); iEvent != m_inputEventHooks.end(); ++iEvent ) \
	//	//	if ( (*iEvent)->onMotion( this, buttonState, x, y, rx, ry ) ) return /*true*/; 
	//}

	//Window inputEventHook functions sends event data to a inputEventHook listener
	//void onDisplayEvent(void) 
	//{ 
	//	for ( InputHookSet::const_iterator iEvent = m_inputEventHooks.begin(); iEvent != m_inputEventHooks.end(); ++iEvent ) \
	//		if ( (*iEvent)->onDisplay( /*this*/ ) );
	//}


} /* END: gfx*/ } /*END: Kismo*/
