/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\Framework\SmallSet.h
 * Status: superseded by kismo-core/_SOURCE_/KismoFramework/SmallSet.h (same name, content differs)
 */
#ifndef FRX_SMALLSET
#define FRX_SMALLSET

#include "Framework/Types.h"

namespace frx
{
	/** \brief SmallSet to replace the STL set when memory is a concern. 
		\remarks Designed for sets of 8 or less items
		\remarks Uses a vector for the internal storage and does an iterated lookup
	*/
	template < typename T > 
	class _DllExport SmallSet
	{
	public:
		typedef Vector< T > DataVec; ///< Data tore vector type
		typedef typename DataVec::const_iterator const_iterator;
		typedef typename DataVec::iterator iterator;
		typedef typename DataVec::const_reverse_iterator const_reverse_iterator;
		typedef typename DataVec::reverse_iterator reverse_iterator;
		typedef typename DataVec::const_reference const_reference;
		typedef typename DataVec::reference reference;

	public:

		/** \brief Reserve space for a certain number of values
		*/
		void reserve( Count count )
		{ m_data.reserve( count ); }

		/** \brief Inserts a value into the set
		\remarks
			Duplicate values are ignored
		*/
		void insert( T val )
		{
			if ( find( val ) == end() )
				m_data.push_back( val );
		}

		/** \brief Returns the number of values held in the set
		*/
		Count size() const
		{ return (Count)m_data.size(); }
		
		/** \brief Returns whether the set is empty ie. has no values
		*/
		Bool empty() const
		{ return (Bool)m_data.empty(); }
		
		/** \brief Finds a value within the set and returns its iterator
		*/
		template < class TComp >
		iterator find( const TComp& val )
		{
			iterator it = begin();
			for (; it != end() && (*it) != val; ++it ) {}
			return it;
		}

		/** \brief Finds a value within the set and returns its const_iterator
		*/
		template < class TComp >
		const_iterator find( const TComp& val ) const
		{
			const_iterator it = begin();
			for (; it != end() && (*it) != val; ++it ) {}
			return it;
		}

		/** \brief Erases the value at a supplied iterator
		*/
		void erase( iterator it )
		{ m_data.erase( it ); }

		/** \brief Clears the set so it is empty
		*/
		void clear()
		{ m_data.clear(); }


		/** \brief Gets and iterator that ponts to the first item in the set
		*/
		iterator begin()
		{ return m_data.begin(); }
		reverse_iterator rbegin()
		{ return m_data.rbegin(); }
	
		/** \brief Gets and const_iterator that points to the first item in the set
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

} //END: frx

#endif
