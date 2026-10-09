/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\framework\KismoMessage.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef _KISMO_MESSAGE_H
#define _KISMO_MESSAGE_H

#include "framework/KismoCommon.h"
#include "framework/KismoSmartPtr.h"
#include "framework/KismoRtti.h"

//TODO: TEMP: \/ Remove  
#include <windows.h>

namespace kismo
{
	namespace frx
	{
		//Base message class
		class _KismoExport Message : public IRefCountImpl< Message >
		{
			KISMO_DECLARE_RTTI;
		public:			
			typedef SmartPtr< Message > Ptr;
		public:
			virtual ~Message();

			void setWait( k_Bool b = kc_TRUE); //Set the message as a locking message 
			void wait( k_UInt32 time = kc_UInt32_MAX ); //Call from a sending thread to wait unti the message is processed (Only if setWait is set)
			void setProcessed(); //Call to set the message as processed so any waiting thread may continue


			//----------------------------
			//Rtti
			//----------------------------
			k_Bool isExactly ( const Rtti& type ) const;
			k_Bool isDerived ( const Rtti& type ) const;
			k_Bool isExactlyTypeOf ( const Message* obj ) const;
			k_Bool isDerivedTypeOf ( const Message* obj ) const;

		protected:			
			Message(); //Protected message contructor to prevent direct creation without a derived class
				 
			k_Bool m_wait; //Whether to wait for the message to be processed
			HANDLE m_hEvent; //TEMP: Event handle
		};
		
		inline bool Message::isExactly( const Rtti& type ) const
		{ return getType().isExactly( type ); }

		inline bool Message::isDerived( const Rtti& type ) const
		{ return getType().isDerived( type ); }

		inline bool Message::isExactlyTypeOf( const Message* obj ) const
		{ return obj && getType().isExactly(obj->getType()); }

		inline bool Message::isDerivedTypeOf( const Message* obj ) const
		{ return obj && getType().isDerived( obj->getType() ); }

	}
}

#endif