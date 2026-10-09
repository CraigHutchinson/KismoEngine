/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoAI\LexaiBottomUpParser.h
 * Status: unique (no other copy of this file name in the archive)
 */
/** \author Craig Hutchinson \file 
	\brief 
*/
#ifndef LEXAI_BOTTOMUPPARSER_H
#define LEXAI_BOTTOMUPPARSER_H

#include "LexaiParser.h"
#include "LexaiState.h"
#include "LexaiParseNode.h"
#include "CoreMath.h"

namespace lexai
{
	
	/** \brief Lexical Parser base interface
	*/
	class BottomUpParser : public Parser
	{
	public:
		
		class State : public BaseState< ParseNode, State >
		{
		public:
			
			/** \brief Default constructor does nothing!
			*/
			State(){};

			/** \brief Constructor breaks a sentence into a state
			*/
			State( const Sentence& sentence );

			/** \brief Delete the enitre tree of parseNodes
			*/
			void destroyTree();

			friend std::ostream& operator << ( std::ostream& os, const State& state );
	
		};

		typedef Set< State > StateSet;

	public:
		
		/** \brief Constructor
			\param[in] grammar A reference to the grammar rules the partser will use
			\param[in] lexicon A reference to the lexicon the parser will use
		*/
		BottomUpParser( Grammar& grammar, Lexicon& lexicon );

		/** \brief Takes a sentence and parses it using the current grammar and lexicon
		*/
		virtual Bool parse( const Sentence& sentence );

	private:

		/** \brief Debug function to write the results of a search
		*/
		void logStateTree( State state );

		/** \brief Checks if a state has been visited already
		*/
		Bool isVisited( const State& state, const StateSet& visitedStates )
		{ PROFILE; return ( visitedStates.find( state ) != visitedStates.end() ); }
		
		Bool successors( const State& state, State::Array& successorStates, StateSet& visitedStates );
		
		/** \brief Creates a successor using a lexicon entry
		*/
		Bool createSemantics( ParseNode& newNode, const LexiconEntry& lexiconEntry );

		/** \brief Creates a successor using a grammar rule
		*/
		Bool createSemantics( ParseNode& newNode, const GrammarRule& grammarRule );
	
		/** \brief Implementation creates a successor compacting the nodes based on the range supplied
			\remarks If the state exists false will be returned
		*/
		Bool createSuccessor( State& newState, State::Lookup& visitedStates, const State& oldState, State::const_iterator beginCollapse, State::const_iterator endCollapse, Word successorName, ParseNode*& newNode );

	private:		
	
		//ParseNode::Array m_freeList; //Free list to clear up memory once we are done
	};



} //END: lexai

#endif