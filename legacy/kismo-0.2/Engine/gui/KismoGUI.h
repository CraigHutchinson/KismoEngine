/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\gui\KismoGUI.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef _KISMO_GUI_H
#define	_KISMO_GUI_H

#include "framework/KismoCommon.h"
#include "framework/KismoThread.h"
#include "framework/KismoVector.h"

#include "gui\KismoGUIMessage.h"

#define _Gui kismo::gui::GUI::getSingleton()

namespace kismo
{
	//Include framework utilities
	using frx::Thread;
	using frx::Singleton;
	using frx::Vector2r;
	using frx::Message;

	namespace gui
	{	
		//Forward declaration
		class GUIRenderer;
		class Widget;

		//The core GUI
		// Is Singleton and runs in its own thread
		class _KismoExport GUI : public Thread, public Singleton< GUI >
		{
		public:
			
			static GUI& getSingleton( void );
			static GUI* getSingletonPtr( void );

		public:

			GUI();
			~GUI();

			//Add a renderer to the map of available gui renderers
			bool addRenderer(  const k_String& name, GUIRenderer* renderer );

			//get a GUi renderer from its name
			GUIRenderer* getRenderer( const k_String& name );
			
			//Remove a renderer
			bool removeRenderer( const k_String& name );
			bool removeRenderer( GUIRenderer* renderer );

			//Update all the Gui renderers
			k_Bool update();

			//virtual void postMessage( Message::Ptr msg ) 
			//{ 				
			//	if ( shouldThreadTerminate() ) return;
			//	if ( isCurrentThread() )
			//		processMessage( msg ); //Process the message now
			//	else
			//		sendMessage( msg ); //Add to the message queue
			//}

			//Thread process
			virtual void run();

		protected:

			
			//Process a message sent to the thread
			virtual void processMessage( Message::Ptr& msg );

		protected:
			
			typedef k_Map< k_String, GUIRenderer* > RENDERER_MAP;
			RENDERER_MAP m_renderers; //The renderers

		};

	} // END: gui
} // END: kismo

#endif
