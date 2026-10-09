/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoAI\LexaiGrammar.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
/** \author Craig Hutchinson \file 
	\brief 
*/
#include "LexaiGrammar.h"

namespace lexai
{
	IMPLEMENT_RTTI_BASE( lexai, Grammar );

	Count GrammarRule::match( Dictionary& dictionary, Word::Array::const_iterator begin, Word::Array::const_iterator end ) const
	{
		Word::Array::const_iterator iComp = getComponents().begin();
		Word::Array::const_iterator iWord = begin;
		
		Count matches = 0;

		while ( iComp != getComponents().end() && iWord != end )
		{
			UInt8 matcher = 0;
			if ( *iComp == dictionary.WildStar )
			{
				/** \brief If the grammar rule has a '*' wildcard the preceding word type should be matched
				for any number of times */
				matcher = UInt8_MAX;
			}
			else
			if ( *iComp == dictionary.WildQuestion )
			{
				/** \brief If the grammar rule has a '?' wildcard the preceding word type should be matched
				once */
				matcher = 1;
			}

			if ( matcher )
			{
				++iComp; //Move to the next component which should be the category to check
				while ( *iComp == *iWord && matcher )
				{
					++iWord;
					++matches;
					--matcher;

					if ( iWord == end ) //make sure we don't iterate too far!
						break;

					//NOTE: iComp isn't checked for speed so assume the syntax is okay
				}
				++iComp;
				matcher = 0;
			}
			else
			if ( *iComp != *iWord )
			{
				return 0;
			}
			else
			{
				++iWord;
				++iComp;
				++matches;
			}
		}
		return ( iComp == getComponents().end() || iWord == end ? matches : 0 );
	}

	
	bool GrammarRule::matchSlots( ParseNode* node ) const
	{
		PROFILE;

		//Iterate through all the rules slots
		for ( SlotMap::const_iterator iSlot = slotBegin(); iSlot != slotEnd(); ++iSlot )
		{
			const Word& slotName = iSlot->first;
			
			//now go through all the items and bind them to the slot for the new parse node	
			for ( SlotItemSet::const_iterator iSlotItem = iSlot->second.begin(); iSlotItem != iSlot->second.end(); ++iSlotItem )
			{
				Sentence::const_iterator iValue = iSlotItem->getValue().begin();
				const Word& childCategory = *iValue;
				const ParseNode* sourceNode = node->findChild( childCategory );
				if ( sourceNode ) ///Check if the rule matched this node
				{
					 //if a wildcard is used we take al the child semantic slots
					if ( slotName == m_grammar.getDictionary().WildStar )
					{
						for ( SlotMap::const_iterator iSubSlot = sourceNode->slotBegin(); iSubSlot != sourceNode->slotEnd(); ++iSubSlot )
						{
							node->addSlot( iSubSlot->first, sourceNode, iSubSlot->first );						
						}
					}
					else
					{				
						if ( ++iValue != iSlotItem->getValue().end() )
							node->addSlot( slotName, sourceNode, *iValue );
						else
							node->addSlot( slotName, sourceNode );
					}
				}
				else
				{
					//The child is not found therefore we assume its a wildcard for now....
				}
			}
		}
		
		return true;
	}

	Grammar::Grammar( Dictionary& dictionary, GrammarLoader* loader ) :
		m_dictionary( dictionary ),
		m_loader( loader )
	{}

	Bool Grammar::load()
	{
		if ( !m_loader )
		{
			logRecoverable( "No grammar loader has been specified - grammar cannot be (re)loaded" );
			return false;
		}
		//Clear the grammar
		m_rules.clear();

		if ( m_loader->load( *this ) )
		{
			logSuccess( "The grammar load from '" << m_loader->getFilename() << "' successfully" );
			return true;
		}
		else
		{
			logRecoverable( "The grammar failed to load from '" << m_loader->getFilename() << "'" );
			return false;
		}
	}

	Bool Grammar::add( const Word& ruleID, const GrammarRule& meta  )
	{
		//TODO: could check the rule name is unique

		//We need to add the new rule data			
		m_rules.insert( RuleMetaMap::value_type( ruleID, meta ) );
		return true;
	}

} //END: lexai