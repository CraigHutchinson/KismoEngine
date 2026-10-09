/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoAI\PathaiPath.h
 * Status: unique (no other copy of this file name in the archive)
 */
/** \author Craig Hutchinson \file PathaiState.h
	\brief The base interface for a path finding State node
*/
#ifndef PATHAI_PATH_H
#define PATHAI_PATH_H

#include "PathaiState.h"
#include "TreeNode.h"

namespace pathai
{
	/** \brief Search path totals the cost to reach a state node
	*/
	class Path : public frx::TreeNode< Path >
	{
	public:

		
		struct derefLess : public std::binary_function< Path*, Path*, bool>
		{	// functor for operator<
			bool operator()( const Path* _Left, const Path* _Right) const
			{	// apply operator< to operands
				return _Left->getPriority( goal ) < _Right->getPriority( goal );
			}

			const State* goal; ////< The goal node
		};

		typedef MultiSet< Path*, derefLess > GoalLookup; //
		typedef Set< Path* > Lookup; //

	public:

		/** \brief intial path constructor without a parent node
		*/
		Path( const State* state ) :
			m_state( state )
		{}

		/** \brief Child path to another path on a joining state
		*/
		Path( Path* parentPath, const State* state ) :
			m_state( state )
		{
			setParent( parentPath );
		}

		/** \brief Get the cost of the path
		*/
		Int16 getCost() const
		{ return (getParent() ? getParent()->getCost() : 0 ) + m_state->getCost(); }

		/** \brief Get the heuristic priority of the path
		*/
		Real getPriority( const State* goal ) const
		{ return (Real)getCost() + m_state->getPriority( goal ); }

		/** \brief Get the state the path is on
		*/
		const State* getState() const
		{ return m_state; }

	protected:

		const State* m_state; ///< Pointer ot the state that the path ends on
	};

} //END: pathai

#endif