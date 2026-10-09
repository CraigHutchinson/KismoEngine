/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Plugins\Win32_GUIRenderer\Win32_GUIRenderer.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef _KISMO_GUI_WIN32RENDERER_H
#define	_KISMO_GUI_WIN32RENDERER_H

//#define WIN32_LEAN_AND_MEAN

//Define Window 2000 as the minimum windows version so we hav scroll wheel support & the extra buttons
#ifndef WINVER 
	#define _WIN32_WINNT 0x0500
	#define WINVER 0x0500
	#define _WIN32_WINDOWS 0x0500 // Change this to the appropriate value to target Windows Me or later.
	#define _WIN32_IE 0x0500
#endif

#include "gui\KismoGUIRenderer.h"

#include <commctrl.h>
//#include <windows.h>

#ifndef SIDEBYSIDE_COMMONCONTROLS
#error "SIDEBYSIDE_COMMONCONTROLS must be defined to = 1"
#endif


namespace kismo
{
	namespace gui
	{	
		//forward declaration
		class Win32Widget;

		class Win32_GUIRenderer : public GUIRenderer
		{
		public:

			Win32_GUIRenderer();
			virtual ~Win32_GUIRenderer();

			//Initialise the renderer
			virtual k_Bool initialise();

			//Destroy the renderer
			virtual void destroy();

			//Update the GUI and poll the message queue
			virtual k_Bool update();

			//Create a widget
			virtual k_Bool createWidget( Widget* widget );//Create a widget
			virtual k_Bool hideWidget( Widget* widget );//hide a widget
			virtual k_Bool showWidget( Widget* widget );//show a widget
			virtual k_Bool sizeWidget( Widget* widget, const Vector2r& size );//set the size of the widget
			virtual k_Bool positionWidget( Widget* widget, const Vector2r& position );//set the position of the widget
			virtual k_Bool setWidgetText( Widget* widget, const k_String& text );//set the text on the widget

			
			//Helper functions for win32

			
			Win32Widget* getWidget( Widget* widget ); //get a win32 widget from a interface widget
			Win32Widget* getWidget( HWND hWnd );//Get the widget from its handle
			Win32Widget* getParent( Win32Widget* widget );//Ge the parent handle for a widget


		protected:

			//Process a message sent to the renderer
			virtual void processMessage( Message::Ptr& msg );

			bool pollWin32MessageQueue(); //UPdate the win32 gui

		protected:

			k_Map< HWND, Win32Widget* > m_hwidgets;
			k_Map< Widget*, Win32Widget* > m_widgets;

		};

	} // END: gui

} // END: kismo

#endif
