/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\gui\KismoWindow.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
#include "gui\KismoWindow.h"

namespace kismo
{
	namespace gui
	{		
		KISMO_IMPLEMENT_RTTI( gui, Window, Widget );

		Vector2r ls_windowDefaultPos = Vector2r( 0, 0 );

		Window::Window( Widget* parent, GUIRenderer* renderer ) :
			Widget( parent )
		{
			setGuiRenderer( renderer );

			WidgetDefinition& def = getDefinition(); 
			def.style = kc_WidgetStyle_Window;
			def.size = Vector2r( 400, 300 );
			def.text = _T("window");
			def.name = _T("_undefined_");
			def.visible = false;
			def.position = ls_windowDefaultPos;
			def.resizeable = true;

			ls_windowDefaultPos.x =ls_windowDefaultPos.y += 15;
		}
	}
}
