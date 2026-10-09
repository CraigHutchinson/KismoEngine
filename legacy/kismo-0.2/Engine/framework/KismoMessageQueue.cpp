/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\framework\KismoMessageQueue.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
#include "framework/KismoMessageQueue.h"

namespace kismo
{
	namespace frx
	{	
		k_Bool MessageQueue::pollMessageQueue()
		{
			Message* msg = (Message*)m_messages.get(); //get the first message
			
			if ( !msg )
				return kc_FALSE;

			while ( msg )
			{
				processMessage( msg ); //Process the message
				msg->setProcessed(); //Set the message as processed

				m_messages.pop(); //Remove the last message
				msg = (Message*)m_messages.get(); //get the next message..if one exists
			}

			return kc_TRUE; //Return that there were messages
		}
	} //END: frx
} //END: kismo