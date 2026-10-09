/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoAI\OpsaiSearch.h
 * Status: unique (no other copy of this file name in the archive)
 */
/** \author Craig Hutchinson \file 
	\brief 
*/
#ifndef OPSAI_SEARCH_H
#define OPSAI_SEARCH_H

#include "OpsaiOperatorStore.h"
#include "OpsaiWorldState.h"

namespace opsai
{
	/** \brief Base searcjh interface
	*/
	class Search
	{
	public:

		/** \brief Stores information for the reuslting operator for a search and the match wildcard values
		*/
		struct OperatorMatch
		{
			const Operator* op; ////< the operator
			State::Match match; ////< The matcher match results whihch can be used with the operator to output information etc
			OperatorMatch( const Operator* oper, const State::Match& matched ) : op( oper ), match( matched ) {}
		};

		typedef Vector< OperatorMatch > OperatorResults; ////< Array of results

	public:

		Search() : m_resultState(0) {}

		/** \brief Clears the search results
		*/
		void clear()
		{
			m_resultOperators.clear();
		}

		/** \brief Performs a search
			\returns Whether the goals can be accoumplished
		*/
		virtual Bool search( const WorldState& start, const State::Array& goals, const OperatorStore& operators ) = 0;

		/** \brief Gets a reference ot the array of operators which fullfill the desired goal search
		*/
		const OperatorResults& getOperators() const
		{ return m_resultOperators; }

		/** \brief Gets a reference to the resulting worldState which meets the desired goals
		*/
		const WorldState& getResultState() const
		{ return m_resultState; }

	protected:
		
		WorldState m_resultState; ////< The state of the resulting world after the search has reached the goal
		OperatorResults m_resultOperators; ////< Array of operators that are the result of the search
	};

} //END: opsai

#endif