/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoAI\LexaiBottomUpParser.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
/** \author Craig Hutchinson \file 
	\brief 
*/
#include "LexaiBottomUpParser.h"

namespace lexai
{

	BottomUpParser::State::State( const Sentence& sentence )
	{
		PROFILE;
				
		reserve( sentence.size() );

		//Put the words into a list of parse nodes
		for ( Sentence::const_iterator iWord = sentence.begin(); iWord != sentence.end(); ++iWord )
			push_back( new ParseNode( *iWord ) );
	}

	std::ostream& operator << ( std::ostream& os, const BottomUpParser::State& state )
	{
		PROFILE;
				
		for ( BottomUpParser::State::const_iterator iWord = state.begin(); iWord != state.end(); ++iWord )
			os << (*iWord)->getValue() << ' ';
		return os;
	}
		
	void BottomUpParser::State::destroyTree()
	{
		PROFILE;

		ParseNode::NodeRemoveSet removed;
		//Deleting just the leaf nodes will delete the whole graph
		for ( iterator iNode = begin(); iNode != end(); ++iNode )
			(*iNode)->destroyGraph( removed );

		//make sure we don't try and destroy the tree again!
		clear();
	}

	BottomUpParser::BottomUpParser(  Grammar& grammar, Lexicon& lexicon ) : 
		Parser( grammar, lexicon ) 
	{
	}


	Bool BottomUpParser::parse( const Sentence& sentence )
	{
		PROFILE;

		//Create the initial state
		State state( sentence );

		State::Lookup visited;
		State::Array states; ///Array of states yet to be explored
		states.push_back( state ); //initial state
		visited.insert( state ); //Add the inital state to the visited list!

		State front;
		while ( !states.empty() )
		{
			front = states.front();

			if ( front.size() == 1 )
			{
				logSuccess( "Sentence found!" );
				logStateTree( front );
			}
			else
			{
				visited.insert( front ); //Push on the front as is more likely to be checked
				successors( front, states, visited );
			}

			states.pop_front(); //pop off the queue
		}

		//Destroy the parse tree
		state.destroyTree();

		return true;
	}

	void BottomUpParser::logStateTree( State state )
	{
		PROFILE;

		frx::Log::LogMessage logMsg( frx::Log::Success ); 
		logMsg LOG_HEAD << "Found single item.\n" ; 

		logMsg << "Semantics:\n" ; 
		logMsg << **state.begin() << '\n';

		logMsg << "Syntax:\n" ; 

		Bool hasMore = true;
		while( hasMore )
		{
			//Output the current state
			hasMore = false;
			for ( State::iterator iState = state.begin(); iState != state.end(); ++iState )
			{
				logMsg << "( " << (*iState)->getID() << ' ' << (*iState)->getValue() << " )\t";			
			}
 
			//Build the next state to output in the tree
			State nextState;
			for ( State::iterator iState = state.begin(); iState != state.end(); ++iState )
			{
				if ( (*iState)->childCount() )
				{
					nextState.push_back( (*iState)->childBegin(), (*iState)->childEnd() );
					hasMore = true;										
				}
				else
				{
					nextState.push_back( (*iState) );
				}
			}
			state = nextState;
			logMsg << '\n';
		}
		
		logMsg << "\n-----------------";

		*g_pLog += logMsg;
	}
	
	Bool BottomUpParser::successors( const State& state, State::Array& successorStates, State::Lookup& visitedStates )
	{
		PROFILE;

		Word::Array words;
		words.reserve( state.size() );
		for ( State::const_iterator iState = state.begin(); iState != state.end(); ++iState )
			words.push_back( (*iState)->getValue() );
		Word::Array::iterator iWords = words.begin();


		//Move the search position forward until we get some matches
		for( State::const_iterator iState = state.begin(); iState != state.end(); ++iState, ++iWords )
		{
			const LexiconEntry::Array& wordLexicon = getLexicon().lookup( (*iState)->getValue() );
			if ( wordLexicon.size() )
			{

				//Add successor states for all the possible word categories
				for( LexiconEntry::Array::const_iterator iLexiconEntry = wordLexicon.begin(); iLexiconEntry != wordLexicon.end(); ++iLexiconEntry )
				{
					State newState; ////< Create the store for new states we create
					ParseNode* newNode = 0; ////< Pointer to the new node that is created when succesfull
					if ( createSuccessor( newState, visitedStates, state, iState, iState + 1, iLexiconEntry->getCategory(), newNode ) )
					{
						assert( newNode );
						createSemantics( *newNode, *iLexiconEntry );
						successorStates.push_back( newState );
					}
				}
				break; ///Break out
			}
			else //... If the word matches the lexicon we don't bother checking for rules at the moment
			{
				//Match this position in the state with the rules
				for ( Grammar::iterator iRule = getGrammar().begin(); iRule != getGrammar().end(); ++iRule )
				{
					Count matched = iRule->second.match( getGrammar().getDictionary(), iWords, words.end() );
					if ( matched )
					{
						State newState; ////< Create the store for new states we create
						ParseNode* newNode = 0; ////< Pointer to the new node that is created when succesfull
						if ( createSuccessor( newState, visitedStates, state, iState, iState + matched, iRule->first, newNode ) )
						{
							(*newNode).setID( iRule->second.getName() );
							assert( newNode );
							createSemantics( *newNode, iRule->second );
							successorStates.push_back( newState );
						}
					}
				}
			}
		}

		return true;
	}
	
	Bool BottomUpParser::createSemantics( ParseNode& newNode, const LexiconEntry& lexiconEntry )
	{	
		//Add a wildcard slot to the word which takes all the slots of the lexicon entry
		lexiconEntry.matchSlots( &newNode );

		return true;
	}

	Bool BottomUpParser::createSemantics( ParseNode& newNode, const GrammarRule& grammarRule )
	{	
		//Add a wildcard slot to the word which takes all the slots of the lexicon entry
		grammarRule.matchSlots( &newNode );
		return true;
	}

	Bool BottomUpParser::createSuccessor( 
		State& newState, 
		State::Lookup& visitedStates, 
		const State& oldState,
		State::const_iterator beginCollapse, 
		State::const_iterator endCollapse, 
		Word successorName,
		ParseNode*& newNode )
	{
		PROFILE;

		//Make sure the new state is empty
		if ( !newState.empty() )
			newState.clear();

		newState.reserve( oldState.size() - (endCollapse - beginCollapse) + 1 );

		//Inser the part before the collapse
		if ( oldState.begin() != beginCollapse )
			newState.push_back( oldState.begin(), beginCollapse );

		//insert the new node
		IPROFILE( newNode = new ParseNode( successorName ); )		
		newState.push_back( newNode );
				
		//insert the sub seqence as its children
		while ( beginCollapse != endCollapse )
		{
			newNode->addChild( *beginCollapse );
			++beginCollapse;
		}

		//Finish by inserting the rest of the state
		newState.push_back( endCollapse, oldState.end() );

		if ( !isVisited( newState, visitedStates ) )
		{			
			logComment( "State: " << newState );

			visitedStates.insert( newState );
			return true;
		}
		else
		{
			safeDelete( newNode );
			return false;
		}
	}


} //END: lexai