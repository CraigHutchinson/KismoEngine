/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoAI\LexaiParser.h
 * Status: unique (no other copy of this file name in the archive)
 */
/** \author Craig Hutchinson \file 
	\brief 
*/
#ifndef LEXAI_PARSER_H
#define LEXAI_PARSER_H

#include "LexaiGrammar.h"
#include "FrxaiSentence.h"
#include "GraphNode.h"

namespace lexai
{
	//Forward delcaration
	class ParseNode;

	/** \brief Lexical Parser base interface
	*/
	class Parser
	{
	public:
		Parser( Grammar& grammar, Lexicon& lexicon );

		virtual Bool parse( const Sentence& sentence ) = 0;

	protected:

		/** \brief Get a reference to the grammar in use
		*/
		Grammar& getGrammar()
		{ return m_grammar; }

		/** \brief Get a reference to the lexicon in use
		*/
		Lexicon& getLexicon()
		{ return m_lexicon; }

	private:
		Grammar& m_grammar; ////< The grammar for the parser
		Lexicon& m_lexicon; ////< The lexicon for the parser

	};


} //END: lexai

#endif