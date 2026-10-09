/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoGFX\RenderTarget.cpp
 * Status: current (newest copy of this file name in the archive)
 */
#include "KismoGFX/Rendertarget.h"

namespace kismo { namespace gfx
{
	RenderTarget::RenderTarget()
	{}

	//bool RenderTarget::addRenderHook( RenderEventHook* callback  )
	//{ 
	//	if ( !callback ) 
	//		return false;
	//	m_renderHooks.insert(callback);
	//	return true;
	//}

	//bool RenderTarget::removeRenderHook( RenderEventHook* callback  )
	//{ 
	//	if ( !callback ) return false;
	//	RenderHookSet::iterator iFind = m_renderHooks.find(callback);
	//	if ( iFind == m_renderHooks.end() )
	//		return false;
	//	m_renderHooks.erase(iFind);
	//	return true;
	//}

	void RenderTarget::onResizeEvent(  const Size2s& size  ) 
	{ 
		m_size = size;
		//for ( RenderHookSet::const_iterator iEvent = m_renderHooks.begin(); iEvent != m_renderHooks.end(); ++iEvent ) \
		//	if ( (*iEvent)->onResize( this, m_size ) ) return /*true*/; 
	}

	void RenderTarget::onIdleEvent(void) 
	{
		//for ( RenderHookSet::const_iterator iEvent = m_renderHooks.begin(); iEvent != m_renderHooks.end(); ++iEvent ) \
		//	if ( (*iEvent)->onIdle( this ) ) return /*true*/; 
	}

} /* END: gfx*/ } /*END: Kismo*/