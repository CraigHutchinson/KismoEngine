/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\KismoCore\KismoFramework\FastQueue.h
 * Status: duplicate of kismo-core/_SOURCE_/KismoFramework/FastQueue.h
 */
/*^**************************************************************************
*	
*	Kismo Engine 
*	© 2008-2013 Craig Hutchinson (craig@craighutchinson.co.uk)
*	All right reserved
*
*	This file is subject to the terms and conditions defined in
*	file 'license.txt', which is part of this source code package.
*
***************************************************************************^*/

#ifndef FRX_FASTQUEUE
#define FRX_FASTQUEUE

#include "Types.h"

namespace frx
{
	/** \brief FastQueue to replace the STL queue when memory contiguity is concerned.
		\remarks Uses a vector for the internal storage and loops the iterator from end to start.
		\par This is a quue based on a ringbuffer approach
	*/
	template < typename T > 
	class FastQueue
	{
	public:
		typedef UInt32 Index_T;
		typedef UInt32 Count_T;
		typedef std::vector< T > DataVec; ////< Data tore vector type
		//typedef typename DataVec::const_iterator const_iterator;
		//typedef typename DataVec::iterator iterator;
		//typedef typename DataVec::const_reverse_iterator const_reverse_iterator;
		//typedef typename DataVec::reverse_iterator reverse_iterator;
		typedef typename DataVec::const_reference const_reference;
		typedef typename DataVec::reference reference;

	public:
		
		FastQueue( Count_T reserveSize ) : m_writePos( 0 ), m_readPos( 0 ), m_size( 0 )
		{ m_data.resize( reserveSize ); }

		/** \brief Inserts a value into the queue
		*/
		void push( T val )
		{
			++m_size;
			m_data[ m_writePos ] = val;
			++m_writePos; ////< We keep track fo the size
			if ( m_writePos >= m_data.size() )
				m_writePos = 0; ////< We go back to the start
		}
	
		/** \brief Pop a value fromt he front of the queue
		*/
		void pop()
		{
			Assert( m_size );
			--m_size;
			++m_readPos;
			if ( m_readPos >= m_data.size() )
				m_readPos = 0; ////< We go back to the start
		}

		/** \brief Gets a reference to the value at the read position (const)
		*/
		const_reference front() const
		{ return m_data[ m_readPos ]; }

		/** \brief Gets a reference to the value at the read position 
		*/
		reference front()
		{ return m_data[ m_readPos ]; }
		
		/** \brief Returns the number of values held in the set
		*/
		Count_T size() const
		{ return (Count_T)m_size; }
		
		/** \brief Get the capacity of the queue
		*/
		Count_T capacity() const
		{ return (Count_T)m_data.capacity(); }

		/** \brief Returns whether the set is empty ie. has no values
		*/
		Bool empty() const
		{ return (Bool)(m_size == 0); }

		/** \brief Resets the read and write and size
		*/
		void clear()
		{ m_size = 0; m_writePos = 0; m_readPos = 0; }

	private:
		Count_T m_size; ////< The size of the queue / number of elements
		Index_T m_readPos; ////< The position of the read header
		Index_T m_writePos; ////< The position of the write header
		DataVec m_data; ////< Vector of the data nodes
	};

} //END: frx

#endif
