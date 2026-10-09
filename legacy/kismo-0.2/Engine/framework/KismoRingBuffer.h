/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\framework\KismoRingBuffer.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef _KISMO_RING_BUFFER_H
#define _KISMO_RING_BUFFER_H

#include "framework/KismoTypes.h"
#include "framework/KismoLockFreeStack.h"

/** Defines the number of bytes to start a record/header on **/
#ifndef MEMORY_ALIGN_BYTES
#	define MEMORY_ALIGN_BYTES 4
#endif

/** Define to specify whether records should be aligned to the memory alignment specified 
by MEMORY_ALIGN_BYTES **/
#define RING_BUFFER_MEMORY_ALIGN

/** Defines whether CAS2 (compare and swap) should be used in the pop() of the ringBuffer.
This could help reduce threading issues and possibly allow multiple threads to 'pop' records
\\TODO: This is to be tested and confirmed
\remark The CAS2 version of pop() is slower by 10-20 times (esitmated) **/
//#define RING_BUFFER_CAS2_POP

/** Defines whether CAS2 (compare and swap) should be used in the push() of the ringBuffer.
This ELIMINATES threading issues and allows multiple threads to 'push' records at one time
\remark The CAS2 version of push() is slower by 10-20 times (esitmated) */
#define RING_BUFFER_CAS2_PUSH

namespace kismo
{
	namespace frx
	{		
		/** \brief Ring buffer for storing a contiuous Queue(FIFO) which requires no dynamic
		memory allocation which would occur with most normal Queues (e.g. STL) and is therefore
		optimised for high performance use.
		\remarks The buffer allows records of variable sizes. If a record passes the end of the buffer,
		the whole record is inserted at the start of the buffer keeping all memory contiguous 
		\remarks The buffer can only be READ BY ONE THREAD but can be WRITTEN BY MANY THREADS
		\remarks Memory will be alligned to MEMORY_ALIGN_BYTES (default 4) bytes for
		optimised performance. #define your platform value if required before including the header */
		class _KismoExport RingBuffer
		{

		public: ///< Public type declarations
			
			typedef k_Vector< RingBuffer > Array;

			typedef k_UInt64 Size_T; ///< Type used to define the buffer size
			typedef k_UInt16 RecordSize_T; ///< Size used to define a records size

			static const Size_T Buffer_Size_MAX = kc_UInt64_MAX-1;
			static const RecordSize_T Record_Size_MAX = kc_UInt16_MAX-2;	

		private: ///< Private type declarations

			enum ke_RecordType ///< Enum values for header values
			{
				kc_RecordType_End = 0, ///< Marks the end of the buffer and no more records are available
				kc_RecordType_InCreate, ///< The record is being pushed and is not ready for get/pop (similar to End)
				kc_RecordType_Created, ///< The record is created and is valid
				kc_RecordType_Wrapped, ///< The record is starts at the beginning of the buffer as could not fit at the end without wrapping
			};

			struct Header ///< Defines the format of a records header section (4Bytes)
			{
				RecordSize_T size; ///< The size of the record in bytes
				k_UByte type; ///< The type/status of the record
			};		

		public: ///< Public functions

			/** \brief Constructs a ring buffer
			\param[in] bufferSizeBytes Specifies the number of bytes to initialise the buffer to
			\remarks It should be noted that each record header will contain sizeof( RecordSize_T ) */
			RingBuffer( k_Size bufferSizeBytes );

			/** \brief Destructor */
			~RingBuffer();		

			/** \brief Initialises the buffer to the specified size clearing any current data
			\return False if the buffer is already created and of a different size.
			\return True if creation succeeded
			\remarks If the buffer size is th same as that already created the buffer will be 
			cleared with clear()*/
			bool create( k_Size bufferSizeBytes );

			/** \brief resizes the buffer to the new size while keeping the contents of the current buffer
			\param[in] bufferSizeBytes Specifies the number of bytes to resize the buffer to
			return Whether the resize was successfull.
			\remark If the buffer has been destroid create() is called internally */
			bool resize( k_Size bufferSizeBytes );

			/** \brief Returns the size of the buffer in bytes */
			k_Size size() const
			{ return m_end - m_start; }

			/** \brief Clears the buffers records */
			void clear();

			/** \brief Destroys the buffer freeing any memory that has been allocated */
			void destroy();

			/** \brief Returns the maximum size available for a new record in bytes */
			k_Size maximumRecordSize() const;

