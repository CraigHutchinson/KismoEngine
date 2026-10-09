/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\KismoCore\KismoFramework\SmallSet.h
 * Status: duplicate of kismo-core/_SOURCE_/KismoFramework/SmallSet.h
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

#ifndef KISMOFRAMEWORK_SMALLSET
#define KISMOFRAMEWORK_SMALLSET

#include "KismoFramework/Types.h"
#include "KismoFramework/Vector.h"

//TEMP:
#include <vector>

namespace kismo
{
	/** \brief		SmallSet to replace the STL set when memory is a concern. 
		\remarks	Designed for sets of 8 or less items.
					Uses a vector for the internal storage and does an iterated lookup
	*/
	template < typename T > 
	class SmallSet
	{
	public:
		typedef /*Vector*/std::vector< T > DataVec; ///< Data tore vector type
		typedef typename DataVec::const_iterator const_iterator;
		typedef typename DataVec::iterator iterator;
		typedef typename DataVec::const_reverse_iterator const_reverse_iterator;
		typedef typename DataVec::reverse_iterator reverse_iterator;
		typedef typename DataVec::const_reference const_reference;
		typedef typename DataVec::reference reference;

	public:

		/** \brief Reserve space for a certain number of values
		*/
		inline void reserve( Count count )
		{ m_data.reserve( count ); }

		/** \brief Inserts a value into the set
		\remarks
			Duplicate values are ignored
		*/
		inline void insert( T val )
		{
			if ( find( val ) == end() )
				m_data.push_back( val );
		}

		/** \brief Returns the number of values held in the set
		*/
		inline Count size() const
		{ return (Count)m_data.size(); }
		
		/** \brief Returns whether the set is empty ie. has no values
		*/
		inline Bool empty() const
		{ return (Bool)m_data.empty(); }
		
		/** \brief Finds a value within the set and returns its iterator
		*/
		template < class TComp >
		inline iterator find( const TComp& val )
		{
			iterator it = begin();
			for (; it != end() && (*it) != val; ++it ) {}
			return it;
		}

		/** \brief Finds a value within the set and returns its const_iterator
		*/
		template < class TComp >
		inline const_iterator find( const TComp& val ) const
		{
			const_iterator it = begin();
			for (; it != end() && (*it) != val; ++it ) {}
			return it;
		}

		/** \brief Erases the value at a supplied iterator
		*/
		inline void erase( iterator it )
		{ m_data.erase( it ); }

		/** \brief Clears the set so it is empty
		*/
		inline void clear()
		{ m_data.clear(); }


		/** \brief Gets and iterator that ponts to the first item in the set
		*/
		inline iterator begin()
		{ return m_data.begin(); }
		inline reverse_iterator rbegin()
		{ return m_data.rbegin(); }
	
		/** \brief Gets and const_iterator that points to the first item in the set
		*/
		inline const_iterator begin() const
		{ return m_data.begin(); }
		inline const_reverse_iterator rbegin() const
		{ return m_data.rbegin(); }
		
		/** \brief Gets and iterator that points the position new items will be inserted ie. after 
			the last item
		*/
		inline iterator end()
		{ return m_data.end(); }
		inline reverse_iterator rend()
		{ return m_data.rend(); }
		
		/** \brief Gets and const_iterator that points the position new items will be inserted ie. after 
			the last item
		*/
		inline const_iterator end() const
		{ return m_data.end(); }
		inline const_reverse_iterator rend() const
		{ return m_data.rend(); }

	private:
#pragma warning(disable:4251)
		DataVec m_data; ///< Vector of the data 
#pragma warning(default:4251)
	};

} //END: kismo

#endif
