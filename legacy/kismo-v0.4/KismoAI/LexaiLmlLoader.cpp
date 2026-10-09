/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoAI\LexaiLmlLoader.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
/** \author Craig Hutchinson \file 
	\brief 
*/
#include "LexaiLmlLoader.h"
#include "FrxaiLmlFileParser.h"
#include "DirectoryManager.h"

const String languageTag = "language";

const String grammarFileExt = ".lml";
const String grammarTag = "grammar";

const String lexiconFileExt = ".lml";
const String lexiconTag = "lexicon";
const String slotTag = ".";

namespace lexai
{
	LmlLoader::LmlLoader( const String& filename ) : 
		GrammarLoader( filename ), LexiconLoader( filename )
	{}

	Bool LmlLoader::load( Grammar& grammar )
	{
		PROFILE;

		String grammarFilename = _DirectoryManager.getFullPath( GrammarLoader::getFilename() + grammarFileExt, Grammar::TYPE );

		frxai::LmlFileParser parser( grammarFilename );
		if ( !parser.open() ) 
		{
			logError( " Failed to parse a grammar file " << grammarFilename );
			return false;
		}    
				
		io::FileParser::Node rootElement = parser.getRootNode();		
		if ( rootElement != languageTag )
		{
			logError( "getRootNode is not a '" << languageTag << "' element when loading the grammar file " << grammarFilename );
			return false;
		}

		io::FileParser::Node grammarElement;
		for ( io::FileParser::Node child = rootElement.firstChild(); child.isValid(); child = child.next() )
			if ( child == grammarTag )
			{
				grammarElement = child;
				break;
			}

		if ( grammarElement.isNull() )
		{
			logError( "A '" << grammarTag << "' element was not found when loading the grammar file " << grammarFilename );
			return false;
		}

		for ( io::FileParser::Node child = grammarElement.firstChild(); child.isValid(); child = child.next() )
		{
			parseRule( child, grammar );
		}

		return true;

	}

	Bool LmlLoader::load( Lexicon& lexicon )
	{
		PROFILE;

		String lexiconFilename = _DirectoryManager.getFullPath( LexiconLoader::getFilename() + lexiconFileExt, Lexicon::TYPE );

		frxai::LmlFileParser parser( lexiconFilename );
		if ( !parser.open() ) 
		{
			logError( " Failed to parse a lexicon file " << lexiconFilename );
			return false;
		}    
				
		io::FileParser::Node rootElement = parser.getRootNode();		
		if ( rootElement != languageTag )
		{
			logError( "getRootNode is not a '" << languageTag << "' element when loading the lexicon file " << lexiconFilename );
			return false;
		}

		io::FileParser::Node lexiconElement;
		for ( io::FileParser::Node child = rootElement.firstChild(); child.isValid(); child = child.next() )
			if ( child == lexiconTag )
			{
				lexiconElement = child;
				break;
			}

		if ( lexiconElement.isNull() )
		{
			logError( "A '" << lexiconTag << "' element was not found when loading the lexicon file " << lexiconFilename );
			return false;
		}

		for ( io::FileParser::Node child = lexiconElement.firstChild(); child.isValid(); child = child.next() )
		{
			parseWord( child, lexicon );			
		}

		return true;

	}

	Bool LmlLoader::parseWord( io::FileParser::Node node, Lexicon& lexicon )
	{
		PROFILE;
		
		//Get the word
		String word = node.getName();

		io::FileParser::Node category = node.firstAttribute();
		assert( category.isValid() );
		
		//Get the category of the word
		LexiconEntry wordLexicon( lexicon.getDictionary().lookup( category.getName() ) );

		//Get all the slots for the word
		for ( io::FileParser::Node slot = node.firstChild(); slot.isValid(); slot = slot.next() )
		{
			Sentence values( lexicon.getDictionary() );
			io::FileParser::Node firstAttr = slot.firstAttribute();

			if ( firstAttr == slotTag ) //If the slot is data signified with a '.'
			{
				for ( io::FileParser::Node value = firstAttr.next(); value.isValid(); value = value.next() )
					values.push_back( value.getName() );	

				wordLexicon.addSlot( lexicon.getDictionary().lookup( slot.getName() ), values );
			}
			else
			{
				// The slot is a function or the sorts
			}
			
		}

		//Add the words lexicon information to the lexicon database
		return lexicon.add( word, wordLexicon );
	}

	Bool LmlLoader::parseRule( io::FileParser::Node node, Grammar& grammar )
	{
		PROFILE;

		//Get the ruleName
		String ruleName = node.getName();

		GrammarRule grammarRule( grammar, grammar.getDictionary().lookup( ruleName ) );
		
		io::FileParser::Node child = node.firstChild();
		
		String ruleID = child.getName();

		//get the syntactic components of the rule
		for ( io::FileParser::Node comp = child.firstAttribute().next(); comp.isValid(); comp = comp.next() )
		{
			grammarRule.addComponent( grammar.getDictionary().lookup( comp.getName() ) );
		}

		//Get all the semantic slots for the rule
		for ( io::FileParser::Node slot = child.next(); slot.isValid(); slot = slot.next() )
		{
			io::FileParser::Node firstAttr = slot.firstAttribute();

			if ( firstAttr == slotTag ) //If the slot is data signified with a '.'
			{
				const Word& slotName = grammar.getDictionary().lookup( slot.getName() );
				io::FileParser::Node value = firstAttr.next();

				if ( value.isNull() ) // if ther are no more attributes check for an elemnt
				{
					value = slot.firstChild();
					if ( value.isNull() )
						logError( "An error occured while parsing the grammar rules as no slot name is supplied on the RHS of a rule '" << slotName << "'" );						

					//break the slot value by the .'s
					Sentence lookup( grammar.getDictionary(), value.getName(), '.' );
					grammarRule.addSlot( slotName, lookup );
					
					value = value.firstAttribute();
					while ( value.isValid() )
					{
						//break the slot value by the .'s
						Sentence lookup( grammar.getDictionary(), value.getName(), '.' );
						grammarRule.addSlot( slotName, lookup );
						value = value.next();
					}
				}
				else
				{			
					while ( value.isValid() )
					{
						//break the slot value by the .'s
						Sentence lookup( grammar.getDictionary(), value.getName(), '.' );
						grammarRule.addSlot( slotName, lookup );
						value = value.next();
					}
				}

			}
			else
			{
				Sentence values( grammar.getDictionary() );
				// The slot is a function or the sorts
			}
			
		}

		return grammar.add( ruleID, grammarRule );
	}
	

} //END: lexai