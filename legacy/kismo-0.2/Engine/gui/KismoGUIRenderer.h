/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\gui\KismoGUIRenderer.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef _KISMO_GUI_IRENDERER_H
#define	_KISMO_GUI_IRENDERER_H

#include "gui\KismoGUI.h"
#include "framework/KismoMessageQueue.h"

namespace kismo
{
	namespace gui
	{	
		class GUIRenderer : public frx::MessageQueue
		{
		public:

			GUIRenderer( const k_String& name ) : 
				m_initialised(false )
			{ 
				_Gui.addRenderer( name, this ); 
			}

			virtual ~GUIRenderer(){} //virtual destructor

			//Initialise the renderer
			virtual k_Bool initialise() = 0;
			k_Bool initialised() { return m_initialised; }

			//Destroy tyhe renderer
			virtual void destroy() = 0;

			//Update the GUI and poll the message queue
			virtual k_Bool update() = 0;
			
		protected:

			k_Bool m_initialised;

		};

	} // END: gui

} // END: kismo

#endif
