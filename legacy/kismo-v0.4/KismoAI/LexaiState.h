/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoAI\LexaiState.h
 * Status: unique (no other copy of this file name in the archive)
 */
/** \author Craig Hutchinson \file 
	\brief 
*/
#ifndef LEXAI_STATE_H
#define LEXAI_STATE_H

#include "FrxaiWord.h"
#include "LexaiSlots.h"

namespace lexai
{
	/** \brief Base State interface which can be specialised as required
	*/
	template< class _TNode, class _TThis >
	class BaseState : protected _TNode::Array
	{
	public:

		typedef List< _TThis > Array;
		typedef Set< _TThis > Lookup;

		typedef typename _TNode::Array::iterator iterator;
		typedef typename _TNode::Array::const_iterator const_iterator;

	public:
		
		/** \brief Virtual destructor to alow derived classes
		*/
		virtual ~BaseState(){}

		/** \brief Compares the node array of two states for equality
		*/
		Int8 compare( const BaseState& rhs ) const;

		/** \brief Compares two states for exact equality
		*/
		Bool operator == ( const BaseState& rhs ) const
		{ return (compare( rhs ) == 0); }
		
		Bool operator < ( const BaseState& rhs ) const
		{ return (compare( rhs ) == -1); }

		Bool operator > ( const BaseState& rhs ) const
		{ return (compare( rhs ) == 1); }
	
		//Iterators
		iterator begin() { return _TNode::Array::begin(); }
		const_iterator begin() const { return _TNode::Array::begin(); }
		iterator end() { return _TNode::Array::end(); }
		const_iterator end() const { return _TNode::Array::end(); }

		//Append state to back of this

		template< class TIt >
		void push_back( TIt ibegin, TIt iend )
		{ while ( ibegin != iend ) { push_back( *ibegin ); ++ibegin; } }
		void push_back( const typename _TNode::Array::value_type& i ) 
		{ return _TNode::Array::push_back( i ); }

		//Container functions
		void clear() { _TNode::Array::clear(); }
		bool empty() const { return _TNode::Array::empty(); }
		typename _TNode::Array::size_type size() const { return _TNode::Array::size(); }
		void reserve( typename _TNode::Array::size_type count ) { _TNode::Array::reserve( count ); }
	};
	
	template< class _TNode, class _TThis >
	Int8 BaseState< _TNode, _TThis >::compare( const BaseState& rhs ) const
	{				
		if ( size() < rhs.size() )
			return -1;

		if ( size() > rhs.size() )
			return 1;

		const_iterator iState1 = begin();
		const_iterator iState2 = rhs.begin();
		while ( iState1 != end() )
		{
			if ( **iState1 > **iState2 )
				return 1;
			if ( **iState1 < **iState2 )
				return -1;

			//At this point should check the child nodes for equality
			if ( (*iState1)->childCount() < (*iState2)->childCount() )
				return -1;		
			if ( (*iState1)->childCount() > (*iState2)->childCount() )	
				return 1;

			//Now we should check the children directly (We know there are the same number of children btw)
			const_iterator iSubState1 = (*iState1)->childBegin();
			const_iterator iSubState2 = (*iState2)->childBegin();
			while( iSubState1 != (*iState1)->childEnd() )
			{				
				//At this point should check the child nodes for equality
				if ( (**iSubState1) < (**iSubState2) )
					return -1;		
				if ( (**iSubState1) > (**iSubState2) )	
					return 1;

				++iSubState1;
				++iSubState2;
			}

			iState1++; iState2++;
		}
		return 0;
	}




} //END: lexai

#endif