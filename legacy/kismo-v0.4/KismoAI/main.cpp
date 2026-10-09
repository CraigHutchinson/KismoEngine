/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoAI\main.cpp
 * Status: current (newest copy of this file name in the archive)
 */
/** \author Craig Hutchinson \file 
	\brief 
*/

/** \mainpage Lex-AI - NLP, GPS, and A* search

	\author Craig Hutchinson
	\section intro_sec Introduction

	The idea of this project was to create an agent that could be commanded to perform a task
	using a natural human language. For this there were two main steps which were described, being
	the NLP (Natural Language Processing) for decoding the users text command and the GPS operator
	search
	
	Due to time the current systems implement a Breadth-First attempt at solving the problems but 
	utilise complex programming methods such as a Bit-wise methods for the matcher for speed. All data
	 is also loaded from a LISP like file format which may be edited as required.
	
	For creating the intelligent agent it also requires to be able to navigate the world in the aim
	to accoumplish the desired tasks, be it to find an object or the likes. For this the A search
	algorithm was created and uses a highly extensible system for fuiture expansion and flexibility.
	
	At present the seperate systems do not interconnect as more time is needed in getting from
	the lexical parsing to the operator goal states for the GPS search. Also the lexical parser requires
	some re-thinking and a later version will hopefully resolve flaws in the design.;

 \section install_sec Installation

 \subsection step1 Step 1: Opening the box
  
 etc...
*/

#include "Profiler.h"
#include "Log.h"
#include "DirectoryManager.h"
#include "Win32ConsoleListener.h"
#include <iostream>
#include <string>

#include "LexaiLmlLoader.h"
#include "LexaiBottomUpParser.h"

#include "OpsaiMatcher.h"
#include "OpsaiWorldState.h"
#include "OpsaiLmlLoader.h"
#include "OpsaiBreadthSearch.h"

#include "pathaiAStarSearch.h"
#include "pathaiGridSpace.h"


INIT_PROFILER;

//Forward declare the testing functions
bool runNLP( frxai::Dictionary& dictionary ); ////< Natural Language Processing tests (NLP)
bool runGPS( frxai::Dictionary& dictionary ); ////< Operator search testing
bool loadGoals( frxai::Dictionary& dictionary, opsai::State::Array& goals, 
			   const String& filename, const String& intermediateStep ); //Helper for loading goals foor the Operator search
bool runAStar(); ////< A* pathfinding search testing

