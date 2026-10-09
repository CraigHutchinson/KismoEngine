/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoAI\OpsaiMatcher.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
/** \author Craig Hutchinson \file 
	\brief 
*/
#include "OpsaiMatcher.h"

namespace opsai
{
	Matcher::Matcher()
	{
	}

	void Matcher::clear()
	{
		m_matches.clear();
	}

	Bool Matcher::match( const State& set, const State& db )
	{
		clear();
		
		State::Match match;
		if ( !set.match( db, match ) )
			return false;
		
		m_matches.push_back( match );
		return true;
	}

	Count Matcher::match( const State& set, WorldState::const_iterator dbIterator, WorldState::const_iterator dbEnd )
	{
		clear();

		m_matches.reserve( 8 ); ////< Save re-allocating the array. We aren't too worried about memory limits!

		State::Match match;
		while ( dbIterator != dbEnd )
		{
			match.clear(); //Make sure the match results are clear
			//Perform the match
			if ( set.match( (*dbIterator), match ) )
			{			
				State::MatchArray::const_iterator iMatch = m_matches.begin();
				//If the match succeeds first check that the same one doesn't already exist
				for (; iMatch != m_matches.end(); ++iMatch )
				{
					if ( *iMatch == match ) break;
				}

				//If we didn't find an existing match thats the same we add the new matched wildcards
				if ( iMatch == m_matches.end() )
					m_matches.push_back( match );
			}

			++dbIterator;
		}

		return (Count)m_matches.size();
	}

	Count Matcher::matchEvery( const State::Array& set, WorldState::const_iterator dbStart, WorldState::const_iterator dbEnd )
	{
		State::Array::const_iterator iSet = set.begin();

		//If we havn't checked the first precondition we need to intialise the search space with the possible solutions
		//Match this state
		if ( !match( (*iSet), dbStart, dbEnd ) )
		{
			return 0; ////< We failed to match the first level in the search set!
		}

		++iSet;

		//Temporary store for the match results
		State::MatchArray tmatches;

		//Temporary store for match results
		State::Match match;
		match.reserve( 4 );

		//Loop through the precondition set
		for (; iSet != set.end() && m_matches.size() != 0; ++iSet )
		{
			State::HashValue wildcardMask = iSet->getWildCardMask();

			//If we are here have have passed the first precondition so should have the base
			// state match form the previous states which we need to check can match the worldState aswell

			//Create a copy of the original states whihc we won't modify as this is the basis of each iteration
			tmatches = m_matches;

			//Clear the permenant store
			m_matches.clear();

			//Perform the match using the previous matched values
			// if the match conflicts with these values false will be returned as expected and the invalid
			// match removed fromt he eprmenant match store
			for ( State::MatchArray::const_iterator iMatch = tmatches.begin();
				iMatch != tmatches.end(); ++iMatch )
			{

				//Loop through the world states
				for ( WorldState::const_iterator dbIterator = dbStart; dbIterator != dbEnd; ++dbIterator )
				{						

					match = *iMatch;
					if ( (*iSet).match( wildcardMask, (*dbIterator), match ) )
					{			
						State::MatchArray::const_iterator iMatch = m_matches.begin();
						//If the match succeeds first check that the same one doesn't already exist
						for (; iMatch != m_matches.end(); ++iMatch )
						{
							if ( *iMatch == match ) break;
						}

						//If we didn't find an existing match thats the same we add the new matched wildcards
						if ( iMatch == m_matches.end() )
							m_matches.push_back( match );
					}

				}///Iterate world state

			} //Iterate the match
		}

		return (Count)m_matches.size();
	}

	//const Word::Array& Matcher::getMatchesFor( const Word& wildCard ) const
	//{
	//	//Debug check to make sure the wildcard exists in the previous search!
	//	State::MatchArray::const_iterator iFind = m_matches.find( wildCard );
	//	assert( iFind != m_matches.end() );
	//	return iFind->second;
	//}

	
	std::ostream& operator << ( std::ostream& os, const Matcher& matcher )
	{
		
		for ( State::MatchArray::const_iterator iMatch = matcher.getMatches().begin();
			iMatch != matcher.getMatches().end();
			++iMatch )
		{
			os << '\t';

			for ( State::Match::const_iterator iWild = iMatch->begin();
				iWild != iMatch->end();
				++iWild )
			{
				os << iWild->first << '=' << iWild->second << '\t';
			}			
			os << '\n';
		}

		return os;
	}


} //END: opsai