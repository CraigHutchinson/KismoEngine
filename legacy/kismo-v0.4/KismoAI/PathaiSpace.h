/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoAI\PathaiSpace.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef PATHAI_SPACE_H
#define PATHAI_SPACE_H

#include "PathaiState.h"

namespace pathai
{

	/** \brief base search space interface for creating successors
	*/
	class Space
	{
	public:

		/** \brief Get shte successor states from a state that is passed in
			\returns the number of suiccessors that were found
			\param[out] successors The successor array
			\param[in] source Teh source state which to find successors from
		*/
		virtual Count getSuccessors( const State* source, State::Array& successors ) = 0;

	};


} //END: lexai

#endif