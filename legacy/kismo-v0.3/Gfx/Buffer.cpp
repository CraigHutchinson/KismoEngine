/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\Gfx\Buffer.cpp
 * Status: current (newest copy of this file name in the archive)
 */
#include "Buffer.h"
#include "Framework/Common.h"
#include "Math/CoreMath.h"
#include "Framework/Log.h"

namespace gfx
{

	SwBuffer::Ptr SwBuffer::create( ePurpose purpose, const BufferDefinition& vertexDefinition, Size elementCount, eUsage usage )
	{ return Ptr( new SwBuffer( purpose, vertexDefinition, elementCount, usage ) ); }

	SwBuffer::Ptr SwBuffer::create( ePurpose purpose, Size byteSize, eUsage usage )
	{ return Ptr( new SwBuffer( purpose, byteSize, usage ) ); } 

	SwBuffer::SwBuffer( ePurpose purpose, const BufferDefinition& bufferDefinition, Size bufferCount, eUsage usage ) :
		Buffer( purpose, bufferDefinition, bufferCount, usage ),
		m_data( 0 ),
		m_locked( false ),
		m_readLocks( 0 )
#ifdef _DEBUG
		,m_lockData(0), m_lockStartOffset(0), m_lengthToLock(0)
#endif
	{}

	SwBuffer::SwBuffer( ePurpose purpose, Size byteSize, eUsage usage ) :
		Buffer( purpose, byteSize, usage ),
		m_data( 0 ),
		m_locked( false ),
		m_readLocks( 0 )
//#ifdef _DEBUG
//		,m_lockData(0), m_lockStartOffset(0), m_lengthToLock(0)
//#endif
	{}

	SwBuffer::~SwBuffer()
	{
		frx::safeDelete( m_data );
//#ifdef _DEBUG
//		frx::safeDelete( m_lockData );
//#endif
	}

	bool SwBuffer::initialise( Size size, const Byte* data )
	{
		if ( frx::isValidPointer( m_data ) )
			return false;
		
		m_size = math::max( size, m_size );
		if ( !m_size )
		{
			logError( "Software buffer initialise failed as a zero size was specified!" );
			return false;
		}

		m_data = new Byte[ m_size ];
		assert( m_data );

		if ( data ) //If initial data was supplied we fill the buffer with it
		{
			memcpy( m_data, data, m_size );
		}

		return true;		
	}

	void* SwBuffer::lock( Size startOffset, Size lengthToLock, eLockFlag flags )
	{
		if ( m_locked ) //If a write lock is inplace we can't lock!
			return 0;

		if ( LockFlag_ReadOnly )
			++m_readLocks; //increase the read lock counter to allow multiple reads
		else
		{
			m_locked = true;
			while ( m_readLocks != 0 ) {} //Wait till we  have explcicit access
		}

		if ( !startOffset && !lengthToLock )
			lengthToLock = getSize();
		else
		if ( startOffset >= getSize() || (startOffset + lengthToLock) > getSize() )
			return 0;
//
//#ifdef _DEBUG
//		m_lockData = new Byte[ lengthToLock ]; //Create new temporary array
//		assert( m_lockData );
//		memcpy( m_lockData, m_data + startOffset, lengthToLock );
//		m_lockStartOffset = startOffset;
//		m_lengthToLock = lengthToLock;
//		return m_lockData;
//#else
		return m_data + startOffset;
//#endif			
	}

	void SwBuffer::release()
	{
		assert( m_locked || m_readLocks );
		if ( m_locked ) //If there was a full lock in place
			m_locked = false;
		else
			--m_readLocks; ///< Decrese the read lock count
//
//#ifdef _DEBUG
//		assert( m_lockData );
//		memcpy( m_data + m_lockStartOffset, m_lockData, m_lengthToLock );
//		frx::safeDeleteArray( m_lockData );
//#else
//		
//#endif 
		
	}

} //END: gfx