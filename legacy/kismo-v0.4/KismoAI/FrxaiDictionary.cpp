/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoAI\FrxaiDictionary.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
/** \author Craig Hutchinson \file 
	\brief 
*/
#include "FrxaiDictionary.h"

namespace frxai
{
	Dictionary::Dictionary() :
		WildStar( lookup( "*" ) ),
		WildQuestion( lookup( "?" ) ),
		WildExclamation( lookup( "!" ) ),
		WildDollar( lookup( "$" ) ),
		Root( lookup( "root" ) )
	{}

	Dictionary::~Dictionary()
	{
		for( MapStringToWord::iterator iImpl = m_words.begin(); iImpl != m_words.end(); ++iImpl )
			delete iImpl->second; 
	}

	/** \brief Lookup the word int he dictionary and return a word for it.
		\remarks if word does not exist then it will be added to the dictionary
	*/
	Word Dictionary::lookup( const String& word )
	{
		MapStringToWord::const_iterator iFind = m_words.find( word );
		if ( iFind != m_words.end() )
		{
			assert( iFind->second );
			return Word( iFind->second );
		}
		else
		{
			m_words.insert( MapStringToWord::value_type( word, new WordImpl( word ) ) );
			return Word( m_words[ word ] );
		}
	}

} //END: frxai