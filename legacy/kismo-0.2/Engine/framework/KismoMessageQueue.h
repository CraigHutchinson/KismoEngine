/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\framework\KismoMessageQueue.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef _KISMO_MESSAGEQUEUE_H
#define _KISMO_MESSAGEQUEUE_H

#include "framework/KismoMessage.h"
#include "framework/KismoRingBuffer.h"

namespace kismo
{
	namespace frx
	{		
		class _KismoExport MessageQueue
		{
		public: ///< Public functions

			MessageQueue() :
				m_messages( 4096 ) {};
			virtual ~MessageQueue(){}

			/** \brief Poll the message queue for messages
			\returns Returns true if there were any messages to process **/
			k_Bool pollMessageQueue();

			/** \brief Add a message to the back of the message buffer for processing
			on the next update
			\param[in] msg Pointer to the message to add to the queue
			\param[in] size Size of the message in bytes **/
			void sendMessage( Message* msg, k_Size size ) 
			{ 
				m_messages.push( (k_Byte*)msg, (RingBuffer::RecordSize_T)size ); 
				msg->wait(); 
			}

			template < class _T >
			void sendMessage( _T& msg ) 
			{ 
				m_messages.push( &msg, sizeof( _T ) ); 
				msg.wait(); 
			}


			/** \brief Post a message to the queue for instant processing
			\param[in] msg Pointer to the message to add to the queue **/
			void postMessage( Message* msg ) 
			{ processMessage( msg ); }

			/** \brief Clears any messages currently in the message queue **/
			void clearMessages() 
			{ m_messages.clear(); }

		protected: ///< Protected functions

			/** \brief Process a message (not implimented in base) */
			virtual void processMessage( Message* msg ) = 0;
			
			/** \brief Checks if there are any messages in the queue **/
			k_Bool hasMessage() const
			{ return !m_messages.empty(); }

		private: ///< Private member variables

			RingBuffer m_messages; ///< Messages posted to the thread are stored in a ring buffer
		};

	} //END: frx

} //END: kismo

#endif