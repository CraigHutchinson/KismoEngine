/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoAI\OpsaiMatcher.h
 * Status: unique (no other copy of this file name in the archive)
 */
/** \author Craig Hutchinson \file 
	\brief 
*/
#ifndef OPSAI_MATCHER_H
#define OPSAI_MATCHER_H

#include "OpsaiWorldState.h"

namespace opsai
{
	/** \brief Matcher allows for wildcard comparisons between states and stores valus of wilcard ID's for 
		matching until the next matcher call is used
	*/
	class Matcher 
	{
	public:

		/** \brief Constructor
		*/
		Matcher();

		/** \brief clears any existing matched values
			\remarks called by any 'match*()' function
		*/
		void clear();

		/** \brief Matches a single state with another
			\param[in] set The state to match which will often contain wildcards
			\param[in] db The state which will be matched against
			\return Whether the states match
			\remarks Will clear any previous call to match*()
		*/
		Bool match( const State& set, const State& db );

		/** \brief Matches a single state against a database
			\param[in] set The state to match which will often contain wildcards
			\param[in] dbStart Begin() iterator of the state database which will be matched against
			\param[in] dbEnd End() iterator of the state database which will be matched against
			\return The number of matches that were found
			\remarks Will clear any previous call to match*()
		*/
		Count match( const State& set, WorldState::const_iterator dbStart, WorldState::const_iterator dbEnd  );

		/** \brief Matches every occurance of the preconditions with the state array
			\param[in] set The set of values to match
			\param[in] dbStart Begin() iterator of the state database which will be matched against
			\param[in] dbEnd End() iterator of the state database which will be matched against
			\return The number of matches that were found
			\remarks Will clear any previous call to match*()
		*/
		Count matchEvery( const State::Array& set, WorldState::const_iterator dbStart, WorldState::const_iterator dbEnd );

		/** \brief Gets the number of matches from the last search
			\remarks The number of matches is calculated from the number of occurances of each matched 'State'
			which will give all the possible combinations of each. Ofcourse if 1 of them is noe found (0) the answer will be 0!
		*/
		Count getMatchedCount() const
		{ return (Count)m_matches.size(); }

		/** \brief Helper function returns the words that were matched with teh spcified wildcard
			\remarks This will assert if the wildcard does not exist in the previous search!
		*/
		//const Word::Array& getMatchesFor( const Word& wildCard ) const;

		/** \brief get a reference to the matches
		*/
		const State::MatchArray& getMatches() const
		{ return m_matches; }

		friend std::ostream& operator << ( std::ostream& os, const Matcher& node );

	protected:
		State::MatchArray m_matches; ////< Map of the match wildcard to the matched values
	};

} //END: opsai

#endif