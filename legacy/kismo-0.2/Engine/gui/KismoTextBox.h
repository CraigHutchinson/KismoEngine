/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\gui\KismoTextBox.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef _KISMO_GUI_TEXTBOX_H
#define	_KISMO_GUI_TEXTBOX_H

#include "KismoWidget.h"

namespace kismo
{
	namespace gui
	{	
		class _KismoExport TextBox : public Widget
		{
		public:
			TextBox( Widget* parent = 0 );
			~TextBox();

		protected:

			virtual void setDefaultDefinition() {}; //sets the default definition for the widget

		};

	} // END: gui

} // END: kismo

#endif
