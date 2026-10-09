/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\KismoCore\_SOURCE_\KismoGL\IWindow.h
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

#ifndef KISMOGL_IWINDOW_H
#define KISMOGL_IWINDOW_H

#include "KismoGL/Common.h"

namespace kismo { namespace gl
{
	
	//Forward Declaration

	/**
		\brief Window interface for GL/GUI contexts
	*/
	class IWindow
	{
	public:
		virtual ~IWindow(){}

		/** Get the GL context handle if this window implementation owns one already (i.e. QtGLWidget)
		*/
		virtual ContextHandle getContext() = 0;	

		/** Gets a window handle form whcih a context could be created
		*/
		virtual WindowHandle getHandle() const = 0;
		
		virtual bool update() = 0;

		operator bool () const
		{ return getHandle() != 0; }
	};

} /*END: gl */ } /*END: kismo */

#endif
