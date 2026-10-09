/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\gui\KismoGUIMessage.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef _KISMO_GUIMESSAGE_H
#define	_KISMO_GUIMESSAGE_H

#include "framework/KismoMessage.h"
#include "framework/KismoVector.h"
//#include "gui\KismoWidget.h"

//Helper function for creating GUI message types
#define DEFINE_GUIWIDGET_MSG( _name ) \
	class _KismoExport _name : public GUWidgetMsg { \
		KISMO_DECLARE_RTTI; \
	public: \
		_name( Widget* widget ) : GUWidgetMsg( widget ) {} }	

//Helper for a Gui message with paramater
#define DEFINE_GUIWIDGET_MSG_EX( _name, _paramType, _paramName ) \
	class _KismoExport _name : public GUWidgetMsg { \
		KISMO_DECLARE_RTTI; \
	public: \
		_name( Widget* widget, const _paramType& _paramName ) : GUWidgetMsg( widget ), _paramName( _paramName ) {} _paramType _paramName; } \

#define DEFINE_GUIRENDERER_MSG( _name ) \
	class _KismoExport _name : public GUIRendererMsg { \
		KISMO_DECLARE_RTTI; \
	public: \
		_name( GUIRenderer* renderer ) : GUIRendererMsg( renderer ) {} }	

//Gui message implimentation for Rtti
#define IMPL_GUIWIDGET_MSG( _name ) \
	KISMO_IMPLEMENT_RTTI( "gui", _name, GUWidgetMsg )

#define IMPL_GUIRENDERER_MSG( _name ) \
	KISMO_IMPLEMENT_RTTI( "gui", _name, GUIRendererMsg )

namespace kismo
{
	namespace gui
	{
		//forward declaration
		class Widget;
		class GUIRenderer;

		//--------------------------------
		// Widget messages
		//--------------------------------
		class _KismoExport GUWidgetMsg : public frx::Message
		{
			KISMO_DECLARE_RTTI;
			Widget* widget;
		protected:
			GUWidgetMsg( Widget* widget ) : widget( widget ) {}			
		};
		DEFINE_GUIWIDGET_MSG( GUIMsg_DestroyWidget );
		DEFINE_GUIWIDGET_MSG( GUIMsg_CreateWidget );
		DEFINE_GUIWIDGET_MSG( GUIMsg_ExecWidget );
		DEFINE_GUIWIDGET_MSG( GUIMsg_ShowWidget );
		DEFINE_GUIWIDGET_MSG( GUIMsg_HideWidget );
		DEFINE_GUIWIDGET_MSG_EX( GUIMsg_SizeWidget, frx::Vector2r, size );
		DEFINE_GUIWIDGET_MSG_EX( GUIMsg_PositionWidget, frx::Vector2r, position );
		DEFINE_GUIWIDGET_MSG_EX( GUIMsg_SetWidgetText, k_String, text );

		//--------------------------------
		// Renderer messages
		//--------------------------------
		class _KismoExport GUIRendererMsg : public frx::Message
		{
			KISMO_DECLARE_RTTI;
			GUIRenderer* renderer;
		protected:
			GUIRendererMsg( GUIRenderer* renderer ) : renderer( renderer ) {}			
		};
		DEFINE_GUIRENDERER_MSG( GUIMsg_InitialiseRenderer );
		DEFINE_GUIRENDERER_MSG( GUIMsg_DestroyRenderer );
		DEFINE_GUIRENDERER_MSG( GUIMsg_RemoveRenderer );

	} // END: gui
} // END: kismo

#endif
