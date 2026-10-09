/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\gui\KismoWindow.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef _KISMO_GUI_WINDOW_H
#define	_KISMO_GUI_WINDOW_H

#include "gui\KismoWidget.h"

namespace kismo
{
	namespace gui
	{	
		class _KismoExport Window : public Widget
		{
			KISMO_DECLARE_RTTI;
		public:
			Window( Widget* parent = 0, GUIRenderer* renderer = 0 );
			virtual ~Window(){}

		protected:

				
		private:			

		};

	} // END: gui

} // END: kismo

#endif
