/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\KismoCore\_SOURCE_\KismoGL\Window.h
 * Status: current (newest copy of this file name in the archive)
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

#ifndef KISMOGL_WINDOW_H
#define KISMOGL_WINDOW_H

#include "KismoGL/IWindow.h"

namespace kismo { namespace gl
{
	
	//Forward Declaration

	class Window : public IWindow
	{
#if KISMO_PLATFORM == KISMO_PLATFORM_WINDOWS
#	include "Window_Win32_Members.h"
#else
#error Unsupported platform for GL::Window
#endif

	public:
		Window();
		~Window();

		ContextHandle getContext();

		WindowHandle getHandle() const
		{ return m_handle; }

	private:
		
		
		bool createWindowClass();

		bool createWindow();

		void destroyWindow();
		
		void destroyWindowClass();
		
		bool update();		

	protected:
		WindowHandle m_handle;
	};


} /*END: gl */ } /*END: kismo */

#endif
