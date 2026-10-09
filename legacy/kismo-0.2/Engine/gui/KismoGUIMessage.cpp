/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\gui\KismoGUIMessage.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
#include "gui\KismoGUIMessage.h"


namespace kismo
{
	namespace gui
	{
		//Base GUI message type used for most messages
		KISMO_IMPLEMENT_RTTI( gui, GUWidgetMsg, frx::Message );
		KISMO_IMPLEMENT_RTTI( gui, GUIRendererMsg, frx::Message );

		//Renderer message types
		IMPL_GUIRENDERER_MSG( GUIMsg_InitialiseRenderer );
		IMPL_GUIRENDERER_MSG( GUIMsg_DestroyRenderer );
		IMPL_GUIRENDERER_MSG( GUIMsg_RemoveRenderer );
		
		//Widget message types
		IMPL_GUIWIDGET_MSG( GUIMsg_DestroyWidget );
		IMPL_GUIWIDGET_MSG( GUIMsg_CreateWidget );
		IMPL_GUIWIDGET_MSG( GUIMsg_ExecWidget );
		IMPL_GUIWIDGET_MSG( GUIMsg_ShowWidget );
		IMPL_GUIWIDGET_MSG( GUIMsg_HideWidget );
		IMPL_GUIWIDGET_MSG( GUIMsg_SizeWidget );
		IMPL_GUIWIDGET_MSG( GUIMsg_PositionWidget );
		IMPL_GUIWIDGET_MSG( GUIMsg_SetWidgetText );

	} // END: gui
} // END: kismo
