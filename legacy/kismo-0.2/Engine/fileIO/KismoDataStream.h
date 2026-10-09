/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\fileIO\KismoDataStream.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef _KISMO_DATASTREAM_H
#define _KISMO_DATASTREAM_H

#include "framework/KismoCommon.h"
#include "framework/KismoSmartPtr.h"

namespace kismo
{
	namespace fio
	{
		//forward declaration
		class File;

		enum ke_streamMode
		{
			kc_streamMode_read = std::ios_base::in,
			kc_streamMode_write = std::ios_base::out,
			kc_streamMode_binary = std::ios_base::binary,
		};

		enum ke_streamError
		{


		};

		class DataStream : public frx::IRefCountImpl< DataStream >
		{
		public:			
			typedef frx::SmartPtr< DataStream > Ptr;
		public:			
			/** Constructor
			\param streamMode The read & write access mode of the stream*/
			DataStream( k_Int32 streamMode ) : m_bytes( 0 ), m_isLocked( kc_FALSE ), m_isOpen( kc_FALSE ), m_mode( streamMode ) {}
			virtual ~DataStream() {}
			/** Gets the size of the source stream */
			k_Size size() const { return m_bytes; }
			/** Gets the name of the stream/file */
			virtual const k_String& name() const { return m_name; }
			/** Returns whether the stream has been locked so no other stream can write to this file
			 @remarks Also stops the source File instance from being closed unless forced to do so which 
			 will close this stream. */
			k_Bool isLocked() const { return m_isLocked; }
			/** Is the stream open/closed */
			k_Bool isOpen() const { return m_isOpen; }
			/** Is the stream binary */
			k_Bool isBinary() const { return (m_mode & kc_streamMode_binary) != 0; }
			/** Can the stream be written to */
			k_Bool writeable() const { return (m_mode & kc_streamMode_write) != 0; } 
			/** Can the stream be read */
			k_Bool readable() const { return (m_mode & kc_streamMode_read) != 0; } 
			/** At the end of the stream? */
			virtual k_Bool eof() const = 0;
			/** At the beginning of the stream? */
			virtual k_Bool bof() const = 0;
			/** Read the specified number fo bytes from the stream
			\param outBufferPtr - Output buffer pointer
			\param byteCount - number of bytes to read
			\param returns - number of bytes that were read */
			virtual k_Size read( void* outBufferPtr, k_Size byteCount ) = 0;
			/** Reads a line from the stream stopping at the deliminator */
			virtual k_Size readLine( k_Char* outBufferPtr, k_Size count, k_String deliminator = _T("\n") ) = 0;
			/** Writes data from the buffer to the stream at the current location
			\returns Number of bytes written */
			virtual k_Size write( void* sourceBufferPtr, k_Size byteCount ) { return 0; }
			/** Writes a line to the stream at the current location adding the deliminator at the end
			\returns Number of bytes written */
			virtual k_Size writeLine( k_Char* sourceBufferPtr, k_Size count, k_String deliminator = _T("\n") ){ return 0; }
			/** Seeks to a byte position in the stream */
			virtual k_Bool seekTo( k_Size bytePos ) = 0;
			/** Seeks back/forward within the stream  */
			virtual k_Bool seek( k_Int32 bytes ) = 0;
			/** Returns the current position within the stream from the start */
			virtual k_Size position() const = 0; 
		protected:
            k_String m_name; ///< Name of resource/file
            k_Size m_bytes; ///< The size of the stream if it is known
			k_Bool m_isLocked; ///< Locks the stream so it cannot be unloaded or written to by another stream
			k_Bool m_isOpen; ///< Is the stream open/closed
			k_Int32 m_mode; ///< Read/write/binary stream mode
		};

		class MemoryDataStream : public DataStream
		{
		public:
			/** Constructs a memory stream which uses an area of memory as its source
			\param freeMemoryOnClose sets whether the stream should delete the memory on destruction or not */
			MemoryDataStream( void* data, k_Size bytes, k_Int32 streamMode = kc_streamMode_read, k_Bool freeMemoryOnClose = false );
			~MemoryDataStream();			
			/** @copydoc DataStream::seekTo */
			virtual k_Bool seekTo( k_Size bytePos );
			/** @copydoc DataStream::seek */
			virtual k_Bool seek( k_Int32 bytes );
		protected:
			k_Byte* m_data; ///< Pointer to the data in memory
			k_Byte* m_end; ///< Pointer to the end of the data in memory
			k_Byte* m_currentPosition; ///< Current position in the memory array
			k_Bool m_freeMemoryOnClose; ///< Sets whether the memory should be deleted on close
		};

		class FileDataStream : public DataStream
		{
		public:
			/** constructor takes a full filePath from which to create the stream */
			FileDataStream( File* file, k_Int32 streamMode = kc_streamMode_read );			
			/** @copydoc DataStream::seekTo */
			virtual k_Bool seekTo( k_Size bytePos );
			/** @copydoc DataStream::seek */
			virtual k_Bool seek( k_Int32 bytes );
		protected:
			k_FStream m_stream; ///< The file stream to read/write to
		};

		
	} //END: fileio

} //END: kismo

#endif