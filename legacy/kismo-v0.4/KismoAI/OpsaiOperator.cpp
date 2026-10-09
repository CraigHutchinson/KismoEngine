/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoAI\OpsaiOperator.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
/** \author Craig Hutchinson \file 
	\brief 
*/
#include "OpsaiOperator.h"
#include "OpsaiMatcher.h"

namespace opsai
{
	Operator::Operator( const String& name ) :
		frx::Named( name )
	{}

	Operator::Operator( const String& name, const Sentence& txt, const State::Array& pre, const State::Array& del, const State::Array& add ) :
		frx::Named( name ),
		m_text( txt ),
		m_preconditions( pre ),
		m_deletions( del ),
		m_additions( add )
	{
		//generate the fast wildcard index map
		updateWildcardLookup( m_deletions, m_deletionWildcards );
		updateWildcardLookup( m_additions, m_additionWildcards );
	}

	void Operator::updateWildcardLookup( const State::Array& states, WildCardLookup& lookupMap )
	{
		lookupMap.clear(); ////< Make sure we start with an empty lookup map

		Index stateIndex = 0;////< Store the current state index
		//iterate over all the states
		for ( State::Array::const_iterator iState = states.begin();
			iState != states.end();
			++iState )
		{
			if ( iState->hasWildcards() ) ////< Check there are wildcards
			{
				//Now iterate over the words in the state finding the wildcards
				for ( Index wordIndex = 0; wordIndex < iState->size(); ++wordIndex )
				{
					if ( iState->isWildcard( wordIndex ) )
					{
						lookupMap[ (*iState)[ wordIndex ] ].push_back( 
							WildCardSubIndex( stateIndex, wordIndex ) );
					}
				}
			}

			++stateIndex; ////< iterate the state index
		}

	}

	Bool Operator::validatePreconditions( Matcher& matcher, const WorldState& worldState ) const
	{
		Bool ret = ( matcher.matchEvery( m_preconditions, worldState.begin(),worldState.end() ) != 0 );
		return ret;
	}

	Sentence Operator::getText( const State::Match& matches ) const
	{
		Sentence text = m_text;
		text.replace( matches );
		return text;
	}
	
	Bool Operator::createSuccessorState( WorldState& worldState, const State::Match& match ) const
	{ PROFILE;
		//Get a copy fo the operators additions and deletions which we will fill wiht the wildcard matches
		State::Array add = getAdditions();
		State::Array del = getDeletions();

		for ( State::Match::const_iterator iMatch = match.begin();
			iMatch != match.end();
			++iMatch )
		{
			//Replace any wildcard in the deletion list with the matched value
			WildCardLookup::const_iterator iWildcards = m_deletionWildcards.find( iMatch->first );
			if ( iWildcards != m_deletionWildcards.end() )
			{
				for ( WildCardMatches::const_iterator iWildLocation = iWildcards->second.begin();
					 iWildLocation != iWildcards->second.end();
					 ++iWildLocation )
				{
					//Replace the wildcard with the matched value
					del[ iWildLocation->stateIndex ].setWord( iWildLocation->wordIndex, iMatch->second ); 
				}
			}

			//Replace any wildcard in the addition list with the matched value
			/*WildCardLookup::const_iterator*/ iWildcards = m_additionWildcards.find( iMatch->first );
			if ( iWildcards != m_additionWildcards.end() )
			{
				for ( WildCardMatches::const_iterator iWildLocation = iWildcards->second.begin();
					 iWildLocation != iWildcards->second.end();
					 ++iWildLocation )
				{
					//Replace the wildcard with the matched value
					add[ iWildLocation->stateIndex ].setWord( iWildLocation->wordIndex, iMatch->second ); 
				}
			}
		}


		//Now we have the matched deletion and addition states we need to apply them to the world state
		Bool ret = worldState.removeAdd( del, add );

		assert( ret );
		return ret;
	}

} //END: opsai