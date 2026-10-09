/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\gui\KismoSlotWidget.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
#include "gui\KismoSlotWidget.h"

//#include "KismoWidget.h"



INIT_GUI_SLOTS()


namespace kismo
{
	namespace gui
	{
		//----------------------------------------------
		// Slot
		//----------------------------------------------
		Slot::Slot( const k_String& name ) : 
			m_name( name ), 
			m_handle( 1 ) //Handles are 1-based
		{
			if ( k_Handle hwnd = find( name ) ) //Make sure multiple slots of the same name are not defined!
			{
				m_handle = ms_slots[ hwnd - 1 ]->m_handle; //Copy from the other slot of the same name
				return; //Don't store duplicates in the static list for faster searching
			}
			else
			if ( ms_slots.size() > 0 ) //Increment ID if previous slots exist
				m_handle += ms_slots.back()->m_handle;

			ms_slots.push_back( this );//Store the Slot so we can search for it later if needed
		}

		//Finds a slots handle from its name
		k_Handle Slot::find( const k_String& name )
		{ 
			//TODO: Slot name map
			for ( k_Vector< Slot* >::iterator iter = ms_slots.begin(); iter != ms_slots.end(); ++iter )
				if ( (*iter)->m_name == name )
					return (*iter)->m_handle; //Return the handle of the found slot			
			return 0; //Return failure
		}

		//gets the name of a slot from its handle
		const k_String& Slot::getName( const k_Handle& hwnd )
		{
			assert ( isValidHandle( hwnd ) && hwnd <= ms_slots.size() );
			
			return ms_slots[ hwnd - 1 ]->name();
		}


		//----------------------------------------------
		// SlotWidget
		//----------------------------------------------

		//Bind event links a slot on one GUI widget to that of another
		// e.g. Button source slot "click" linked to a window widgets "hide" slot would cause the windows hide() 
		//		slot to be called causing the window to close
		k_Bool SlotWidget::bind( k_Handle sourceSlot, SlotWidget* targetWidget, k_Handle actionSlot )
		{
			assert( isValidPointer( targetWidget ) );

			m_receivingSlots.insert( k_SLOT_RECIEVER_MAP::value_type( sourceSlot, 
										ReceivingSlot( targetWidget, actionSlot ) )  );

			return kc_TRUE;
		}

		// Sends an event to a recieveing slot
		k_Bool SlotWidget::sendEvent( const k_Handle sendingSlot )
		{
			k_SLOT_RECIEVER_MAP::const_iterator iter;
			
			//Find the first reciever that wants this event
			iter = m_receivingSlots.find( sendingSlot );

			while ( iter != m_receivingSlots.end() && (*iter).first == sendingSlot )//Check that we found a receiver
			{
				assert( isValidPointer( (*iter).second.widget ) );

				//Send the event to the receiving widget
				if ( !(*iter).second.widget->receiveEvent( (*iter).second.slot ) )
				{
					//LOG_RECOVERABLE( "GUI Widget \"" + ((Widget*)this)->name() + "\" sent event to slot [" + Slot::getName( (Widget*)((*iter).second.slot) ) + "] on widget \"" + (*iter).second.widget->name() + "\" which it does not have. The event has been ignored." );
				}
				++iter; //Check that there is not more than one widget wanting this event
			}

			return false;                
		}


	} // END: gui

} // END: kismo