int main(int argc, char **argv)
{		
	/// Leak checking on PC Platform in debug mode only - needs to be set at app entry
#ifdef WIN32
	DEBUG_ONLY( _CrtSetDbgFlag( _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_DELAY_FREE_MEM_DF ); )
#endif

	//Create the log
	frx::Log globalLog( "logging_" BUILD_MODE_FULL ".log" );
	g_pLog = &globalLog; //Set this log as the global log
	if ( !globalLog.isOpen() )
	{
		logError( "Failed to open log for write access! Messages may not be written to file!" );
	}

	//Create a log listener to output the log messages to the windows console aswell as to file
	// This also resizes the console so it can store more information
	win32::Win32ConsoleListener consoleLogger;
	consoleLogger.listenToLog( g_pLog );

	//Set up the driectory structure for where different file types can be found
	io::DirectoryManager directoryManager;
	directoryManager.addDirectoryMap( lexai::Lexicon::TYPE, "data/languages/" );
	directoryManager.addDirectoryMap( lexai::Grammar::TYPE, "data/languages/" );
	directoryManager.addDirectoryMap( opsai::OperatorStore::TYPE, "data/operators/" );
	directoryManager.addDirectoryMap( opsai::WorldState::TYPE, "data/world/" );

	//lexai::XmlLoader languageLoader( "language1" );
	frxai::Dictionary dictionary;

	Char userBuffer[512];////< Buffer for storing users input
	Bool retry = true; //Retry flag for existing to the main menu

	logSuccess( "By Craig Hutchinson (D4076953)\t\t Version 0.1\n\n"
		"Games Artificial Intelligence (GAI) 2008\n\n"
		"Introduction:\n"
		"--------------\n"
		"The idea of this project was to create an agent that could be commanded to perform a task\n"
		"using a natural human language. For this there were two main steps which were described, being\n"
		"the NLP (Natural Language Processing) for decoding the users text command and the GPS operator\n"
		"search\n"
		"\n"
		"Due to time the current systems implement a Breadth-First attempt at solving the problems but \n"
		"utilise complex programming methods such as a Bit-wise methods for the matcher for speed. All data\n"
		" is also loaded from a LISP like file format which may be edited as required.\n"
		"\n"
		"For creating the intelligent agent it also requires to be able to navigate the world in the aim\n"
		"to accoumplish the desired tasks, be it to find an object or the likes. For this the A* search\n"
		"algorithm was created and uses a highly extensible system for fuiture expansion and flexibility."
		"\n\n"
		"At present the seperate systems do not interconnect as more time is needed in getting from\n"
		"the lexical parsing to the operator goal states for the GPS search. Also the lexical parser requires\n"
		"some re-thinking and a later version will hopefully resolve flaws in the design.\n" );

#if _DEBUG
	logSuccess( "\n\n"
		"-------------------------------------------------------------------------------------\n"
		"\tYou are running in DEBUG, GPS search may take over 1 minute.\n"
		"\t\t\tRelease mode may be 90 times faster. \n"
		"-------------------------------------------------------------------------------------\n" );
#endif

	//-----------------------------------
	// Start of user loop 
	//-----------------------------------
	do
	{
		//Ask what the user wishes to do
		logSuccess( std::endl << 
			"-------------------------------------------------------------------------------------\n"
			"What AI test segment do you wish to display?\n\n"
			"\to/O = Operator Search\n"
			"\tl/L = Natural Language parsing (NLP)\n"
			"\ta/A = A* search algorithm\n"
			"\n\tNOTE:Type anything else to quit at any time or to return to this menu\n" );

		std::cout << "\noption>";
		std::cin.getline( userBuffer, 512, '\n' );
		std::cin.clear();
		logSuccess( "" );

		retry =  true;
		//Check the users input
		switch ( userBuffer[0] )
		{
		case 'o': case 'O': //Enter the operator search test function
			{
				runGPS( dictionary );
				break;
			}
		case 'l': case 'L': //Enter the NLP test function
			{
				runNLP( dictionary );
				break;
			}
		case 'a': case 'A': //Enter the A* test function
			{
				runAStar();
				break;
			}
		default: //Retry the search
			{
				retry = userBuffer[0] == 'y' ||  userBuffer[0] == 'Y';
				break;
			}
		}
	}
	while( retry ); //Loop until the user quits

	system( "pause" );

	//End the profiler when we are using a METRICS build
	END_PROFILER;

	return 1;
}

