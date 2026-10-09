/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoAI\LexaiLexicon.h
 * Status: unique (no other copy of this file name in the archive)
 */
/** \author Craig Hutchinson \file 
	\brief 
*/
#ifndef LEXAI_LEXICON_H
#define LEXAI_LEXICON_H

#include "FrxaiDictionary.h"
#include "LexaiSlots.h"
#include "Rtti.h"

namespace lexai
{
	/** \brief Wordmeta stores the lexical and semantic information about a word
	*/
	class LexiconEntry : public Slots
	{
	public:
		
		typedef Vector< LexiconEntry > Array;

	public:

		/** \brief Constructor takes the name of the words category
		*/
		LexiconEntry( const Word& category ) : m_category( category ) {}

		/** \brief Match the slots of the rule with a parse node
		*/
		bool matchSlots( Slots* node ) const;


		const Word& getCategory() const
		{ return m_category; }

	private:
		Word m_category; ////< vector of categories
	};

	//Forward delcarations
	class LexiconLoader;

	/** \brief Lexicon class stores the lexicon for understanding natural language
	*/
	class Lexicon
	{
		DECLARE_RTTI_BASE;
	public:


		const LexiconEntry::Array NullEntry; ////< null meta entry

	public:

		Lexicon( Dictionary& dictionary, LexiconLoader* loader = 0 );

		/** \brief If the loade has been specified calling load() will erase the current lexicon and then
			call the loader to lad the config file	
			\remarks If no loader has been set then False will always be returned
		*/
		Bool load();

		/** \brief Add a word to the lexicon froma  string literal
			\param[in] str The string literal
			\param[in] meta A referene to the data to apply to the word
			\returns If the word exists false will be returned
		*/
		Bool add( const String& str, const LexiconEntry& meta )
		{ return add( m_dictionary.lookup( str ), meta ); }

		/** \brief Add a word to the lexicon from a unique string ID
			\param[in] word unique word identifyer for the word
			\param[in] meta A referene to the data to apply to the word
			\returns If the word exists false will be returned
		*/
		Bool add( const Word& word, const LexiconEntry& meta );

		/** \brief Looks up a word and returns the information known about that word
			\remarks If a word doesn't exist a null LexiconEntry will be returned
			\param[in] str A string literal representation of the word
		*/
		const LexiconEntry::Array& lookup( const String& str )
		{ return lookup( m_dictionary.lookup( str ) ); }

		/** \brief Looks up a word and returns the information known about that word
			\remarks If a word doesn't exist a null LexiconEntry will be returned
			\param[in] word A word id to find
		*/
		const LexiconEntry::Array& lookup( const Word& word );

		/** \brief get a reference ot the dictionary this lexicon uses
		*/
		Dictionary& getDictionary()
		{ return m_dictionary; }

	private: 
		LexiconLoader* m_loader; ////< The loade rwhich parses the lexicon file
		Dictionary& m_dictionary; ////< A reference to the dictionary the lexicon uses for its words

		typedef Map< Word, LexiconEntry::Array > WordMetaMap;
		WordMetaMap m_words; ////< Map of words and their lexicon and semantic data
	};

	/** \brief Base intyerface for a lexicon loader
	*/
	class LexiconLoader
	{
	public:
		LexiconLoader( const String& filename ) : m_filename( filename ) {}

		/** \brief Virtual load function must be implimented
		*/
		virtual Bool load( Lexicon& lexicon ) = 0;
	
		/** \brief get the filename the loade rwill load from
		*/
		const String& getFilename() const
		{ return m_filename ; }

	private:
		String m_filename; ///< The filename the loader will load from
	};

} //END: lexai

#endif