/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\KismoCore\_SOURCE_\KismoGL\Context.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
/*^**************************************************************************
*	
*	Kismo Engine 
*	© 2008-2013 Craig Hutchinson (craig@craighutchinson.co.uk)
*	All right reserved
*
*	This file is subject to the terms and conditions defined in
*	file 'license.txt', which is part of this source code package.
*
***************************************************************************^*/


#include "Context.h"
#include "Window.h"
#include "KismoFramework\Common.h"

namespace kismo { namespace gl
{
	Context::Context( IWindow* window, Index majorGLVersion, Index minorGLVersion )
	{
		if ( !window )
		{
			m_window = new Window();
			m_isWindowOwner = true;
			AssertEx( m_window->getHandle(), "Failed to create Window!" );
		}
			
		//Get an existing context from the window
		// - If one diesn't exist then we create a default context
		m_handle = m_window->getContext();	

		if ( !m_handle )
			m_handle = createDefaultRenderDeviceContext( majorGLVersion, minorGLVersion );
	}

	Context::~Context()
	{
	}
	
	VBO Context::createVBO()
	{ return VBO(); }


} /*END: gl */ } /*END: kismo */