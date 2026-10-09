/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoAI\PathaiSearch.h
 * Status: unique (no other copy of this file name in the archive)
 */
/** \author Craig Hutchinson \file 
	\brief 
*/
#ifndef PATHAI_SEARCH_H
#define PATHAI_SEARCH_H

#include "PathaiState.h"
#include "PathaiSpace.h"

namespace pathai
{

	/** \brief Base path search interface
	*/
	class Search
	{
	public:

		Search( Space& searchSpace ) :
			m_searchSpace( searchSpace )
		{}


		/** \brief Search interface function
			\param[in] start the starting state
			\param[in] goal The target goal state
			\returns Whether the goal was reached successfully
		*/
		virtual Bool search( const State* start, const State* goal ) = 0;

		/** \brief get a reference to the result states for the path
		*/
		const State::Array& getResultStates() const
		{ return m_resultStates; }


	protected:

		Space& m_searchSpace; ////< The search space that is being used for the search
		State::Array m_resultStates; ////< The array of states which will meet the desired goal
		State::Array m_resultClosedStates; ////< The array of states that were closed in the search
		State::Array m_resultOpenStates; ////< The array of states that were open when the search finished

	};

} //END: pathai

#endif