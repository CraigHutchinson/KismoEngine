/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\KismoCore\_SOURCE_\KismoGL\Context_Win32.cpp
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
#include "KismoFramework/Common.h"
#include "KismoFramework/String.h"
#include <gl\GL.h>

namespace kismo { namespace gl
{	
	
	bool Context::createDefaultRenderDeviceContext(Index majorGLVersion, Index minorGLVersion )
	{
		PIXELFORMATDESCRIPTOR pfd =				// pfd Tells Windows How We Want Things To Be
		{
			sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
			1,											// Version Number
			PFD_DRAW_TO_WINDOW |						// Format Must Support Window
			PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
			PFD_DOUBLEBUFFER,							// Must Support Double Buffering
			PFD_TYPE_RGBA,								// Request An RGBA Format
			32,										// Select Our Color Depth
			0, 0, 0, 0, 0, 0,							// Color Bits Ignored
			0,											// No Alpha Buffer
			0,											// Shift Bit Ignored
			0,											// No Accumulation Buffer
			0, 0, 0, 0,									// Accumulation Bits Ignored
			32,											// 16Bit Z-Buffer (Depth Buffer)  
			0,											// No Stencil Buffer
			0,											// No Auxiliary Buffer
			PFD_MAIN_PLANE,								// Main Drawing Layer
			0,											// Reserved
			0, 0, 0										// Layer Masks Ignored
		};
		
		if (!(m_handle.device=GetDC( m_window->getHandle() )))							// Did We Get A Device Context?
		{
			AssertEx(false,"Can't Create A GL Device Context.");
			return false;								// Return false
		}

		GLuint pixelFormat;			// Holds The Results After Searching For A Match
		if ( !(pixelFormat = ChoosePixelFormat(m_handle.device,&pfd)) )	// Did Windows Find A Matching Pixel Format?
		{
			AssertEx(false,"Can't Find A Suitable pixelFormat.");
			return false;								// Return false
		}

		if(!SetPixelFormat(m_handle.device,pixelFormat,&pfd))		// Are We Able To Set The Pixel Format?
		{
			AssertEx(false,"Can't Set The pixelFormat.");
			return false;								// Return false
		}

		if (!(m_handle.renderContext=wglCreateContext(m_handle.device)))				// Are We Able To Get A Rendering Context?
		{
			AssertEx(false,"Can't Create A GL Rendering Context." );
			return false;								// Return false
		}
		
		if ( !setGLRenderContextVersion( majorGLVersion, minorGLVersion ) )
		{	
			AssertEx(false,"Requested GL Version not supported!");
			return false;	
		}

		return true;
	}

	bool Context::present()
	{ 
		BOOL ret = SwapBuffers( m_handle.device ); 
		Assert( ret == TRUE );
		return ret == TRUE;
	}
	

} /*END: gl */ } /*END: kismo */
