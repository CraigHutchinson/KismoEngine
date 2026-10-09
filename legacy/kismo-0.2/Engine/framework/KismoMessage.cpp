/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\framework\KismoMessage.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
#include "framework/KismoMessage.h"

namespace kismo
{
	namespace frx
	{	
		const Rtti Message::TYPE( _T("Kismo.Message"), 0 );

		Message::Message() : 
			m_wait( kc_FALSE ), m_hEvent( 0 )
		{} 

		Message::~Message()
		{
			if ( m_hEvent )
				CloseHandle( m_hEvent );
		}

		void Message::setWait( k_Bool b ) 
		{ 
			m_wait = b; 
			if ( m_wait )
				m_hEvent = CreateEvent( NULL, FALSE, FALSE, NULL );
			else
			if ( m_hEvent )
			{
				CloseHandle( m_hEvent );
				m_hEvent = 0;
			}
		}	

		void Message::wait( k_UInt32 time ) //Call from a sending thread to wait unti the message is processed (Only if setWait is set)
		{
			if ( m_hEvent )
				WaitForSingleObject( m_hEvent, time );			
		}

		void Message::setProcessed() //Call to set the message as processed so any waiting thread may continue$
		{
			if ( m_hEvent )
				SetEvent( m_hEvent );						
		}


	}
}