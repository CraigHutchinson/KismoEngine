/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoAI\FrxaiSentence.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
/** \author Craig Hutchinson \file 
	\brief 
*/
#include "FrxaiSentence.h"

namespace frxai
{

	Sentence::Sentence( Dictionary& dictionary, const String& sentence, Char delim ) :
		m_dictionary( &dictionary )
	{
		parse( sentence, delim );
	}

	Sentence::Sentence( Dictionary& dictionary ) :
		m_dictionary( &dictionary )
	{
	}

	Sentence::Sentence( const Sentence& sentence ) :
		m_dictionary( sentence.m_dictionary ),
		Word::Array( sentence )
	{
	}

	void Sentence::parse( const String& sentence, Char delim )
	{	
		clear(); ///make sure the sentence is cleared

		if ( !m_dictionary )
		{
			logError( "Phrase could not be parsed as no Dictionary has been supplied!" );
			return;
		}

		if ( !sentence.length() )
		{
			logError( "The supplied sentence is empty!" );
			return;
		}

		//parse the string into an array of parse nodes
		Index wordStart = 0;
		Index wordEnd = 0;
		while ( wordEnd <= sentence.length() )
		{
			wordEnd = (Index)sentence.find( delim, wordStart );
			if ( wordEnd != wordStart ) ////< Don't insert empty strings
			{
				String subStr = sentence.substr( wordStart, wordEnd - wordStart );
				if ( subStr.length() ) //Make sure we don't insert blank strings
					push_back( m_dictionary->lookup( subStr ) );
			}
			wordStart = wordEnd+1;
		}
	}

	void Sentence::replace( const ReplaceMap& matches )
	{
		for ( ReplaceMap::const_iterator iMatch = matches.begin(); iMatch != matches.end(); ++iMatch )
		{
			for ( iterator iWord = begin(); iWord != end(); ++iWord )
			{
				if ( *iWord == iMatch->first  )// If the word matches
					*iWord = iMatch->second; //Replace it
			}
		}
	}

	std::ostream& operator << ( std::ostream& os, const Sentence& sentence )
	{
		Sentence::const_iterator begin = sentence.begin();
		for ( Sentence::const_iterator iWord = begin; iWord != sentence.end(); ++iWord )
		{
			if ( iWord != begin )
				os << ' ';
			os << *iWord;
		}

		return os;
	}

} //END: frxai