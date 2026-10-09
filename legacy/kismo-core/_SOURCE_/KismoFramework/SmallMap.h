/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\KismoCore\_SOURCE_\KismoFramework\SmallMap.h
 * Status: current (newest copy of this file name in the archive)
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

#ifndef KISMOFRAMEWORK_SMALLMAP
#define KISMOFRAMEWORK_SMALLMAP

#include "KismoFramework/Types.h"

namespace kismo
{
	/** \brief SmallMap to replace the STL map when memory is a concern. 
		\remarks Designed for maps of 8 or less items
		\remarks Uses a vector for the internal storage and does an iterated lookup
		\remarks const The data stores should be reconstructable i.e - not complex classes
	*/
	template < typename TKey, typename TValue > 
	class SmallMap
	{
	public:
		
		typedef TKey key_type;
		typedef TValue mapped_type;
		struct value_type ///< Value type similar to stl map type
		{
			key_type first;
			mapped_type second;
		};
		typedef std::vector< value_type > DataVec; ///< Data store vector type
		typedef typename DataVec::const_iterator const_iterator;
		typedef typename DataVec::iterator iterator;
		typedef typename DataVec::const_reverse_iterator const_reverse_iterator;
		typedef typename DataVec::reverse_iterator reverse_iterator;
		typedef typename const TValue& const_reference;
		typedef typename TValue& reference;

	public:

		/** \brief Reserve space for a certain number of values
		*/
		void reserve( Count count )
		{ m_data.reserve( count ); }

		/** \brief Inserts a value into the map
		\remarks
			Replaces any existing value
		*/
		void insert( const value_type& val )
		{
			iterator it = find( val.first );
			if ( it == end() )
				m_data.push_back( val );
			else
				*it = val;
		}

		/** \brief Returns the number of values held in the map
		*/
		Count size() const
		{ return (Count)m_data.size(); }
		
		/** \brief Returns whether the map is empty ie. has no values
		*/
		Bool empty() const
		{ return (Bool)m_data.empty(); }
		
		/** \brief Finds a value within the map and returns its iterator
			\remarks Takes any paramater that compares (!= operator) to key_type
		*/
		template < class TKeyComp >
		iterator find( const TKeyComp& val )
		{
			iterator it = begin();
			for (; it != end() && it->first != val; ++it ) {}
			return it;
		}

		/** \brief Finds a value within the map and returns its const_iterator
			\remarks Takes any paramater that compares (!= operator) to key_type
		*/
		template < class TKeyComp >
		const_iterator find( const TKeyComp& val ) const
		{
			const_iterator it = begin();
			for (; it != end() && it->first != val; ++it ) {}
			return it;
		}

		/** \brief Array access operator 
			\remarks If key doesn't exist it will be created
		*/
		reference operator [] ( const key_type& key )
		{
			iterator it = find( key );
			if ( it != end() )
				return it->second;
			else
			{
				value_type tval = { key, mapped_type() };
				m_data.push_back( tval );
				return m_data.back().second;
			}
		}

		/** \brief Array access operator (const)
			\remarks If key doesn't exist an Assertion will occur!
		*/
		const_reference operator [] ( const key_type& key ) const
		{
			const_iterator it = find( key );
			if ( it != end() )
				return it->second;
			else
			{
				Assert( false );
				return m_data.back().second;
			}
		}

		/** \brief Erases the value at a supplied iterator
		*/
		void erase( iterator it )
		{ m_data.erase( it ); }

		/** \brief Clears the map so it is empty
		*/
		void clear()
		{ m_data.clear(); }


		/** \brief Gets and iterator that ponts to the first item in the map
		*/
		iterator begin()
		{ return m_data.begin(); }
		reverse_iterator rbegin()
		{ return m_data.rbegin(); }
	
		/** \brief Gets and const_iterator that points to the first item in the map
		*/
		const_iterator begin() const
		{ return m_data.begin(); }
		const_reverse_iterator rbegin() const
		{ return m_data.rbegin(); }
		
		/** \brief Gets and iterator that points the position new items will be inserted ie. after 
			the last item
		*/
		iterator end()
		{ return m_data.end(); }
		reverse_iterator rend()
		{ return m_data.rend(); }
		
		/** \brief Gets and const_iterator that points the position new items will be inserted ie. after 
			the last item
		*/
		const_iterator end() const
		{ return m_data.end(); }
		const_reverse_iterator rend() const
		{ return m_data.rend(); }

	private:
		DataVec m_data; ///< Vector of the data nodes
	};

} //END: kismo

#endif
