/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoAI\FrxaiSentence.h
 * Status: unique (no other copy of this file name in the archive)
 */
/** \author Craig Hutchinson \file 
	\brief 
*/
#ifndef LEXAI_SENTENCE_H
#define LEXAI_SENTENCE_H

#include "FrxaiDictionary.h"
#include "SmallMap.h"

namespace frxai
{
	class Sentence : protected Word::Array
	{
	public:

		typedef Word::Array::const_iterator const_iterator;
		typedef Word::Array::iterator iterator;
		typedef Word::Array::const_reference const_reference;
		typedef Word::Array::reference reference;

		typedef frx::SmallMap< Word, Word > ReplaceMap;

	public:

		Sentence() : m_dictionary( 0 ) {}

		/** \brief Constructor
			\param[in] dictionary A reference to the dictionary used to lookup words
			\param[in] sentece The sentence in string form
		*/
		Sentence( Dictionary& dictionary, const String& sentence, Char delim = ' ' );		

		/** \brief Constructor
			\param[in] dictionary A reference to the dictionary used to lookup words
		*/
		Sentence( Dictionary& dictionary );

		/** \brief Copy Constructor
			\param[in] sentence Sentence to copy
		*/
		Sentence( const Sentence& sentence );

		/** \brief destructor
		*/
		~Sentence() { clear(); }

		/** \brief Asign a new string sentence to this sentence
			\remarks This can only be used if the string was constructed with a dictionary
		*/
		Sentence& operator =( const String& sentence )
		{ assert( m_dictionary ); parse( sentence ); return *this; }

		void parse( const String& sentence, Char delim = ' ' );

		//Iterators
		iterator begin() { return Word::Array::begin(); }
		const_iterator begin() const { return Word::Array::begin(); }
		iterator end() { return Word::Array::end(); }
		const_iterator end() const { return Word::Array::end(); }
		reference front() { return Word::Array::front(); }
		const_reference front() const { return Word::Array::front(); }
		reference back() { return Word::Array::back(); }
		const_reference back() const { return Word::Array::back(); }
		
		void push_back( const Word& i ) 
		{ Word::Array::push_back( i ); }

		/** \brief Insert a word at the back of the sentence from a string
			\remarks Requires that a dictionary was set in the constructor
		*/
		void push_back( const String& s ) 
		{ assert( m_dictionary ); push_back( m_dictionary->lookup( s ) ); }
		
		void push_back( const Sentence& s ) 
		{ push_back( s.begin(), s.end() ); }

		template< class TIt >
		void push_back( TIt ibegin, TIt iend )
		{ while ( ibegin != iend ) { push_back( *ibegin ); ++ibegin; } }

		/** \brief Array access operator (const)
		*/
		const Word& operator [] ( Index i ) const
		{ return at( i ); }

		/** \brief Array access operator
		*/
		Word& operator [] ( Index i )
		{ return at( i ); }

		//Container functions
		void clear() { return Word::Array::clear(); }
		size_type size() const { return Word::Array::size(); }
		void reserve( size_type count ) { return Word::Array::reserve( count ); }

		/** \brief Replaces the words in the sentence based upon the supplied map of words to
			their replacements
			\remarks This is usefull for replacing wildcard values
		*/
		void replace( const ReplaceMap& matches );

		friend std::ostream& operator << ( std::ostream& os, const Sentence& node );
	private:


	private:
		Dictionary* m_dictionary; ////< The dictionary which is used to lookup words
	};


} //END: frxai

#endif