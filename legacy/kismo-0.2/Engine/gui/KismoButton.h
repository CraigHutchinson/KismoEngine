/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\gui\KismoButton.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef _KISMO_GUI_BUTTON_H
#define	_KISMO_GUI_BUTTON_H

#include "gui\KismoWidget.h"

namespace kismo
{
	namespace gui
	{	
		class _KismoExport Button : public Widget
		{
		public:
			Button( Widget* parent );
			virtual ~Button(){}
		

			//Events
			DEFINE_SLOT( clicked );
			DEFINE_SLOT( doubleClicked );

			RECIEVE_SLOT_FUNC
			{
				EXEC_SLOT( clicked );
				return EXEC_PARENT( Widget );
			}

		protected:


		};

	} // END: gui

} // END: kismo

#endif