			/** \brief Returns the number of records in the buffer */
			k_Count recordCount() const
			{ return (m_pushes - m_pops); }
			
			/** \brief Returns whether the buffer is empty **/
			bool empty() const
			{ return m_pushes == m_pops; }

			/** \brief Adds a new record and copies the new record into the buffer
			\param[in] srcBuff pointer to the data to put into the record
			\param[in] srcBytes Specifies the size of the new record
			\return Success or failure. If insertion fails false is returned
			\remarks If sizeBytes is 0 or greater than Record_Size_MAX false will be returned
			\remarks This function is thread safe and can be called from any thread */
			bool push( k_Byte* srcBuff, RecordSize_T srcBytes );
			
			/** \brief Returns a pointer to the next record at the read position
			\param[out] recordBytes returns the size of the next record. Pass zero to ignore this paramater
			\return pointer to the next record or 0 if the record is invalid 
			\remarks This is an optimal 'get()' as no memory operations (copy etc) are required
			and the memory in the buffer can be directly accessed by the read thread 
			\remark Should ONLY be called from the same thread that calls pop() */
			k_Byte* get( RecordSize_T* recordBytes = 0 );
			
			/** \brief Fills a buffer with next record at the read position
			\param[in] dataBuffPtr Pointer to a buffer to copy the buffer to
			\param[in] dataBuffSizeBytes Specifies the size of the output buffer
			\param[out] recordSizeBytes Returns the size of the record. Pass zero to ignore this paramater
			\return Whether the target buffer was big enough and all the record was copied
			\remark Should ONLY be called from the same thread that calls pop() */
			bool get( k_Byte* dstBuff, RecordSize_T dstBytes, RecordSize_T* recordBytes = 0 );

			/** \brief Moves the read cursor to the next record
			\return If no record exists False will be returned
			\remark Should only be called from one thread unless RING_BUFFER_CAS2_POP is defined
			\remark RING_BUFFER_CAS2_POP needs to be tested and verified */
			bool pop();

		private: ///< Private data members

			k_Byte* m_start; ///< Pointer to the start of the buffers allocated memory
			k_Byte* m_end; ///< Pointer to the end of the buffer allocated memory

			// NOTE: the order of these members is assumed by CAS2.
			k_Byte* volatile m_read; ///< Position of the current read cursor
			volatile k_Count  m_pops; ///< The number of records that have been popped
			k_Byte* volatile m_write; ///< Position of the current write cursor
			volatile k_Count m_pushes; ///< The number of records that have been pushed
		};

		RingBuffer::RingBuffer( k_Size bufferSizeBytes ) :
			m_start( 0 )
		{
			create( bufferSizeBytes );
		}

		RingBuffer::~RingBuffer()
		{
			destroy();
		}			

		bool RingBuffer::create( k_Size bufferSizeBytes )
		{
			if ( m_start )
			{
				if ( size() == bufferSizeBytes )
				{
					clear(); // The record should be cleared
					return true; //Buffer is already the desired size
				}
				else
				{
					return false; ///Buffer is already created
				}
			}

			m_start = new k_Byte[ bufferSizeBytes ]; ///Create the buffer
			m_end = m_start + bufferSizeBytes;
			m_read = m_write = m_start;
			m_pushes = m_pops = 0;

			return (m_start != 0);
		}

		bool RingBuffer::resize( k_Size bufferSizeBytes )
		{
			return false;
		}

		void RingBuffer::clear()
		{
		}

		void RingBuffer::destroy()
		{
		}

		k_Size RingBuffer::maximumRecordSize() const
		{
			return 0;
			//return math::max( 
		}

