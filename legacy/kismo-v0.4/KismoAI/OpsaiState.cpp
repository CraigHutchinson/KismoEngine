/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoAI\OpsaiState.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
/** \author Craig Hutchinson \file 
	\brief 
*/
#include "OpsaiState.h"

namespace opsai
{
	State::State() :
		Sentence()
	{
		m_hash.value = 0;
		m_wildCards = 0;
	}

	State::State( Dictionary& dictionary ) :
		Sentence( dictionary )
	{
		m_hash.value = 0;
		m_wildCards = 0;
	}

	State::State( Dictionary& dictionary, const String& sentence, Char delim ) :
		Sentence( dictionary, sentence, delim )
	{
		processWildCards();		
	}

	State::State( const Sentence& words  ) :
		Sentence( words )		
	{
		processWildCards();	
	}

	Bool State::set( const Sentence& words )
	{
		//Store the words into this
		*(Sentence*)this = words;

		//return whether the wildcards are processed ok
		return processWildCards();
	}

	Bool State::processWildCards()
	{
		Count size = (Count)this->size();
		assert( size <= MaxWords );

		//Clear the wildcard data		
		m_hash.value = 0;
		m_wildCards = 0;

		//Now to generate the hash
		//For now this is a simple pack of integers
				for ( Index i = 0; i < size; ++i )
		{
			//Assign the word ID or if the index is invalid assign 0 in the hash
			m_hash.wids[ i ] = (Word::WID)at(i);
			m_wildCards[ i ] = (((const String&)at(i))[0] == '?');
		}

		return true;
	}

	Bool State::match( const State& db, Match& matches ) const
	{
		if ( size() != db.size() ) //If the number of words in each satte is different we don't have a match!
			return false; 

		//If there are no wildcards we just do a stright forwards compare
		if ( m_wildCards == 0 )
			return m_hash == db.m_hash;

		//Conver the wildcard flags to a bitmask for fast compare
		HashValue mask = getWildCardMask();

		//Perform a match using the mask		
		return match( mask, db,  matches );
	}

	Bool State::matchWildcards(  const State& db, Match& matches ) const
	{
		Count size = (Count)this->size();

		//So the states match
		//Now we take the wildcard words and add the matches to the 'matches' map for later use
		for ( Index i = 0; i < size; ++i )
		{
			if ( m_wildCards[ i ] ) //Check if this is a wildcard parameter
			{
				Match::const_iterator iMatch = matches.find( at( i ) );
				if ( iMatch == matches.end() )
				{
					//Store the wildCard in the map and insert the value into the value vector
					matches[ at( i ) ] = db.at( i );
				}
				else
				{
					//The word is already matched so we can use the value to verify that 
					// this is a valid match substituting the previous value for the wildcard
					if ( iMatch->second != db.at( i ) )
					{
						return false; ////< not a match!
					}		
					else
					{
						//Continue matching any new values
					}
				}
			}
		}

		return true;
	}

} //END: opsai