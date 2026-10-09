/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\KismoCore\_SOURCE_\KismoFramework\MacroNamespace.cpp
 * Status: unique (no other copy of this file name in the archive)
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


#include "MacroNamespace.h"

namespace kismo
{	
	MacroNamespace::MacroNamespace( MacroNamespace* parentNamespace ) : 
		m_parentNamespace(parentNamespace)
	{}

	bool MacroNamespace::setMacro( const String& macroID, const String& value)
	{
		MacroLookup::_Pairib iInsertion = m_macros.insert( MacroLookup::value_type( macroID, value ) );
		if ( !iInsertion.second )
			iInsertion.first->second = value;
		
		return iInsertion.second;
	}

	Count MacroNamespace::setMacros( const MacroDefinition* macros, Count macroCount )
	{
		Count count = 0;
		for ( Index i = 0; i != macroCount; ++i )
			if ( !setMacro( macros[i].name, macros[i].value ) )
				++count;
		return count;
	}

	const String* MacroNamespace::findMacro( const String& macroID )
	{
		MacroLookup::const_iterator iFind = m_macros.find ( macroID );
		if ( iFind != m_macros.end() )
			return &iFind->second;

		return parentNamespace() ? parentNamespace()->findMacro( macroID ) : 0;
	}

	Count MacroNamespace::findMacros( const Char* inString, Count inStringLength, MacroPosition* macros, Count maxMacros )
	{
		String macroName;
		macroName.reserve(64); //, Reserve space for the macros name

		bool inMacro = false; //< Whether the current cursor is wihtin a macro name
		MacroPosition* macro = macros;
		const MacroPosition* endMacro = macros + maxMacros;
		const Char* inEndLast = inString;
		const Char* const stringEnd = inString + inStringLength; 
		for ( const Char* inPtr = inString; inPtr != stringEnd && macro != endMacro; ++inPtr )
		{
			Assert(inPtr[0] != terminator(inPtr) );

			if ( inPtr[0] == kMacroIdentifier &&  inPtr[1] == _T('(') )
			{
				assert( !inMacro );
				inMacro = true;
				macro->offset = Index(inPtr - inEndLast);
				inPtr += 2;
			}
			else				
			if ( inMacro && inPtr[0] == _T(')') )
			{
				const Char* macroStart = inEndLast + macro->offset + 2; //< + 2 for '$('
				macro->namelength = Index(inPtr - macroStart);
				macroName.assign( macroStart, macro->namelength );				
				macro->value = findMacro( macroName );
				if ( macro->value )
				{ 
					++macro; //< We don't record invalid/not-found macros
					inEndLast = inPtr+1;
				}
				inMacro = false;
			}
		}

		return Count(macro - macros);
	}
	
	Count MacroNamespace::expandMacros( const Char* inString, Count inStringLength, Char* outString, Count outStringLength  )
	{
		MacroPosition macros[8];
		Count macroCount = findMacros( inString, inStringLength, macros, 8 );

		Count macroDelta = 0;

		const Char* inPtr = inString;
		Char* outPtr = outString;
		const Char* const inEnd = inString + inStringLength;
		const Char* const outEnd = outString + outStringLength;
		for ( Index i =0; i != macroCount; ++i )
		{
			//Copy data between macros
			if ( macros[i].offset )
			{
				memcpy_s( outPtr, (Char8*)outEnd-(Char8*)outPtr, inPtr, macros[i].offset*sizeof(Char) );
				outPtr += macros[i].offset;
			}

			Assert( macros[i].value );
			memcpy_s( outPtr, (Char8*)outEnd-(Char8*)outPtr, macros[i].value->c_str(), macros[i].value->length() * sizeof(Char) );
			outPtr += macros[i].value->length();

			inPtr += macros[i].offset + macros[i].namelength + 3;
			//macros[i].value->length() - macros[i].namelength;
		}
		if ( inPtr != inEnd )
		{
			Count endLength = Count((Char8*)(inEnd+1)-(Char8*)inPtr);
			memcpy_s( outPtr, (Char8*)outEnd-(Char8*)outPtr, inPtr, endLength );
		}

		return true;
	}

} //END: kismo