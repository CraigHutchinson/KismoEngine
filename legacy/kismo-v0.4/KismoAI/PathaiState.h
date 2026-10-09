/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoAI\PathaiState.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef PATHAI_STATE_H
#define PATHAI_STATE_H

#include "PathaiCommon.h"

namespace pathai
{
	/** \brief Base Path search state
	*/
	class State
	{
	public:
		
		typedef Vector< const State* > Array;
		
	public:

		/** \brief COnstructor
			\param[in] cost The cost for ths state
		*/
		State( Int8 cost = 0 ) :
			m_cost( cost )
		{}

		/** \brief Get the cost of the state
		*/
		Int8 getCost() const
		{ return m_cost; }

		/** \brief Set the cost of the state
		*/
		void setCost( Int8 cost )
		{ m_cost = cost; }

		/** \brief Get the heuristic priority of the state
		*/
		virtual Real getPriority( const State* goal ) const = 0;

	protected:
		Int8 m_cost; ////< The cost of the state where 0 is normal

	};

} //END: lexai

#endif