/** \brief Tests the GPS operator implementation and Matcher using Element1 LISP operators and 
	world state
*/
bool runGPS( frxai::Dictionary& dictionary )
{
	//Create the matcher instance
	// This could be singleton but for multithreading its advantageous using an instance by reference
	opsai::Matcher matcher;

	//Create the oeprator loader for parsing the operators from file
	opsai::LmlOperatorStoreLoader operatorLoader( dictionary, "operators1" );

	//Create tjhe oeprator store which will contain and manage all the operators
	opsai::OperatorStore operators( &operatorLoader );

	//Load the operators from file
	if ( !operators.load() )
	{
		logError( "Failed to load operators" );
		return false;
	}

	//Create a loader to load the world state
	opsai::LmlWorldStateLoader worldStateLoader( dictionary, "worldState1" );

	//Create our initial world state
	opsai::WorldState worldState( &worldStateLoader );

	//Store for a custom goal set by the user
	opsai::State customGoal( dictionary );
	Bool useCustomGoal = false;

	Char userBuffer[512];////< Buffer for storing users input
	Bool retry = true; //Retry flag for existing to the main menu
	String intermediateGoal = "microwave"; ////< By default we sue the microwave as the inetrmediate goal
	
	//-----------------------------------
	// Start of user loop 
	//-----------------------------------
	do
	{
		//Create the array storeage for all the goals
		opsai::State::Array allGoals;

		//If we aren't using a custom goal we re-load the world state and load the goals from file
		if ( !useCustomGoal )
		{
			//Attempt to load the world state from file
			if ( !worldState.load() )
			{
				logError( "Failed to load the worldState" );
				return false;
			}		

			//Now load the goals and
			if ( !loadGoals( dictionary, allGoals, "goals1", intermediateGoal) ) ////< Use the microwave intermediate step by default
			{
				logWarning( "Failed to load the goals!" );
				return false;
			}
		}
		else
		{
			//Use the custom goal set by the user
			allGoals.push_back( customGoal );

			//Set that we won't re-use this next time
			useCustomGoal = false;
		}

		//Create tyhe search method instance
		//TODO: proper GPS implementation as the breadth search is very slow
		opsai::BreadthSearch opsBreadthSearch;

		//Get the start time for profiling
		UInt32 totalStartTime = _SystemTimer.runningTime_ticks();
		
		logSuccess( 
			"\n-------------------------------------------------------------------------------------\n"
			"Started operator search....\n" );

		Bool goalFound = false;
		//Iterate through all the goals fullfilling each one at a time for speed so the search space doesn't expand
		//TODO: proper GPS Means-End Analysis implementation
		for ( opsai::State::Array ::const_iterator iGoals = allGoals.begin();
			iGoals != allGoals.end();
			++iGoals )
		{
			//Get the start time for profiling
			UInt32 goalStartTime = _SystemTimer.runningTime_ticks();

			//Create the goal store
			opsai::State::Array goals;

			//Add the next goal to the list to obtain
			goals.push_back( *iGoals );

			logSuccess( "'" <<  *iGoals << "'..." );

			//------------------------------------------------------------------------------
			//Perform a breadth search to meet the desirted goals
			//------------------------------------------------------------------------------
			goalFound = opsBreadthSearch.search( worldState, goals, operators );

			//Get the end time for timing puurposes
			UInt32 goalEndTime = _SystemTimer.runningTime_ticks();	

			//Output the timings for obtainign the results of the first goal
			logSuccess( "\tTime = " << (Float32)(goalEndTime-goalStartTime) / _SystemTimer.ticksPerSecond() 
				<< "\t Success = " << goalFound << '\n' );

			if ( !goalFound ) //Don't continue if a goal fails
			{
				break;
			}
			else
			{
				//Use the resultijng state as input to the next iteration
				worldState = opsBreadthSearch.getResultState();
			}
		}

		UInt32 totalEndTime = _SystemTimer.runningTime_ticks();

		logSuccess( "TOTAL: breadth-search time = " << (Float32)(totalEndTime-totalStartTime) / _SystemTimer.ticksPerSecond() 
			<< "\t result = " << goalFound );

		//If we managed to actually reach the target goals we output the results
		if ( goalFound )
		{
			logSuccess( "\n\nThe results of the search are...\n" );
			for ( opsai::Search::OperatorResults::const_iterator iResult = opsBreadthSearch.getOperators().begin();
				 iResult != opsBreadthSearch.getOperators().end();
				 ++iResult )
			{
				logSuccess( iResult->op->getName() << 
					"\t:\t" << iResult->op->getText( iResult->match ) );				
			}
			logSuccess( "(END)\n" );
		}
		else
		{
			logSuccess( "Goal was not reached. No results" );
		}

		
GPS_PROMPT:
		//Ask if another phrase should be parsed
		logSuccess( std::endl << 
			"-------------------------------------------------------------------------------------\n"
			"Do you wish to perform another operator search?\n"
			"\ty/Y = Perform Search again.(NOTE: World state is always re-loaded before search)\n"
			"\tr/R = Reload Operators & WorldState from file to load modifications \n"
			"\to/O = Open the Operator file in notepad.exe for editing\n"
			"\tw/W = Open the WorldState file in notepad.exe for editing\n"
			"\ti/I = Change the intermediate goal to use the microwave or oven-hob\n"
			"\tc/C = Define a custom goal and perform a search without re-loading the WorldState from file\n"
			"\n" );

		std::cout << "\noption>";
		std::cin.getline( userBuffer, 512, '\n' );
		std::cin.clear();
		logSuccess( '\n' );

		//Check the users input
		switch ( userBuffer[0] )
		{
		case 'o': case 'O': //Edit the operators
			{
				logSuccess( 
					"-------------------------------------------------------------------------------------\n"
					"Launching Operator file....." );

				//Get the location of the grammar file
				String operatorFilename = "notepad \"" + _DirectoryManager.getFullPath( 
					operatorLoader.getFilename() + ".lml", opsai::OperatorStore::TYPE ) + "\"";

				//Call the system to open the gramma/lexivcon file with notepad
				system( operatorFilename.c_str() );
				logSuccess( "\tDone\n" );
			}
		case 'r': case 'R': //Reload operators
			{
				logSuccess( 
					"-------------------------------------------------------------------------------------\n"
					"Reloading Operators & WorldState.....\n" );
				retry = operators.load();
				retry = retry && worldState.load();
				goto GPS_PROMPT; ////< Use a goto for convenienece.....!
			}
		case 'w': case 'W': //Edit world State
			{
				logSuccess(
					"-------------------------------------------------------------------------------------\n"
					"Launching WorldState file....." );

				//Get the location of the grammar file
				String worldStateFilename = "notepad \"" + _DirectoryManager.getFullPath( 
					worldStateLoader.getFilename() + ".lml", opsai::WorldState::TYPE ) + "\"";

				//Call the system to open the WorldState file with notepad
				system( worldStateFilename.c_str() );
				logSuccess( "\tDone\n" );

				goto GPS_PROMPT; ////< Use a goto for convenienece.....!
			}
		case 'i': case 'I': //Change intermediate state
			{
				logSuccess( 
					"-------------------------------------------------------------------------------------\n"
					"\nEnter the name of the intermediate state to use. This should match a child name wihtin the"
					" goals file\n"
					"\n\tEnter either 'microwave' or 'oven-hob'\n" );
				logSuccess( "intermediateState>" );
				std::cin.getline( userBuffer, 512, '\n' );
				std::cin.clear();
				
				intermediateGoal = userBuffer;
				logSuccess( "\nIntermedioate Goal states set to '" << intermediateGoal << "'" );

				goto GPS_PROMPT; ////< Use a goto for convenienece.....!
			}
		case 'c': case 'C': //Change intermediate state
			{
				logSuccess(
					"-------------------------------------------------------------------------------------\n"
					"\nEnter the goal tupple of 4 words or less.\n"
					"\n\te.g. at agent bin' 'on plate oven-hob'\n\n" );
				logSuccess( "goal entry>" );
				std::cin.getline( userBuffer, 512, '\n' );
				std::cin.clear();
				
				customGoal.set( frxai::Sentence( dictionary, userBuffer) );
				useCustomGoal = true;
				logSuccess( "\nCustom goal set to '" << customGoal << "'."
					"The world state will not be re-loaded before search." );

				retry = true;
				break;
			}
		default: //Retry the search
			{
				retry = userBuffer[0] == 'y' ||  userBuffer[0] == 'Y';
				break;
			}
		}
	}
	while( retry ); //Loop until the user quits

	return true;
}

