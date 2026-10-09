/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoAI\LexaiSlots.h
 * Status: unique (no other copy of this file name in the archive)
 */
/** \author Craig Hutchinson \file 
	\brief 
*/
#ifndef LEXAI_SLOTS_H
#define LEXAI_SLOTS_H

#include "LexaiCommon.h"
#include "SmallSet.h"
#include "SmallMap.h"

namespace lexai
{
	//Forward declarations
	class Slots;

	/** \brief Slot Item data structure
		\remarks This structure holds a pointer to a slot and a word. If the slot is set then the
			item data is to be obtained form that slot. When slot is 0 then the word is taken a a literal value.
			When the slot is set and word isn't then the contents of the slot will be taken, if a word is supplied then
			the information of that value will be obtained
	*/
	class SlotItem
	{
	public:

		/** \brief Constructor takes a pointer to another slot which whos entire value will be 
			used as part of this item			
		*/
		SlotItem( const Slots* subSlot ) : m_slot( subSlot ) {}

		/** \brief Constructor takes a pointer to another slot and the name o the subSlot that the value will be taken from
			for this item
		*/
		SlotItem( const Slots* subSlot, const Word& subSlotName ) : m_slot( subSlot ) 
		{ m_value.push_back( subSlotName ); }

		/** \brief Constructor takes a single value
		*/
		SlotItem( const Word& value ) : m_slot( 0 ) 
		{ m_value.push_back( value ); }

		/** \brief Constructor takes an array of words
		*/
		SlotItem( const Sentence& values ) : m_slot( 0 ), m_value( values )
		{}

		/** \brief Get the value(s) of the item building a sentence of the values
			\param[out] sentence The sentence structure to fill with the value for the item
		*/
		//void getValue( Sentence& sentence ) const;

		/** \brief Equality operator checks if this items first piece of data equals 'value'
			\param[in] value The word to compare the value too
		*/
		Bool operator == ( const Word& value ) const;
		
		/** \brief Inequality operator checks if this slot is equal to another
		*/
		Bool operator != ( const SlotItem& value ) const
		{ 
			if ( m_slot != value.m_slot ||
				m_value.size() != value.m_value.size() ||
				m_value.size() && m_value.front() != value.m_value.front() ) return true;

			return true; 
		}

		const Slots* getSlot() const
		{ return m_slot; }

		const Sentence& getValue() const
		{ return m_value; }

	private:
		const Slots* m_slot; ////< (optional) slot pointer to a sub slot
		Sentence m_value; ////< (optional) name of a sub slot or holds direct value(s) depending on m_slot being specified
	};

	/** \brief Slot class implementes a slot like structures like the lKit slots
	*/
	class Slots
	{
	public:		

		/** \brief Vector of slot items as it is possible for a slot to hold 0 or multiple pieces of data
			\remarks As a set which is indexed by first value of the item
			\remarks Items which get all the sub slot data (ie have no value) are indexed by the slot
		*/
		typedef frx::SmallSet< SlotItem > SlotItemSet;

		/** \brief Small map stores slot names and there item data
		*/
		typedef frx::SmallMap< Word, SlotItemSet > SlotMap;

		typedef SlotMap::const_iterator slot_iterator;
		typedef SlotItemSet::const_iterator slotItem_iterator;


	public:

		/** \brief Add a slot who only has a slot name and no item value
			\param[in] slotName The name given to the new slot
		*/
		void addSlot( const Word& slotName );

		/** \brief Add a slot that has a name and a value literal
			\param[in] slotName The name given to the new slot
			\param[in] value The string value given/added to the slot
		*/
		void addSlot( const Word& slotName, const Word& value );

		/** \brief Add a slot that has a name and an array of values (sentence)
			\param[in] slotName The name given to the new slot
			\param[in] value The sentence value given/added to the slot
		*/
		void addSlot( const Word& slotName, const Sentence& value );

		/** \brief Add a slot who's data is that of another Slots instance
			\param[in] slotName The name given to the new slot
		*/
		void addSlot( const Word& slotName, const Slots* slot );

		/** \brief Add a slot who's data is that of another Slots instance
			\param[in] slotName The name given to the new slot
		*/
		void addSlot( const Word& slotName, const Slots* slot, const Word& subSlot );

		/** \brief Finds a slot by name
			\param[in] slotName The name of the slot to find
			\returns An iterator to the slot of the specified name or slotEnd() if not found
		*/
		slot_iterator findSlot( const Word& slotName ) const
		{ return m_slots.find( slotName ); }

		/** \brief Returns the iterator for the first slot used for iterating the slots
		*/
		slot_iterator slotBegin() const
		{ return m_slots.begin(); }

		/** \brief Returns the iterator for the first child used for iterating the nodes
		*/
		slot_iterator slotEnd() const
		{ return m_slots.end(); }

		

		bool outputSlots( std::ostream& os ) const;
		bool outputSlotItems( std::ostream& os, const Word& slotName ) const;
	
	protected:

		bool outputSlotItems( std::ostream& os, const Slots::slot_iterator& iSlot ) const;

	private:


		SlotMap m_slots; ////< Stores the slots for this item
	}; 

} //END: lexai

#endif
