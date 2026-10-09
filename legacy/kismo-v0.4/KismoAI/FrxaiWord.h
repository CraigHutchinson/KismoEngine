/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoAI\FrxaiWord.h
 * Status: unique (no other copy of this file name in the archive)
 */
/** \author Craig Hutchinson \file 
	\brief 
*/
#ifndef LEXAI_WORD_H
#define LEXAI_WORD_H

#include "Types.h"
#include "Common.h"

#include "Unique.h"
#include "Named.h"

/** \brief The frxai namespace contains basic framework classes such as Word, Sentence, and Dictionary
*/
namespace frxai
{
	/** \brief Word class relates a string literal to a integer value for fast compares etc
		\remarks This class is the impl which is contained by the Word class to prevent duplication
		\remarks should not be created dorectly
	*/
	class WordImpl : private frx::Unique< WordImpl >, private frx::Named
	{
		friend class Word; ///Word needs to access data
		friend class Dictionary; ///Dictionary needs to create

	protected: //protected to prevent anybody creating the class

		typedef frx::Unique< WordImpl >::UID WID; ///World ID type definition

		WordImpl( const String& name ) : frx::Named( name ) {}


		//Access operators
		const String& getWord() const { return frx::Named::getName(); }
		const WID getWID() const { return frx::Unique< WordImpl >::getUID(); }

		operator const String&() const { return getName(); }
		operator const WID() const { return getUID(); }
	};

	/** \brief Word class wraps WordImpl to prevent duplication of word implementations
	*/
	class Word
	{
	public:			

		typedef WordImpl::WID WID; ///World ID type definition
		typedef Vector< Word > Array; ////< Vector of words

	public:

		Word() :
			m_word( 0 )
#ifdef _DEBUG ///Debugging information
		, _WORD( "_null_" ), _WID( 0 )
#endif
		{}

		explicit Word( const WordImpl* wordImpl ) :
			m_word( wordImpl )
#ifdef _DEBUG ///Debugging information
			, _WORD( (wordImpl ? wordImpl->getName() : "_null_" ) ), _WID( (wordImpl ? wordImpl->getWID() : 0 ) )
#endif
		{ }

		/** \brief Assignment operator
		*/
		Word& operator = (const Word& rhs )
		{
			m_word = rhs.m_word;
			DEBUG_ONLY( _WORD = *m_word; _WID = *m_word; )
			return *this;
		}

		//Cast operators
		operator const String&() const { return *m_word; }
		operator const WID() const { return *m_word; }

		//Equality operators
		Bool operator == ( const Word& rhs ) const { return m_word == rhs.m_word; }
		Bool operator != ( const Word& rhs ) const { return m_word != rhs.m_word; }
		Bool operator < ( const Word& rhs ) const { return m_word < rhs.m_word; }
		Bool operator > ( const Word& rhs ) const { return m_word > rhs.m_word; }
		Bool operator == ( const String& rhs ) const { assert( m_word ); return m_word->getWord() == rhs; }
		Bool operator != ( const String& rhs ) const {  assert( m_word ); return m_word->getWord() != rhs; }

		operator Bool () const
		{ return (m_word != 0); }

		friend std::ostream& operator << ( std::ostream& os, const Word& word )
		{
			if ( word.m_word ) //Can only output the word if it is valid!
				os << (String)word;
			else
				os << '?'; ////< Unknown!
			return os;
		}

	private:
#ifdef _DEBUG
		String _WORD; ////< Debug only: The word string literal
		WID _WID; ////< Debug only: The word ID
#endif
		const WordImpl* m_word;
	};

} //END: frxai

#endif