/** \brief Helper function for loading the goals for a GPS search from file
*/
Bool loadGoals( frxai::Dictionary& dictionary, opsai::State::Array& goals, const String& filename, const String& intermediateStep )
{
	goals.clear();

	//Create a loader for the search goals
	opsai::LmlWorldStateLoader goalsLoader( dictionary, filename );

	//Load the goals from file
	Bool ret = goalsLoader.load( goals, "first" ); ////< Load the first set of goals for toasting etc
	ret = ret && goalsLoader.load( goals, intermediateStep ); ////< Load the goals to direct topwards using the microwave by default
	ret = ret && goalsLoader.load( goals, "last" ); ////< Load the last set of goals for cleaning up etc

	if ( !ret )
	{
		logWarning( "One or more goal sets failed to load!" );
	}
	else
	{
		logSuccess( "Loaded goal states with intemediate goals for '" << intermediateStep << "'" );
	}

	return ret;
}

/** \brief Natural language processing tests
	\param[in] dictionary The dictionary that will be used
*/
bool runNLP( frxai::Dictionary& dictionary )
{
	//Create a loader for the lexicon and grammar files
	// This keeps file operations seperate maintining an extedible interface for the system
	lexai::LmlLoader languageLoader( "language1" );

	//Create the lexicon and grammar instances we will use for language parsing
	lexai::Lexicon lexicon( dictionary, &languageLoader );	
	lexai::Grammar grammar( dictionary, &languageLoader );

	//Attempt to load the grammar and lexicon
	if ( !lexicon.load() || !grammar.load() )
	{
		logError( "Failed to load grammar and lexicon" );
		return false;
	}

	Char userBuffer[512];////< Buffer for storing users input
	Bool retry = true; //Retry flag for existing to the main menu

	//-----------------------------------
	// Start of user loop 
	//-----------------------------------
	do
	{
		logSuccess( "\n-------------------------------------------------------------------------------------\n"
			"Enter a phrase to parse (without punctuation or capitalisation)\n"
			"\n\tE.g.\t'put the white plate on the big table onto the red shelf'" );
		
		//Read in the users input phrase
		std::cout << "\nin-phrase> ";
		std::cin.getline( userBuffer, 512, '\n' );
		std::cin.clear();
		
		//TODO: Add a preprocess to remove punctuation etc

		//Parse the string into a scentence structure whihc is baisically an array of 'words'
		lexai::Sentence sentence( dictionary, userBuffer );
		logSuccess( "" );
	
		//Creat the parser which uses our lexicon and grammar we loaded earlier
		//For not this is a bsaic bottom up parser
		lexai::BottomUpParser nlpParser( grammar, lexicon );
	
		//------------------------------------------------------------------------------
		//Call the parser to process the sentence
		//------------------------------------------------------------------------------
		nlpParser.parse( sentence );

NLP_PROMPT:
		//Ask if another phrase should be parsed
		logSuccess( "\n-------------------------------------------------------------------------------------\n"
			"Do you wish to parse another phrase?\n"
			"\ty/Y = Parse another phrase.\n"
			"\tr/R = Reload Lexicon & Grammar to load modifications\n"
			"\to/O = Open the Lexicon/Grammar file in notepad.exe for editing\n"
			"\n" );

		std::cout << "\noption>";
		std::cin.getline( userBuffer, 512, '\n' );
		std::cin.clear();
		logSuccess( "\n Phrase to parse is '" << userBuffer << "'\n" );

		//Check the users input
		switch ( userBuffer[0] )
		{
		case 'o': case 'O': //Edit grammar and lexicon
			{
				logSuccess( "Launching Lexicon/Grammar file....." );

				//Get the location of the grammar file
				String grammarFilename = "notepad \"" + _DirectoryManager.getFullPath( 
					languageLoader.getFilename() + ".lml", lexai::Grammar::TYPE ) + "\"";

				//Call the system to open the gramma/lexivcon file with notepad
				system( grammarFilename.c_str() );
				logSuccess( "\nDone\n" );

				//goto NLP_PROMPT;
			}
		case 'r': case 'R': //Reload grammar and lexicon
			{
				logSuccess( "Reloading grammar.....\n" );
				retry = lexicon.load() && grammar.load();
				goto NLP_PROMPT; ////< Use a goto for convenienece.....!
			}
		default: //Retry
			{
				retry = userBuffer[0] == 'y' ||  userBuffer[0] == 'Y';
				break;
			}
		};
	}
	while( retry ); //Loop until the user quits

	return true;
}


