/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\KismoCore\_SOURCE_\KismoGL\Context.h
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

#ifndef KISMOGL_CONTEXT_H
#define KISMOGL_CONTEXT_H

#include "KismoGL/Common.h"
#include "KismoGL/IWindow.h"
#include "KismoGL/VertexBufferObject.h"

namespace kismo { namespace gl
{
	
	//Forward Declaration

	static const Index kMajorGLVersion = 3;
	static const Index kMinorGLVersion = 3;

	/** \brief GL Context
	*/
	class Context
	{
#if KISMO_PLATFORM == KISMO_PLATFORM_WINDOWS
#	include "Context_Win32_Members.h"
#else
#error Unsupported platform for GL::Context
#endif

	public:

		/** Create context
			\param[in] window Window from which to obtain a context, Window created automatically if null
			\remarks If the window alreayd has a context the requested GL version will be requested. If not
			supported (i.e. a null context returned) then 
		*/
		Context( IWindow* window = 0, Index majorGLVersion = kMajorGLVersion, Index minorGLVersion = kMinorGLVersion );
		~Context();

		IWindow* window() const
		{ return m_window; }

		VBO createVBO();

		
		ContextHandle getHandle() const
		{ return m_handle; }
		
		operator bool () const
		{ return getHandle() != 0; }

		/** Swap the front-back buffers to present the next frame
			\remarks --Platform specific Impl--
			\remarks By default this will wait for a vertical-sync (i.e. syncronize wiht monitor at 60Hz)
		*/
		bool present();

	private:
		//Functions with platform specific implementation//

		/** Creates a default GL rendering context from the current window
			\remarks --Platform specific Impl--
		*/
		bool createDefaultRenderDeviceContext( Index majorGLVersion = kMajorGLVersion, Index minorGLVersion = kMinorGLVersion );

		/** If the window doesn't have its own context then we should destroy it
		*/
		void destroyDefaultRenderDeviceContext();
	
	private:
		ContextHandle m_handle; //, The GL context handle
		IWindow* m_window;
		bool m_isWindowOwner; //< Whether the window is owned/managed by the context (void constructor)
	};

} /*END: gl */ } /*END: kismo */

#endif