		bool RingBuffer::push( k_Byte* srcBuff, RecordSize_T srcBytes )
		{
			//new record header which puts the record in a 'create' state so it does not get pop'ed before memcpy finishes
			Header newheader = { srcBytes, kc_RecordType_InCreate };

			k_Count pushes;
			k_Byte* write;

			while(true)
			{
				pushes = m_pushes;
				write = m_write;

				// Check that the header at 'write' is an empty one and replace with the new header
				if( CAS((void *volatile *)&( write ), reinterpret_cast<void*>(NULL), reinterpret_cast<void*>( *(k_UInt64*)&newheader ) ))
				{
					break;
				}
				else
				{
					k_Byte* next = write;

					switch ( ((Header*)write)->type ) //check the record type
					{
						case kc_RecordType_End: //If reached you are at the end of the buffer
							assert( false );
							return false; //TODO: If this were the case then the first CAS should have succeeded

						case kc_RecordType_Wrapped:
							next = m_start + ((Header*)write)->size; ///Record is from the start of the buffer
							break;

						case kc_RecordType_InCreate:// No record created so could be wrapped or not
							if ( write + sizeof(Header) + ((Header*)write)->size >= m_end ) //does it wrap?
							{
								next = m_start + ((Header*)write)->size;
								break;
							}

						case kc_RecordType_Created:
							next = write + sizeof(Header) + ((Header*)write)->size; //Move past current record
							break;
					}

					// Since another thread has pushed a new header in the buffer we need to move the write head along
					//Without this the algo is basically locking!					
					CAS2((void *volatile *)&m_write, write, pushes, next, pushes + 1 );
				}
			}

			//Check that the new record is ours just incase thread code mucks up!
			assert(		((Header*)write)->type == newheader.type && 
						((Header*)write)->size == newheader.size );

			bool wrapped = (write + sizeof(Header) + ((Header*)write)->size >= m_end); //does it wrap? 
	
			k_Byte* recordWrite = write + 4; 
			if ( wrapped )
				recordWrite = m_start;

			assert( srcBytes == ((Header*)write)->size ); //Just incase extra check

			memcpy( recordWrite, srcBuff, srcBytes );//Copy the record into the the buffer
			k_Byte* next = (recordWrite + srcBytes); //get a pointer to where the next new record will go
			*(k_UInt32*)(next) = 0; //Set the header for the next item to null
			
			//Update the write position
			CAS2((void *volatile *)&m_write, write, pushes, next, pushes + 1 );

			//Set that the record is written/created
			((Header*)write)->type = (wrapped ? kc_RecordType_Wrapped : kc_RecordType_Created );

			return false;
		}

		k_Byte* RingBuffer::get( RecordSize_T* recordBytes )
		{
			if( m_read == m_write ) ///The buffer is empty
				return 0;

			if ( recordBytes ) //If the record size has been requested set it here
				*recordBytes = ((Header*)m_read)->size;

			switch ( ((Header*)m_read)->type ) //check the record type
			{
				case kc_RecordType_Created:
					return m_read + sizeof(Header); //Return the record which is just after the header					
					break;
				case kc_RecordType_Wrapped:
					return m_start; //return the record which has been put at the start of the buffer
					break;

				default:
					assert( false );
				case kc_RecordType_End:
				case kc_RecordType_InCreate:
					return 0; // No record to return
			}
		}

		bool RingBuffer::get( k_Byte* dstBuff, RecordSize_T dstBytes, RecordSize_T* recordBytes )
		{
			k_Byte* record = get( recordBytes );
			if ( !record )
			{
				return false; //No record was found
			}
			else
			{
				memcpy_s( dstBuff, dstBytes, record, (rsize_t)recordBytes );
				return true;				
			}
		}

		bool RingBuffer::pop()
		{
#ifndef RING_BUFFER_CAS2_POP
			switch ( ((Header*)m_read)->type ) //check the record type
			{
				case kc_RecordType_End:
				case kc_RecordType_InCreate:
					return false; // No record to pop

				case kc_RecordType_Created:
					m_read += sizeof(Header) + ((Header*)m_read)->size; //Move past current record
					break;
				case kc_RecordType_Wrapped:
					m_read = m_start + ((Header*)m_read)->size; ///Record is from the start of the buffer
					break;
			}
			return true;

#else
			while(true)
			{
				k_Count pops = m_pops;
				k_Byte* read = m_read;
				k_Byte* next = read;

				// Verify that we did not get the pointers in the middle of another update.
				if( pops != m_pops )
				{
					continue;
				}

				// Check if the queue is empty.
				if( read == m_write )
				{
					return false;
				}
				else
				{
					switch ( ((Header*)read)->type ) //check the record type
					{
						case kc_RecordType_End: //If reached you are at the end of the buffer
							asset( false );
							return false; //TODO: Should not reach here as read & write should have been the same
						case kc_RecordType_InCreate:
							return false; // No record to pop... yet

						case kc_RecordType_Created:
							next = read + sizeof(Header) + ((Header*)read)->size; //Move past current record
							break;
						case kc_RecordType_Wrapped:
							next = m_start + ((Header*)read)->size; ///Record is from the start of the buffer
							break;
					}

					// Move the read pointer, effectively removing the Node
					if(CAS2((void *volatile *)&m_read, read, pops, next, pops + 1))
					{
						break;
					}
				}
			}
			return true;
#endif
		}



	} //END: frx

} //END: kismo

#endif