/** \brief A* search algorithm test
*/
bool runAStar()
{
	Index editMapLine = (Index)__LINE__;
	/// This array stores the tile costs for each tile as we don't have file loading 
	// for this part due to time
	// 127 = wall, other values including negatives are costs
	//----------------EDIT MAP HERE -----------------
	//-----------------------------------------------
	const Int8 cellCosts[ 9 * 9 ] = {
		0,	0,		2,	127,	5,	2,		0,		0,		0,
		0,	127,	3,	127,	15,	127,	127,	127,	0,
		0,	127,	2,	127,	1,	1,		0,		127,	0,
		0,	127,	1,	127,	1,	2,		2,		127,	0,
		0,	127,	0,	5,		0,	0,		1,		127,	0,
		0,	127,	0,	0,		1,	2,		3,		127,	0,
		1,	10,		0,	0,		5,	127,	127,	127,	10,
		1,	8,		0,	20,		2,	16,		0,		10,		0,
		1,	4,		0,	30,		2,	10,		0,		20,		0
	};
	//-----------------------------------------------
	//--------------- EDIT MAP HERE -----------------

	logSuccess( "-------------------------------------------------------------------------------------\n"
		"This is a test of a A* implementation which performs a cost and distance heuristic for\n"
		"picking the best route through the map.\n\n"
		"\t NOTE: Due to time visual output is displayed with text.\n\n"
		"The test supplied here is a 9x9 grid size with a start at the middle left and the end\n" 
		" at the middle right denoted by the '<' characters.\n"
		"In the middle are some obstacles forming some walls and a U shape barrier  with walls\n"
		"having a value of 127 and any other value being a 'cost' to pass the tile.\n\n"
		"EDITING: Due to time, the map tile costs may be edited in the source in 'main.cpp' on line " << editMapLine << "\n"
		"\n-------------------------------------------------------------------------------------\n" );

	const frx::Size2i size( 9, 9 );
	//Create the search space. For this we create a grid search space.
	// This could be anything such as a graph of locations etc. Due to the seperation fo the search space
	// the algorithm can be changed and applied to any variety of possible search scenarios
	// TODO: This 'could' for the gps search in future with a different SearchSpace and Algorithm using the same interfaces
	pathai::GridSpace gridSpace( size );
			
	//Set the costs of the map tiles
	//TODO: Load from file, as there isn't time we just use the above array
	//NOTE: cost of 127 is a wall.
	logSuccess( "The inital search space....\n" );
	//-----------------------------------------------
	// output the tile costs of the map
	//-----------------------------------------------
	StringStream tgridbuff;
	for ( UInt16 y = 0; y < size.h; ++y )
	{
		tgridbuff << "\n\t";
		for ( UInt16 x = 0; x < size.w; ++x )
		{
			tgridbuff << (Int16)cellCosts[ x + (y *size.w )] << '\t';
			gridSpace.getGridState( x,y )->setCost( cellCosts[ x + (y *size.w )] );
		}
	}
	logSuccess( tgridbuff.str() );
	
	//Create the A* search instance and suply the search space that will be used
	pathai::AStarSearch pathSearch( gridSpace );

	logSuccess( "\n\nRuning A* search...." );

	//-----------------------------------------------
	// Perform the A* search for the best path through the map
	//-----------------------------------------------
	//-----------------------------------------------
	
	Bool ret = pathSearch.search( gridSpace.getGridState( 0,size.h/2 ),
		gridSpace.getGridState( size.w-1,size.h/2 ) );
	
	//-----------------------------------------------
	//-----------------------------------------------
	
	logSuccess( "Done" );

	if ( ret )
	{
		logSuccess( "\nSearch results..... ( * = Route )\n" );

		//-----------------------------------------------
		// Output the results of the search grid using a '*' to denote the route
		//-----------------------------------------------
		StringStream tbuff;
		logSuccess( "" );
		for ( UInt16 y = 0; y < size.h; ++y )
		{
			tbuff << "\n\t";
			for ( UInt16 x = 0; x < size.w; ++x )
			{
				tbuff << (Int16)cellCosts[ x + (y *size.w )];

				for( pathai::State::Array::const_iterator iState = pathSearch.getResultStates().begin(); iState != pathSearch.getResultStates().end(); ++iState )
				{
					pathai::GridSpace::State_T* state = (pathai::GridSpace::State_T*)*iState;

					if( state->getPosition().x == x &&state->getPosition().y == y )
					{
						tbuff << '*';
						break; 
					}

				}
				if ( ( x==0 || x==size.w-1) && y ==size.h/2 ) 
					tbuff << '<';

				tbuff << '\t';
			}
		}

		tbuff << "\n\n";
		
		//-----------------------------------------------
		// Output the coordinates of the found route
		//-----------------------------------------------
		Count count = 0;
		//Ouput the state positions for the resulting route
		for( pathai::State::Array::const_iterator iState = pathSearch.getResultStates().begin(); iState != pathSearch.getResultStates().end(); ++iState )
		{
			//Get the search space State from teh GridSpace we used to search on
			pathai::GridSpace::State_T* state = (pathai::GridSpace::State_T*)*iState;

			//Output the coordinates of the foudn routes state
			tbuff << '(' << state->getPosition().x << ',' << state->getPosition().y << ") ";

			//Newline every 10
			if ( count == 10 ) 
			{ 
				count = 0; 
				tbuff << '\n';
			}
			++count;
		}

		tbuff << "\n\n";
		logSuccess( tbuff.str() );
	}
	else
	{
		logSuccess( "The goal could not be reached or an error occured!" );
	}

	return true;
}