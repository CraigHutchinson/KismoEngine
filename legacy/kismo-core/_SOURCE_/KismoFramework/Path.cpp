/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\KismoCore\_SOURCE_\KismoFramework\Path.cpp
 * Status: current (newest copy of this file name in the archive)
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


#include "KismoFramework/Path.h"

namespace kismo
{
	Index Path::GetFilenamePart( const String& str )
	{ return GetFilenamePart(str.c_str(),(Count)str.length() ); }

	Index Path::GetFilenamePart( const Char* str )
	{ return GetFilenamePart(str,(Count)_tcsclen(str) ); }

	Index Path::GetFilenamePart( const Char* str, Count length )
	{
		for ( const Char* pstr = str + length; pstr >= str; --pstr, --length )
		{
			if ( *pstr == '\\' || *pstr == '/')
				return length+1;
		}
		return Index_Max;
	}

	Index Path::GetExtensionPart( const String& str )
	{ return GetExtensionPart(str.c_str(),(Count)str.length() ); }

	Index Path::GetExtensionPart( const Char* str )
	{ return GetExtensionPart(str,(Count)_tcsclen(str) ); }

	Index Path::GetExtensionPart( const Char* str, Count length )
	{
		for ( const Char* pstr = str + length; pstr >= str; --pstr, --length )
		{
			if ( *pstr == '.')
				return length+1;
		}
		return Index_Max;
	}

	void Path::FileParts( const Char* str, Count length, Index& extensionPart, Index& filenamePart )
	{
		filenamePart = extensionPart = Index_Max;
		for ( const Char* pstr = str + length; pstr >= str; --pstr, --length )
		{
			if ( extensionPart == Index_Max && *pstr == '.' )
				extensionPart = length+1;
			else
			if ( *pstr == '\\' || *pstr == '/') //< If last slash not found then se the start fo the filename
			{
				filenamePart = length+1;
				return;
			}
		}
	}

	bool Path::parsePath()
	{
		Index extensionStart, filenameStart;
		FileParts( m_filePath.c_str(), (Count)m_filePath.length(), extensionStart, filenameStart );
		m_extensionStart = (UInt16)(extensionStart!= Index_Max ? extensionStart : m_filePath.length());
		m_filenameStart = (UInt16)(filenameStart!= Index_Max ? filenameStart : m_filePath.length());
		return true;
	}

} //END: kismo