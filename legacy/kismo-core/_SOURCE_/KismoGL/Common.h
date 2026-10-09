/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\KismoCore\_SOURCE_\KismoGL\Common.h
 * Status: current (newest copy of this file name in the archive)
 */
#ifndef KISMOGL_COMMON_H
#define KISMOGL_COMMON_H

#include "KismoFramework/Types.h"


namespace kismo 
{
	/**
		\brief	Namespace containing OpenGL v3.0+ C++ wrapper
	*/
	namespace gl
	{	

	#if KISMO_PLATFORM == KISMO_PLATFORM_WINDOWS
		#define WIN32_LEAN_AND_MEAN  //Thin windows headers a bit
		#include <windows.h>
		#undef WIN32_LEAN_AND_MEAN //< Undefine as other code may want full windows
			
		typedef HWND WindowHandle;
		typedef HDC DeviceHandle;
		typedef HGLRC RenderContextHandle;

		struct ContextHandle
		{
			DeviceHandle device;
			RenderContextHandle renderContext;

			ContextHandle( int i=0 ) 
				: device(0)
				, renderContext()
			{}

			ContextHandle( DeviceHandle device, RenderContextHandle renderContext ) 
				: device(device)
				, renderContext(renderContext)
			{}

			operator bool () const
			{ return device != 0 && renderContext != 0; }
		};

	#elif KISMO_PLATFORM == KISMO_PLATFORM_LINUX
		#include <pthread.h> //, Unix/Linux pthreads
		
		//TODO:
		typedef void WindowHandle;
		typedef void DeviceHandle;
		typedef void RenderContextHandle;
		struct ContextHandle;

	#endif
	} /* END: gl */

} /*END: kismo*/

#endif