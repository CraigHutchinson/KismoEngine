/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\fileIO\KismoDataStream.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
#include "fileIO\KismoDataStream.h"
#include "fileIO\KismoFile.h"

namespace kismo
{
	namespace fio
	{


		MemoryDataStream::MemoryDataStream( void* data, k_Size bytes, k_Int32 streamMode, k_Bool freeMemoryOnClose ) :
			m_data( (k_Byte*)data ),
			m_end( (k_Byte*)data + bytes ),
			m_currentPosition( (k_Byte*)data ),
			m_freeMemoryOnClose( freeMemoryOnClose ),
			DataStream( streamMode )
		{}

		MemoryDataStream::~MemoryDataStream()
		{
			if ( m_freeMemoryOnClose && m_data )
				delete [] m_data;
		}

		k_Bool MemoryDataStream::seekTo( k_Size bytePos )
		{
			return kc_FALSE;
		}

		k_Bool MemoryDataStream::seek( k_Int32 bytes )
		{
			return kc_FALSE;
		}


		FileDataStream::FileDataStream( File* file, k_Int32 streamMode ) :
			DataStream( streamMode )
		{
			assert( isValidPointer( file ) );
			if ( isValidPointer( file ) )
			{
				m_stream.open( file->fullPath().c_str(), m_mode );
				m_isOpen = m_stream.is_open();
			}
		}
		
		k_Bool FileDataStream::seekTo( k_Size bytePos )
		{
			return kc_FALSE;
		}

		k_Bool FileDataStream::seek( k_Int32 bytes )
		{
			return kc_FALSE;
		}

	} //END: fileio
} //END: kismo