/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoAI\LexaiCommon.h
 * Status: unique (no other copy of this file name in the archive)
 */
/** \author Craig Hutchinson \file 
	\brief 
*/
#ifndef LEXAI_COMMON_H
#define LEXAI_COMMON_H

#include "FrxaiWord.h"
#include "FrxaiDictionary.h"
#include "FrxaiSentence.h"

/** \brief The lexai namespace continst the lexical parseing and NLP classes
*/
namespace lexai
{
	//Bring the commong utility clases in to the lexical AI namespace
	typedef frxai::Word Word;
	typedef frxai::Dictionary Dictionary;
	typedef frxai::Sentence Sentence;

} //END: lexai

#endif