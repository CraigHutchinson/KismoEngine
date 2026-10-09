/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoAI\LexaiXmlLoader.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
#include "LexaiXmlLoader.h"
#include "XmlFileParser.h"
#include "DirectoryManager.h"

const String grammarFileExt = ".xml";

const String languageTag = "language";
const String commentTag = "comment";

const String grammarTag = "grammar";
const String ruleTag = "rule";
const String syntacticTag = "syntactic";
const String semanticTag = "semantics";
const String idTag = "id";
const String nameTag = "name";

const String lexiconFileExt = ".xml";

const String lexiconTag = "lexicon";
const String wordTag = "word";
const String categoryTag = "category";

namespace lexai
{
	XmlLoader::XmlLoader( const String& filename ) : 
		GrammarLoader( filename ), LexiconLoader( filename )
	{}

	Bool XmlLoader::load( Grammar& grammar )
	{
		PROFILE;

		String grammarFilename = _DirectoryManager.getFullPath( GrammarLoader::getFilename() + grammarFileExt, Grammar::TYPE );

		io::XmlFileParser parser( grammarFilename );
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
				grammarElement = child;

		if ( grammarElement.isNull() )
		{
			logError( "A '" << grammarTag << "' element was not found when loading the grammar file " << grammarFilename );
			return false;
		}

		for ( io::FileParser::Node child = grammarElement.firstChild(); child.isValid(); child = child.next() )
		{
			if ( child == ruleTag )
			{
				parseRule( child, grammar );
			}
		}

		return true;

	}

	Bool XmlLoader::load( Lexicon& lexicon )
	{
		PROFILE;

		String lexiconFilename = _DirectoryManager.getFullPath( LexiconLoader::getFilename() + lexiconFileExt, Lexicon::TYPE );

		io::XmlFileParser parser( lexiconFilename );
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
				lexiconElement = child;

		if ( lexiconElement.isNull() )
		{
			logError( "A '" << lexiconTag << "' element was not found when loading the lexicon file " << lexiconFilename );
			return false;
		}

		for ( io::FileParser::Node child = lexiconElement.firstChild(); child.isValid(); child = child.next() )
		{
			if ( child == wordTag )
			{
				parseWord( child, lexicon );				
			}			
		}

		return true;

	}

	Bool XmlLoader::parseWord( io::FileParser::Node node, Lexicon& lexicon )
	{
		PROFILE;

		Word category;

		for ( io::FileParser::Node child = node.firstChild(); child.isValid(); child = child.next() )
		{
			if ( child == categoryTag )
				category = lexicon.getDictionary().lookup( child.getValue() );

		}

		LexiconEntry wordLexicon( category );

		return lexicon.add( node.getValue(), wordLexicon );
	}

	Bool XmlLoader::parseRule( io::FileParser::Node node, Grammar& grammar )
	{
		PROFILE;

		String ruleID = node.firstAttribute( idTag );
		String ruleName = node.firstAttribute( nameTag );

		Grammar::GrammarRule rule;
		rule.name = grammar.getDictionary().lookup( ruleName );
		
		for ( io::FileParser::Node child = node.firstChild(); child.isValid(); child = child.next() )
		{
			if ( child == syntacticTag )
			{
				for ( io::FileParser::Node comp = child.firstChild(); comp.isValid(); comp = comp.next() )
				{
					rule.components.push_back( grammar.getDictionary().lookup( comp.getName() ) );
				}
			}
			else
			if ( child == semanticTag )
			{

			}
		}

		return grammar.add( ruleID, rule );
	}
	

} //END: lexai