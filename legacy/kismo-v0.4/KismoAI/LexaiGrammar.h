/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoAI\LexaiGrammar.h
 * Status: unique (no other copy of this file name in the archive)
 */
/** \author Craig Hutchinson \file 
	\brief 
*/
#ifndef LEXAI_GRAMMAR_H
#define LEXAI_GRAMMAR_H

#include "LexaiLexicon.h"
#include "LexaiParseNode.h"
#include "Rtti.h"

namespace lexai
{
	//Forward declarations
	class Grammar;

	class GrammarRule : public Slots
	{
	public:
		
		typedef Vector< LexiconEntry > Array;

	public:

		/** \brief Contructor akes the name of the rule
		*/
		GrammarRule( Grammar& grammar, const Word& niceName ) : m_name( niceName ), m_grammar( grammar ) {}
		
		/** \brief Match the components to the rule
			\returns the number fo matches that occured or 0 if a match failed
		*/
		Count match( Dictionary& dictionary, Word::Array::const_iterator begin, Word::Array::const_iterator end ) const;

		/** \brief Match the slots of the rule with a parse node
		*/
		bool matchSlots( ParseNode* node ) const;

		/** \brief Gets te identifying name of the rule
			\remarks The actual Category is stored within the Grammar instance and not in the rule itself
		*/
		const Word& getName() const
		{ return m_name; }

		/** \brief Gets a reference to the components of the rule
		*/
		const Word::Array& getComponents() const
		{ return m_components; }

		/** \brief Add a new component to the back of rthe component array
		*/
		void addComponent( const Word& category )
		{ m_components.push_back( category ); }

	private:

		Word m_name; ////< The nice name for the rule
		Word::Array m_components; ////< The components of the rule
		Grammar& m_grammar; ////< The grammar the rule belongs to
	};

	//Forward delcarations
	class GrammarLoader;

	/** \brief Grammar class stores the grammar for understanding natural language
	*/
	class Grammar
	{
		DECLARE_RTTI_BASE;
	public:


		typedef MultiMap< Word, GrammarRule> RuleMetaMap;
		typedef RuleMetaMap::const_iterator iterator;

	public:

		Grammar( Dictionary& dictionary, GrammarLoader* loader = 0 );

		/** \brief If the loade has been specified calling load() will erase the current grammar and then
			call the loader to load the config file	
			\remarks If no loader has been set then False will always be returned
		*/
		Bool load();

		/** \brief Add a grammar rule
			\param[in] ruleID The string literal ID of the rule
			\param[in] meta The rule data
		*/
		Bool add( const String& ruleID, const GrammarRule& meta )
		{ return add( m_dictionary.lookup( ruleID ), meta ); }

		/** \brief Add a grammar rule
			\param[in] ruleID The word ID of the rule
			\param[in] meta The rule data
		*/
		Bool add( const Word& ruleID, const GrammarRule& meta );

		/** \brief get an iterator for the first rule
		*/
		iterator begin() const
		{ return m_rules.begin(); }

		/** \brief get an iterator for the last rule
		*/
		iterator end() const
		{ return m_rules.end(); }

		/** \brief get a reference ot the dictionary this grammar uses
		*/
		Dictionary& getDictionary()
		{ return m_dictionary; }

	private: 
		GrammarLoader* m_loader; ////< The loade rwhich parses the grammar file
		Dictionary& m_dictionary; ////< A reference to the dictionary the grammar uses for its words

		RuleMetaMap m_rules; ////< Map of rules and their components
	};

	/** \brief Base intyerface for a grammar loader
	*/
	class GrammarLoader
	{
	public:
		GrammarLoader( const String& filename ) : m_filename( filename ) {}

		/** \brief Virtual load function must be implimented
		*/
		virtual Bool load( Grammar& grammar ) = 0;
	
		/** \brief get the filename the loade rwill load from
		*/
		const String& getFilename() const
		{ return m_filename ; }

	private:
		String m_filename; ///< The filename the loader will load from
	};

} //END: lexai

#endif