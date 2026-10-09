/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\gui\KismoSlotWidget.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef _KISMO_GUI_SLOT_H
#define	_KISMO_GUI_SLOT_H

#include "framework/KismoPlatform.h"
#include "framework/KismoCommon.h"

//Create ms_slots static member on Slot ( Must be in the first .cpp)
// TODO: Find a way to make the static initialise before Slot instances!
#define INIT_GUI_SLOTS() k_Vector< kismo::gui::Slot* > kismo::gui::Slot::ms_slots;


//Slot header
#define DEFINE_SLOT( _name ) \
	static const gui::Slot kc_Slot_##_name; \
	virtual void _name()

//Slot implimentation - NOTE: Must be in .cpp
#define IMPL_SLOT( _className, _name ) \
	const gui::Slot _className::kc_Slot_##_name( _T(#_name) ); \
	void _className::_name()

//On DO Event calls the relevant member function when an event is received
#define EXEC_SLOT( _name ) \
	if ( receivingSlot == kc_Slot_##_name ) \
	{ \
		_name(); \
		SEND_EVENT( _name ); \
		return kc_TRUE; \
	}

//Execute the parent classes recieve event
#define EXEC_PARENT( parentClass ) parentClass::receiveEvent( receivingSlot )

//Function header for slot event reciever function
#define RECIEVE_SLOT_FUNC virtual k_Bool receiveEvent( const k_Handle receivingSlot )


//Sends an event to any reciever slots
#define SEND_EVENT( _name ) sendEvent( kc_Slot_##_name );


namespace kismo
{
	namespace gui
	{	
		//forward delaration
		class Widget;

		struct _KismoExport Slot
		{
			//Constructor
			Slot( const k_String& name );

			const k_String& name() const { return m_name; }
			operator k_Handle () const { return m_handle; }

			//Finds a slots handle from its name
			static k_Handle find( const k_String& name );

			//gets the name of a slot from its handle
			static const k_String& getName( const k_Handle& hwnd );

		private:
			const k_String m_name; //Name of the slot used for linking events
            k_Handle m_handle; //The quick handle to the slot for faster event sending
			
			static k_Vector< Slot* > ms_slots; //Vector of created slots
		};

		//Defines base class for using the slot mechanism
		class _KismoExport SlotWidget
		{
		public:
			//----------------------------------------------
			// Function slots
			//----------------------------------------------
			//Bind event links a slot on one GUI widget to that of another
			// e.g. Button source slot "click" linked to a window widgets "hide" slot would cause the windows hide() 
			//		slot to be called causing the window to close
			k_Bool bind( k_Handle sourceSlot, SlotWidget* targetWidget, k_Handle actionSlot );

			virtual k_Bool receiveEvent( const k_Handle receivingSlot ) = 0;

			//performs an event and distributed the event
			k_Bool doEvent( const k_Handle slot )
			{
				return receiveEvent( slot );
			}

		protected:
			//----------------------------------------------
			// Sends an event to a recieveing slot
			//----------------------------------------------
			k_Bool sendEvent( const k_Handle sendingSlot );

			
		private:
			//Slot event reciever data
			struct ReceivingSlot 
			{
				ReceivingSlot(SlotWidget* targetWidget, k_Handle actionSlot) :
					widget( targetWidget ), slot( actionSlot ) {}

                SlotWidget* widget;
				k_Handle slot;
			};

			//Map of widgets receiving events on a slot
			typedef std::multimap< k_Handle, ReceivingSlot > k_SLOT_RECIEVER_MAP;
			k_SLOT_RECIEVER_MAP m_receivingSlots; //Map of recieving slots for events

		};

	} // END: gui

} // END: kismo


#endif
