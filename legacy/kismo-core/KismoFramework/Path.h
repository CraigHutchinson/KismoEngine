/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\KismoCore\KismoFramework\Path.h
 * Status: duplicate of kismo-core/_SOURCE_/KismoFramework/Path.h
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

#ifndef KISMOFRAMEWORK_PATH_H
#define KISMOFRAMEWORK_PATH_H

#include "KismoFramework/Types.h"
#include "KismoFramework/String.h"

namespace kismo
{
	class Path
	{
	public:
		
		static Index GetFilenamePart( const String& str );

		static Index GetFilenamePart( const Char* str );

		static Index GetFilenamePart( const Char* str, Count length );

		static Index GetExtensionPart( const String& str );

		static Index GetExtensionPart( const Char* str );

		static Index GetExtensionPart( const Char* str, Count length );

		static void FileParts( const Char* str, Count length, Index& extensionPart, Index& filenamePart );

	protected:

	public:
		Path() : m_filenameStart(0), m_extensionStart(0) {}
		Path( const Char* str ) { set(str); }
		Path( const String& str ) { set(str); }

		const Char* fileName() const
		{ return (m_filePath.size() ? m_filePath.c_str()+m_filenameStart : 0 ); }
		
		const Char* extension() const
		{ return (m_filePath.size() ? m_filePath.c_str()+m_extensionStart : 0 ); }


		String directory() const
		{ return (m_filePath.size() ? String( m_filePath.c_str(), m_filenameStart) : 0 ); }

		bool operator = ( const Char* str )
		{ return set(str); }

		bool operator = ( const String& str )
		{ return set(str); }

		String operator + ( const String& str ) const 
		{ return m_filePath+str; }

		String operator + ( const Char* str ) const 
		{ return m_filePath+str; }

		bool operator == ( const String& str ) const
		{ return m_filePath==str; }

		operator const String& () const
		{ return m_filePath; }
		
		const Char* c_str() const
		{ return m_filePath.c_str(); }

		const String& str() const
		{ return  m_filePath; }

		bool set( const String& str )
		{ return (str.size() ? set( str.c_str(), (Count)str.length() ) : false); }

		bool set( const Char* str )
		{ return (str? set( str, (Count)_tcsclen(str)) : false); }

		bool set( const Char* str, Count length )
		{
			Assert( str[length] == '\0' );
			m_filePath.assign( str, length );
			return parsePath();
		}

		//friend OStream& operator << ( OStream& stream, const Path& rhs )
		//{ stream << rhs.m_filePath; return stream; }

		//friend IStream& operator >> ( IStream& stream, Path& rhs )
		//{ 
		//	stream >> rhs.m_filePath; 
		//	if ( !rhs.parsePath() )
		//	{
		//		Assert(false);
		//	}
		//	return stream;
		//}

	protected:
		bool parsePath();


	protected:
		String m_filePath; //, The filePath
		UInt16 m_filenameStart; //< index to the last 
		UInt16 m_extensionStart; //< index to the last slash
	};

} //END: kismo

#endif