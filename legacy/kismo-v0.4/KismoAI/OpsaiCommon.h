/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoAI\OpsaiCommon.h
 * Status: unique (no other copy of this file name in the archive)
 */
/** \author Craig Hutchinson \file 
	\brief 
*/
#ifndef OPSAI_COMMON_H
#define OPSAI_COMMON_H

#include "FrxaiWord.h"
#include "FrxaiDictionary.h"
#include "FrxaiSentence.h"

/** \brief The opsai namespace contains classes and code for performing Operator searchs and 
	GPS searches to reach a goal.
	\remarks The opsai namespace also contains the Matcher for now but should be in frxai at some point
*/
namespace opsai
{
	//Bring the commong utility clases in to the operator (GPS) AI namespace
	typedef frxai::Word Word;
	typedef frxai::Dictionary Dictionary;
	typedef frxai::Sentence Sentence;

} //END: lexai

#endif