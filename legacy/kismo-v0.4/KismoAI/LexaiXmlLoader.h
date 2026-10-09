/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoAI\LexaiXmlLoader.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef LEXAI_XMLLOADER_H
#define LEXAI_XMLLOADER_H

#include "LexaiGrammar.h"
#include "LexaiLexicon.h"
#include "FileParser.h"

namespace lexai
{
	/** \brief XML lexicon and grammar loader
	*/
	class XmlLoader : public GrammarLoader, public LexiconLoader
	{
	public:
		XmlLoader( const String& filename );

		/** \brief Parse grammer
		*/
		Bool load( Grammar& grammar );

		/** \brief parse lexicon
		*/
		Bool load( Lexicon& lexicon );
	
	protected:
		
		Bool parseRule( io::FileParser::Node node, Grammar& grammar );
		
		Bool parseWord( io::FileParser::Node node, Lexicon& lexicon );
	};

} //END: lexai

#endif