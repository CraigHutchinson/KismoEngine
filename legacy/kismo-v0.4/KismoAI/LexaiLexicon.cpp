/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoAI\LexaiLexicon.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
/** \author Craig Hutchinson \file 
	\brief 
*/
#include "LexaiLexicon.h"

namespace lexai
{
	IMPLEMENT_RTTI_BASE( lexai, Lexicon );

	Lexicon::Lexicon( Dictionary& dictionary, LexiconLoader* loader ) :
		m_dictionary( dictionary ),
		m_loader( loader )
	{}

	Bool Lexicon::load()
	{
		if ( !m_loader )
		{
			logRecoverable( "No lexicon loader has been specified - lexico cannot be (re)loaded" );
			return false;
		}

		//Clear the lexicon
		m_words.clear();

		if ( m_loader->load( *this ) )
		{
			logSuccess( "The lexicon load from '" << m_loader->getFilename() << "' successfully" );
			return true;
		}
		else
		{
			logRecoverable( "The lexicon failed to load from '" << m_loader->getFilename() << "'" );
			return false;
		}
	}

	Bool Lexicon::add( const Word& word, const LexiconEntry& wordLexicon )
	{
		//See if we have any information about it
		WordMetaMap::iterator iFind = m_words.find( word );
		if ( iFind != m_words.end() )
		{
			//Make sure the lexicon doesn't try and define the same word and category twice
			//for ( LexiconEntry::Array::const_iterator iDef = iFind->second.begin();
			//	iDef != iFind->second.end();
			//	++iDef )
			//{
			//}


			//Insert the new lexcon information about the word
			iFind->second.push_back( wordLexicon );

			return false;
		}


		//We need to add the new data			
		m_words[ word ].push_back( wordLexicon );

		//Add the workd to the lexicon entry slots so that it can be visible in the resulting output
		Word root( getDictionary().lookup("root") );
		m_words[ word ].back().addSlot( root, word );
		return true;
	}

	const LexiconEntry::Array& Lexicon::lookup( const Word& word )
	{
		//See if we have any information about it
		WordMetaMap::const_iterator iFind = m_words.find( word );
		if ( iFind != m_words.end() )
		{
			return iFind->second;
		}

		return NullEntry; ///< Return null
	}

	bool LexiconEntry::matchSlots( Slots* node ) const
	{
		PROFILE;

		//Iterate through all the slots
		for ( SlotMap::const_iterator iSlot = slotBegin(); iSlot != slotEnd(); ++iSlot )
			node->addSlot( iSlot->first, this, iSlot->first );
		
		return true;
	}


} //END: lexai