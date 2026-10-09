/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoAI\FrxaiDictionary.h
 * Status: unique (no other copy of this file name in the archive)
 */
/** \author Craig Hutchinson \file 
	\brief 
*/

#ifndef LEXAI_DICTIONARY_H
#define LEXAI_DICTIONARY_H

#include "FrxaiWord.h"

namespace frxai
{
	class Dictionary
	{

	public:
		Dictionary();
		
		~Dictionary();

		/** \brief Lookup the word int he dictionary and return a word for it.
			\remarks if word does not exist then it will be added to the dictionary
		*/
		Word lookup( const String& word );

	private:
		typedef Map< String, WordImpl* > MapStringToWord;
		MapStringToWord m_words;

		
	public:
		//Const word declared last as the word map needs to be created first
		const Word WildStar;
		const Word WildQuestion;
		const Word WildExclamation;
		const Word WildDollar;
		const Word Root;
	};


} //END: frxai

#endif