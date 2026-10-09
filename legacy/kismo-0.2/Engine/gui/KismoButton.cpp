/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\gui\KismoButton.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
#include "gui\KismoButton.h"

namespace kismo
{
	namespace gui
	{	
		Vector2r ls_buttonDefaultPos = Vector2r( 0, 0 );

		IMPL_SLOT( Button, clicked ){};
		IMPL_SLOT( Button, doubleClicked ){};

		Button::Button( Widget* parent ) : 
			Widget( parent )
		{
			WidgetDefinition& def = getDefinition(); 
			def.style  = kc_WidgetStyle_Button;
			def.size = Vector2r( 100, 25 );
			def.position = Vector2r( 0, 0 );
			def.text = _T("button");
			def.name = _T("_undefined_");
			def.visible = true;
			def.resizeable = false;
			def.position = ls_buttonDefaultPos;

			ls_buttonDefaultPos.x = ls_buttonDefaultPos.y += 15;

		}

	}
}
