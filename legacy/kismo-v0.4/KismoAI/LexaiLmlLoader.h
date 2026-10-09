/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoAI\LexaiLmlLoader.h
 * Status: unique (no other copy of this file name in the archive)
 */
/** \author Craig Hutchinson \file 
	\brief 
*/
#ifndef LEXAI_LMLLOADER_H
#define LEXAI_LMLLOADER_H

#include "LexaiGrammar.h"
#include "LexaiLexicon.h"
#include "FileParser.h"

namespace lexai
{
	/** \brief Custom lml (Language markup language) file format loader for lexicon and grammar
	*/
	class LmlLoader : public GrammarLoader, public LexiconLoader
	{
	public:
		LmlLoader( const String& filename );
		
		/** \brief Parse grammer
		*/
		Bool load( Grammar& grammar );

		/** \brief parse lexicon
		*/
		Bool load( Lexicon& lexicon );
	
		const String& getFilename() const
		{ return GrammarLoader::getFilename(); }

	protected:
		
		Bool parseRule( io::FileParser::Node node, Grammar& grammar );
		
		Bool parseWord( io::FileParser::Node node, Lexicon& lexicon );

	};

} //END: lexai

#endif