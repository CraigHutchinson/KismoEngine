/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\KismoCore\KismoFramework\MacroNamespace.h
 * Status: duplicate of kismo-core/_SOURCE_/KismoFramework/MacroNamespace.h
 */
/*^**************************************************************************
*	
*	Kismo Engine 
*	© 2008-2013 Craig Hutchinson (craig@craighutchinson.co.uk)
*	All right reserved
*
*	This file is subject to the terms and conditions defined in
*	file 'license.txt', which is part of this source code package.
*
***************************************************************************^*/

#ifndef KISMOFRAMEWORK_MACRONAMESPACE_H
#define KISMOFRAMEWORK_MACRONAMESPACE_H

#include "KismoFramework\Common.h"
#include "KismoFramework/Types.h"
#include "KismoFramework/String.h"
#include <map>

namespace kismo
{	
	/** Structure used to assist creation of multiple macros as a batch process to alow passing 
		an array in a single functionc all
	*/
	struct MacroDefinition
	{
		const Char* name;
		const Char* value;
	};

	/**	MacroNamespace containing any number of macro values indexed by id
		\brief	A MacroNamespace contains key and value pairs for obtaining values of string Ids. This namespace is searched for a 'macroId' and then subsequent parent namespaces
				until the key is found.
	*/
	class MacroNamespace
	{
	public:

		//typedef Char* MacroID;
		//typedef Char* MacroValue;

		static const Char kMacroIdentifier = '$'; //, Macros are formatted as '$(_MACRONAME_)' int he same way as visual studio

		typedef std::map<String,String> MacroLookup;
	public:

		MacroNamespace( MacroNamespace* parentNamespace = 0 );

		/**	Sets the value of a macro with values of known length
			\brief		A macro consists of a 'macroID' name identifier which maps to a 'value'. e.g. 'AppPath' could map to 'C:\Program Files\...\...\App.exe'
			\remarks	If 'macroId' already exists the existing value will be replaced. If this is not intended call 'findMacro' first.
			\returns Whether the new macroID was unique
		*/
		inline bool setMacro( const Char* macroID, Count macroIDLength, const Char* value, Count valueLength )
		{ return setMacro( String(macroID,macroIDLength), String(value,valueLength) ); }

		/**	Sets the value of a macro
			\brief		A macro consists of a 'macroID' name identifier which maps to a 'value'. e.g. 'AppPath' could map to 'C:\Program Files\...\...\App.exe'
			\remarks	If 'macroId' already exists the existing value will be replaced. If this is not intended call 'findMacro' first.
			\returns Whether the new macroID was unique
		*/
		inline bool setMacro( const Char* macroID, const Char* value )
		{ return setMacro( String(macroID), String(value) ); }
		
		bool setMacro( const String& macroID, const String& value );

		/** Sets a list of macros calling setMacro( const Char*, const Char*) for each
			\returns The number of macros that were unique (i.e. not already defined in this namespace)
		*/
		Count setMacros( const MacroDefinition* macros, Count macroCount );

		/** Searches for a macros value
			\returns Pointer to the string value or 0 is the macroID does not exist
			\param[in] macroID Pointer to macro name
			\param[in] macroIDLength Length of macroName if known or 0 for length to be calculated
		*/
		inline const String* findMacro( const Char* macroID, Count macroIDLength = 0 )
		{ return findMacro( String(macroID, macroIDLength) ); }
		
		const String* findMacro( const String& macroID );

		/**	Returns the parent namespace or 'null' if this is a root namespace
		*/
		MacroNamespace* parentNamespace()
		{ return m_parentNamespace; }

		/** Expands any macros in the inString and returns the resulting string onto the outString buffer
			\returns The number of characters required in the output buffer. If insufficient buffer si supplied the output is terminated and results are undefined
		*/
		inline Count expandMacros( const String& inString, Char* outString, Count outStringLength )
		{ return expandMacros( inString.c_str(), (Count)inString.length(), outString, outStringLength ); }

		/** Expands any macros in the inString and returns the resulting string onto the outString buffer
			\returns The number of characters required in the output buffer. If insufficient buffer si supplied the output is terminated and results are undefined
		*/
		inline Count expandMacros( const Char* inString, Char* outString, Count outStringLength )
		{ return expandMacros( inString, measure(inString), outString, outStringLength ); }

		/** Expands any macros in the inString and returns the resulting string onto the outString buffer
			\returns The number of characters required in the output buffer. If insufficient buffer si supplied the output is terminated and results are undefined
		*/
		Count expandMacros( const Char* inString, Count inStringLength, Char* outString, Count outStringLength  );

		/** Expands any macros in the inString and returns the resulting string into the outString buffer resizing 
			the buffer as necessary
			\remarks The resizxing of outString may be a performance consideration
		*/
		//Count expandMacros( const String& inString, String& outString );
		
		/** Expands any macros in the inoutString 
			\remarks The resizing of inoutString may be a performance consideration
		*/
		//Count expandMacros( const String& inoutString );
	private:
		
		struct MacroPosition
		{ 
			Index offset; //< Offset from start of string/end of last macro
			Count namelength;
			const String* value;
		};
		Count findMacros( const Char* inString, Count inStringLength, MacroPosition* outMacros, Count maxOutMacros );
	
	private:
		MacroNamespace* m_parentNamespace;		
#pragma warning(disable:4251)
		MacroLookup m_macros;
#pragma warning(default:4251)
	};
	
} //END: kismo

#endif