/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoAI\LexaiSlots.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
/** \author Craig Hutchinson \file 
	\brief 
*/
#include "LexaiSlots.h"

namespace lexai
{
	//-------------------------------------------------------------------------------
	// Slot item implementation
	//-------------------------------------------------------------------------------
	//void SlotItem::getValue( Sentence& sentence ) const
	//{
	//	if ( m_slot )
	//	{
	//		assert( m_value.size() == 1 ); ///Should only ever have 1 item to look for in the slot for now
	//		m_slot->getValue( m_value.front(), sentence ); //Retrieve the sub item data
	//	}
	//	else
	//	{
	//		sentence.push_back( m_value ); ///Append the value to the sentence
	//	}
	//}

	Bool SlotItem::operator == ( const Word& value ) const
	{
		if ( m_value.size() != 0 )
		{
			return m_value.front() == value;
		}
		else
		{
			assert( m_slot );
			return m_slot->findSlot( value ) != m_slot->slotEnd();												
		}
	}

	void Slots::addSlot( const Word& slotName )
	{
		m_slots[ slotName ];		
	}

	void Slots::addSlot( const Word& slotName, const Word& value )
	{
		m_slots[ slotName ].insert(	SlotItem( value ) ); 
	}

	void Slots::addSlot( const Word& slotName, const Sentence& value )
	{
		m_slots[ slotName ].insert(	SlotItem( value ) ); 
	}

	void Slots::addSlot( const Word& slotName, const Slots* slot )
	{
		m_slots[ slotName ].insert(SlotItem( slot ) ); 
	}

	void Slots::addSlot( const Word& slotName, const Slots* slot, const Word& subSlot )
	{
		m_slots[ slotName ].insert(	SlotItem( slot, subSlot ) ); 
	}

	bool Slots::outputSlots( std::ostream& os ) const
	{	
		static Index indentPos = 0;
		static Char indentTxt[256] = {'\0'};
		indentTxt[ indentPos ] = '\t';
		indentTxt[ ++indentPos ] = '\0';

		for ( Slots::SlotMap::const_iterator iSlot = m_slots.begin(); iSlot != m_slots.end(); ++iSlot )
		{
			if ( iSlot->first )
				os << '\n' << indentTxt << "( " << iSlot->first << ' ';

			//Output the item data for the slot
			outputSlotItems( os, iSlot );

			if ( iSlot->first )
				os << " )";
		}

		indentTxt[ --indentPos ] = '\0';
		return true;
	}

	bool Slots::outputSlotItems( std::ostream& os, const Word& slotName ) const
	{		
		const Slots::slot_iterator& iSlot = findSlot( slotName );

		//assert( iSlot != m_slots.end() );
		if ( iSlot == m_slots.end() )
			return false;
		else
			return outputSlotItems( os, iSlot );
	}

	bool Slots::outputSlotItems( std::ostream& os, const Slots::slot_iterator& iSlot ) const
	{
		assert( iSlot != m_slots.end() );


		//Output all the items for the slot
		for ( Slots::SlotItemSet::const_iterator iSlotItem = iSlot->second.begin(); iSlotItem != iSlot->second.end(); ++iSlotItem )
		{
			const Slots* subSlot = iSlotItem->getSlot();
			if ( subSlot ) //If the slot refers to data in a sub slot
			{

				const Sentence& subSlotValue = iSlotItem->getValue();
				
				//refers to specific data in the sub slot
				if ( subSlotValue.size() )
				{
					assert( subSlotValue.size() == 1 );
					subSlot->outputSlotItems( os, *subSlotValue.begin() );
				}
				else
				{
					subSlot->outputSlots( os ); //Get all the slot data
				}
				
			}
			else
			{
				os << "( ";
				os << iSlotItem->getValue();
				os << " )";
			}			
			
		}

		return true;
	}

} //END: lexai