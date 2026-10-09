/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoAI\OpsaiOperator.h
 * Status: unique (no other copy of this file name in the archive)
 */
/** \author Craig Hutchinson \file 
	\brief 
*/
#ifndef OPSAI_OPERATOR_H
#define OPSAI_OPERATOR_H

#include "OpsaiWorldState.h"
#include "named.h"

namespace opsai
{
	//Forward delcarations
	class Matcher;

	class Operator : public frx::Named
	{
	public:

		typedef Vector< Operator* > Array; ////< Array of operator pointers

	protected:
		
		/** \brief Structure for holding the location of a wildcard within an array
			\remarks This is used for locating the wildcard values within then operator wihtout having to check every state and word
		*/
		struct WildCardSubIndex 
		{
			Index stateIndex; ////< The index of the state that the wildcard is within
			Index wordIndex; ////< The index of the word within the state that is the wildcard
			WildCardSubIndex( Index state, Index word ) : stateIndex( state ), wordIndex( word ) {}

		};

		typedef Vector< WildCardSubIndex> WildCardMatches; ////< Stores the position of all the filecard matches wihtin the state array
		typedef frx::SmallMap< Word, WildCardMatches > WildCardLookup; ///< finds the location of a wildcard

	public: 

		/** \brief Operator constructor takes a name which is used to identify the operator for debugging
		*/
		Operator( const String& name );

		/** \brief Operator constructor
			\param[in] name Name used for identifying the operastor for debugging
			\param[in] txt A state which is used to match values for debug output of the operators action
			\param[in] pre Array of precondition states including wildcards that must be matched
			\param[in] del If the rpeconditions match the del states will be removed form the world state
			\param[in] add If the rpeconditions match the add states will be added to the world
		*/
		Operator( const String& name, const Sentence& txt, const State::Array& pre, const State::Array& del, const State::Array& add );

		/** \brief Verifies the preconditions against a world state split into the dynamic and 
			static states
			\param[in] matcher reference to the matcher instance we will use
			\[aram[in] worldState The static and dynamic states of the world
		*/
		Bool validatePreconditions( Matcher& matcher, const WorldState& worldState ) const;


		/** \brief Creates a new successor state by modifying the worldState paramater
			\param[in/out] worldState The world state to apply the operator to
			\param[in] match The wildcard match values for filling in the addition and deletion state arrays
		*/
		Bool createSuccessorState( WorldState& worldState, const State::Match& match ) const;

		/** \brief Get the operators preconditions
		*/
		const State::Array& getPreconditions() const
		{ return m_preconditions; }

		/** \brief Sets the operators preconditions
		*/
		void setPreconditions( const State::Array& pre )
		{ m_preconditions = pre; }

		/** \brief Get the operators deletions
		*/
		const State::Array& getDeletions() const
		{ return m_deletions; }

		/** \brief Sets the operators deletions
		*/
		void setDeletions( const State::Array& del )
		{ m_deletions = del; updateWildcardLookup( m_deletions, m_deletionWildcards ); }

		/** \brief Get the operators additions
		*/
		const State::Array& getAdditions() const
		{ return m_additions; }

		/** \brief Sets the operators additions
		*/
		void setAdditions( const State::Array& add )
		{ m_additions = add; updateWildcardLookup( m_additions, m_additionWildcards ); }

		/** \brief gets the operators text whith wildcards matched with the supplied data
		*/
		Sentence getText( const State::Match& matches ) const;

	protected:

		/** \brief Helper function for generating a fast lookup map for replacing wildcards within the
			deletion and addition state arrays
		*/
		void updateWildcardLookup( const State::Array& states, WildCardLookup& lookupMap );

	protected:

		Sentence m_text; ////< The debug text output for the operation
		State::Array m_preconditions; ////< Array of precondition states which may include wildcards
		State::Array m_deletions; ////< Array of states to delete
		State::Array m_additions; ////< Array of addition states which matched widlcards where be replaced with those found in the preconditions when inserted

		WildCardLookup m_deletionWildcards; ////< Map of wildcards to their index within the deletion state array
		WildCardLookup m_additionWildcards; ////< Map of wildcards to their index within the addition state array

	};

} //END: opsai

#endif