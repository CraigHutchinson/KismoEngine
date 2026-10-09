/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoAI\LexaiParser.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
/** \author Craig Hutchinson \file 
	\brief 
*/
#include "LexaiParser.h"

namespace lexai
{
	Parser::Parser( Grammar& grammar, Lexicon& lexicon ) :
		m_grammar( grammar ), m_lexicon( lexicon )
	{}

} //END: lexai