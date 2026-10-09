/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoAI\OpsaiBreadthSearch.h
 * Status: unique (no other copy of this file name in the archive)
 */
/** \author Craig Hutchinson \file 
	\brief 
*/
#ifndef OPSAI_BREADTHSEARCH_H
#define OPSAI_BREADTHSEARCH_H

#include "OpsaiSearch.h"

namespace opsai
{
	class BreadthSearch : public Search
	{
	public:
		/** \brief Performs a search
		*/
		virtual Bool search( const WorldState& start, const State::Array& goals, const OperatorStore& operators );

	protected:

		/** \brief Internal search implementation
			\returns The resulting WorldState that meets the required goals
		*/
		const WorldState* search( WorldState::PtrBuffer& ready, WorldState::PtrLookup& visited, const State::Array& goals, const OperatorStore& operators );
	
	};

} //END: opsai

#endif