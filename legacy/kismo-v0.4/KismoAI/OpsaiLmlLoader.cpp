/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoAI\OpsaiLmlLoader.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
/** \author Craig Hutchinson \file 
	\brief 
*/
#include "OpsaiLmlLoader.h"
#include "DirectoryManager.h"
#include "FrxaiLmlFileParser.h"


const String operatorStoreFileExt = ".lml";
const String operatorStoreTag = "operators";
const String textTag = "txt";
const String preconditionTag = "pre";
const String deletionTag = "del";
const String additionTag = "add";

const String worldStateFileExt = ".lml";
const String worldStateTag = "world_state";
const String staticTag = "static";
const String dynamicTag = "dynamic";

namespace opsai
{
	//Helper function for loading a node which contains any number of child elements containing states
	bool parseStates( Dictionary& dictionary, io::FileParser::Node node, State::Array& states )
	{
		for ( io::FileParser::Node stateNode = node.firstChild(); stateNode.isValid(); stateNode = stateNode.next() )
		{
			String stateValue( stateNode.getName() );
			stateValue += stateNode.getValue();
		
			states.push_back( State( dictionary, stateValue ) );
		}		

		return true;
	}


	//----------------------------------------------------------------
	// Operator loader
	//----------------------------------------------------------------

	LmlOperatorStoreLoader::LmlOperatorStoreLoader( Dictionary& dictionary, const String& filename ) :  
		OperatorStoreLoader( dictionary, filename ) 
	{}
		
	Bool LmlOperatorStoreLoader::load( OperatorStore& operatorStore )
	{
		PROFILE;

		String operatorStoreFilename = _DirectoryManager.getFullPath( OperatorStoreLoader::getFilename() + operatorStoreFileExt, OperatorStore::TYPE );

		frxai::LmlFileParser parser( operatorStoreFilename );
		if ( !parser.open() ) 
		{
			logError( " Failed to parse an operatorStore file " << operatorStoreFilename );
			return false;
		}    
				
		io::FileParser::Node rootElement = parser.getRootNode();		
		if ( rootElement != operatorStoreTag )
		{
			logError( "getRootNode is not a '" << operatorStoreTag << "' element when loading the operatorStore file " << operatorStoreFilename );
			return false;
		}

		//Storeage for the new operators data
		String name;
		Sentence txt( getDictionary() );
		State::Array pre;
		State::Array del;
		State::Array add;

		//Iterate through all the operators within the file
		for ( io::FileParser::Node opNode = rootElement.firstChild(); opNode.isValid(); opNode = opNode.next() )
		{
			//Get the name of the operator
			name = opNode.getName();

			//Clear the state arrays before we load the new data for the opwerator
			pre.clear(); del.clear(); add.clear();

			//Now get all the sub elements of the operator
			for ( io::FileParser::Node child = opNode.firstChild(); child.isValid(); child = child.next() )
			{

				if ( child == textTag ) //Operator text output message
				{
					txt = child.getValue();
				}
				else
				if ( child == preconditionTag ) ///operator preconditions
				{
					parseStates( getDictionary(), child, pre );
				}
				else
				if ( child == deletionTag ) //Operator deletions
				{
					parseStates( getDictionary(), child, del );
				}
				else
				if ( child == additionTag ) //operator addition
				{
					parseStates( getDictionary(), child, add );
				}
			}


			if ( !operatorStore.createOperator( name, txt, pre, del, add ) )
			{
				logError( "Failed to create operator '" << name << "'" );
			}
		}

		return true;
	}	

	//----------------------------------------------------------------
	// World State loader
	//----------------------------------------------------------------

	LmlWorldStateLoader::LmlWorldStateLoader( Dictionary& dictionary, const String& filename ) :  
		WorldStateLoader( dictionary, filename )
	{}

	Bool LmlWorldStateLoader::load( WorldState& worldState )
	{
		PROFILE;

		String worldStateFilename = _DirectoryManager.getFullPath( WorldStateLoader::getFilename() + worldStateFileExt, WorldState::TYPE );

		frxai::LmlFileParser parser( worldStateFilename );
		if ( !parser.open() ) 
		{
			logError( " Failed to parse an worldState file " << worldStateFilename );
			return false;
		}    
				
		io::FileParser::Node rootElement = parser.getRootNode();		
		if ( rootElement != worldStateTag )
		{
			logError( "getRootNode is not a '" << worldStateTag << "' element when loading the worldState file " << worldStateFilename );
			return false;
		}

		State::Array* staticStates = 0;
		State::Array dynamicStates;
		
		//Iterate through to find the static and dynamic states
		for ( io::FileParser::Node child = rootElement.firstChild(); child.isValid(); child = child.next() )
		{
			if ( child == staticTag )
			{
				assert( staticStates == 0 );
				if ( !staticStates )
					staticStates = new State::Array;
				parseStates( getDictionary(), child, *staticStates );
			}
			else
			if ( child == dynamicTag )
			{
				parseStates( getDictionary(), child, dynamicStates );
			}
		}

		//Now set the world state
		worldState.setStates( staticStates, dynamicStates );

		return true;
	}
	
	Bool LmlWorldStateLoader::load( State::Array& states, const String& nodeName )
	{
		PROFILE;

		String worldStateFilename = _DirectoryManager.getFullPath( WorldStateLoader::getFilename() + worldStateFileExt, WorldState::TYPE );

		frxai::LmlFileParser parser( worldStateFilename );
		if ( !parser.open() ) 
		{
			logError( " Failed to parse an worldState file " << worldStateFilename );
			return false;
		}    
				
		io::FileParser::Node rootElement = parser.getRootNode();

		//If we ghave supplied the name of the node we are laoding from...
		if ( nodeName.length() ) 
		{
			for ( io::FileParser::Node child = rootElement.firstChild(); child.isValid(); child = child.next() )
			{
				if ( child.getName() == nodeName )
				{
					return parseStates( getDictionary(), child, states );
				}
			}

			//We didn't find the node!
			logError( "Failed to find the goal sub node called '" << nodeName << "'" );
		}
		else
		{ //Otherwise we jsut load from the root
			return parseStates( getDictionary(), rootElement, states );
		}

		return false;
	}


} //END: opsai
