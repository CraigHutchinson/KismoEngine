/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\KismoCore\_SOURCE_\KismoFramework\String.cpp
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



#include "KismoFramework\Common.h"
#include "KismoFramework/String.h"

#include <stdarg.h>
#include <stdlib.h>

namespace kismo
{		
#if 0	
	const Char8 char8Terminator = '\0';
	const Char16 char16Terminator = L'\0';

	template< class _TChar >
	StringT< _TChar > StringT< _TChar >::number( UInt32 num )
	{
		return StringT< _TChar >();
	}

	template< class _TChar >
	StringT< _TChar >::StringT()
	{
	}	

	template< class _TChar >
	StringT< _TChar >::StringT( Size16 capacity )
	{
		reserve( capacity );
	}

	template< class _TChar >
	StringT< _TChar >::StringT( Size16 length, _TChar fillChar = '\0' )
	{
		resize(length+1);
		kismo::fillArray( c_ptr(), length, fillChar );
		(*this)[size()-1] = terminator();
	}	

	template< class _TChar >
	StringT< _TChar >::StringT( const StringT& str )
	{ 
		assign( str ); 
	}		

	template< class _TChar >
	StringT< _TChar >::StringT( const _TChar* str )
	{ 
		assign( str ); 
	}

	template< class _TChar >
	StringT< _TChar >::StringT( const _TChar* str, Count length )
	{ 
		if ( str )
			assign( str, length ? length : measure(str) ); 
	}

	template< class _TChar >
	StringT< _TChar >::~StringT()
	{		
	}							

	template< class _TChar >
	const _TChar* StringT< _TChar >::c_str() const 
	{
		if ( isNull() )
			return &terminator();
		return c_ptr(); 
	}

	template< class _TChar >
	bool StringT< _TChar >::isNull() const
	{
		return c_ptr() == 0;
	}


	template< class _TChar >
	void StringT< _TChar >::assign( const StringT& str )
	{	
		if ( str.c_ptr() )
			assign( str.c_ptr(), str.length() );
		else
			clear();
	}

	template< class _TChar >
	void StringT< _TChar >::assign( const _TChar* str )
	{
		if ( str )
			assign( str, measure( str ) );	
		else
			clear();
	}

	template< class _TChar >
	void StringT< _TChar >::assign( const _TChar* str, Count length )
	{
		Assert( measure(str) >= length );
		++length;
		resize( length );
		Vector::assign( str, length );
		at(length-1) = terminator();
	}

	template< class _TChar >
	void StringT< _TChar >::grow( Size16 increaseBy ) 
	{ 
		reserve( length() + increaseBy );
	}

	template< class _TChar >
	void StringT< _TChar >::compact()
	{
		resize( size() );		
	}		

	template< class _TChar >
	typename Index16 StringT< _TChar >::findFromStart( _TChar char2find, Index16 startIndex_T ) const
	{
		return Index16_Max;
	}

	template< class _TChar >
	typename Index16 StringT< _TChar >::findFromEnd( _TChar char2find, Index16 startIndex_T ) const
	{
		return Index16_Max;			
	}
	
	template< class _TChar >
	typename Index16 StringT< _TChar >::findFromStart( const StringT< _TChar >& str2find, Index16 startIndex_T ) const
	{
		return Index16_Max;
		
	}
	
	template< class _TChar >
	typename Index16 StringT< _TChar >::findFromEnd( const StringT< _TChar >& str2find, Index16 startIndex_T ) const
	{
		return Index16_Max;
		
	}

	template< class _TChar >
	void StringT< _TChar >::subString( StringT< _TChar >& ostr, Index16 start, Index16 end ) const
	{
		if ( end >= size() ) end = size(); //make sure we don't go past the end of the string
		if ( start > end ) start = end; ///Make sure we get a positive length!

		Size16 newLength = end - start;
		ostr.resize( newLength + 1 ); // reserve space for the new string
		ostr.assign( c_ptr() + start, newLength );
		ostr.c_ptr()[ newLength ] = terminator(); //set the terminator
	}
	
	template< class _TChar >
	StringT< _TChar > StringT< _TChar >::subString( Index16 start, Index16 end ) const
	{ 
		StringT str; 
		subString( str, start, end ); 
		return str;
	}
	
	template< class _TChar >
	typename Size16 StringT< _TChar >::subString( _TChar* ostr, Size16 ostrSize_T, Index16 start, Index16 end ) const
	{
		return 0;			
	}

	template< class _TChar >
	void StringT< _TChar >::left( StringT< _TChar >& ostr, Index16 index ) const
	{
		if ( index > size() ) index = size(); //make sure we don't go past the end of the string
		ostr.resize( index + 1 ); // reserve space for the new string
		ostr.assign( c_ptr(), index );
		ostr.c_ptr()[ index ] = terminator(); //set the terminator
	}
	
	template< class _TChar >
	StringT< _TChar > StringT< _TChar >::left( Index16 index ) const
	{ 
		StringT str; 
		left( str, index ); 
		return str; 
	}
	
	template< class _TChar >
	void StringT< _TChar >::right( StringT< _TChar >& ostr, Index16 index ) const
	{
		Assert( index < size() );// index = size(); //make sure we don't go past the end of the string
		ostr.resize( (size() - index) + 1 ); // reserve space for the new string
		
		if ( ostr.size() )
			ostr.assign( c_ptr() + index, ostr.size() );
	}
	
	template< class _TChar >
	const _TChar* StringT< _TChar >::right( Index16 index ) const
	{  
		return c_ptr() + index; 
	}

	template< class _TChar >
	typename Size16 StringT< _TChar >::replace( _TChar char2find, _TChar replacement )
	{
		return 0;
	}
	
	template< class _TChar >
	typename Size16 StringT< _TChar >::replace( const StringT< _TChar >& str2find, const StringT& replacement )
	{
		return 0;
	}

	template< class _TChar >
	typename Size16 StringT< _TChar >::split( typename StringT< _TChar >::Array& strs, _TChar splitChar ) const
	{
		return 0;
	}

	template< class _TChar >
	void StringT< _TChar >::trimLeft( _TChar char2find )
	{
		if ( size() )
		{
			Size16 pos = 0;
			while(	pos < size() 
					&& c_ptr()[ pos ] == char2find )
				++pos;
			
			if ( pos != 0 )
			{
				if ( pos == size() )
					clear();
				else
				{
					memmove_s( c_ptr(), size() * sizeof( _TChar), c_ptr() + pos, size() * sizeof( _TChar) );
					resize( size() - pos );
				}
			}
		}
	}
	
	template< class _TChar >
	void StringT< _TChar >::trimRight( _TChar char2find )
	{
		if ( size() )
		{
			Size16 length = size()-1;
			while( c_ptr()[ length ] != char2find )
			{
				--length;
				if ( length >= size() )
					return; //< Didn not find character before end of string
			}
			
			++length;
			c_ptr()[ length ] = terminator();
			resize( length+1 );
		}
	}
	
	template< class _TChar >
	void StringT< _TChar >::trim( _TChar char2find ) 
	{ 
		trimRight(char2find); 
		trimLeft(char2find);
	}

	template< class _TChar >
	Int8 StringT< _TChar >::compare( const StringT& str, bool caseSensitive/*, Index16 start, Index16 end*/ ) const
	{
		return compare( str.c_ptr(), caseSensitive );	
	}	

	template<>
	Int8 StringT< Char8 >::compare( const Char8* str, bool caseSensitive/*, Index16 start, Index16 end*/ ) const
	{
		if ( isNull() )
			return ( isValidPointer( str ) ? -1 : 0 ); //This string is less ( -1 ) | this string is equal ( 0 )
		if ( !isValidPointer( str ) ) //If the second string is null
			return 1; //this string is greater ( 1 )
		
		//perform the string comparison
		if ( caseSensitive )
			return strcmp( c_ptr(), str );
		else
			return _stricmp( c_ptr(), str );	
	}

	template<>
	Int8 StringT< Char16 >::compare( const Char16* str, bool caseSensitive/*, Index16 start, Index16 end*/ ) const
	{
		if ( isNull() )
			return ( isValidPointer( str ) ? -1 : 0 ); //This string is less ( -1 ) | this string is equal ( 0 )
		if ( !isValidPointer( str ) ) //If the second string is null
			return -1; //this string is greater ( 1 )
		
		//perform the string comparison
		if ( caseSensitive )
			return wcscmp( c_ptr(), str );
		else
			return _wcsicmp( c_ptr(), str );		
	}

	template< class _TChar >
	void StringT< _TChar >::append( const StringT& str )
	{
		Size16 oldSize = size()-1;
		resize( oldSize + str.size()-1 ); //reserve the required space	
		Vector::assign( oldSize, str.c_str(), str.size() );
	}

	template< class _TChar >
	void StringT< _TChar >::append( const _TChar* str )
	{ append( str, measure( str ) ); }

	template< class _TChar >
	void StringT< _TChar >::append( const _TChar* str, Count strLen  )
	{
		Size16 tlen = strLen + 1;
		Size16 oldLength = length();
		resize( oldLength + tlen ); //reserve the required space	
		Vector::assign( oldLength, str, tlen );
	}

	template< class _TChar >
	void StringT< _TChar >::append( const _TChar chr )
	{
		reserve( size() + 1 ); //reserve the required space	
		(*this)[ size()-2] = chr;
		(*this)[ size()-1] = terminator();
	}

	template< class _TChar >
	StringT< _TChar > StringT< _TChar >::operator + ( const StringT< _TChar >& str2 ) const
	{
		StringT tstr( size() + str2.size() + 1 ); //Create a string with the required capacity (Saves a resize & copy)
		tstr.assign( *this ); tstr.append( str2 ); return tstr; //set, append, and return
	}

	template< class _TChar >
	StringT< _TChar > operator + ( const _TChar* str1, const StringT< _TChar >& str2 )
	{
		StringT< _TChar > tstr( measure( str1 ) + str2.length() + 1 ); //create a string of the required size (Saves a resize & copy)
		tstr.assign( str1 ); tstr.append( str2 ); return tstr; //set, append, and return
	}

	template< class _TChar >
	StringT< _TChar > StringT< _TChar >::operator + ( const _TChar* str2 ) const
	{
		StringT tstr( size() + measure( str2 ) + 1 ); //create a string of the required size (Saves a resize & copy)
		tstr.assign( *this ); tstr.append( str2 ); return tstr; //set, append, and return
	}

	template< class _TChar >
	StringT< _TChar > StringT< _TChar >::operator + ( const _TChar chr2 ) const
	{
		StringT tstr( size() + 1 + 1 ); //create a string of the required size (Saves a resize & copy)
		tstr.assign( *this ); tstr.append( chr2 ); return tstr; //set, append, and return
	}

	template<>
	String8 StringT< Char8 >::asString8() const
	{ return *this; }

	template<>
	String16 StringT< Char8 >::asString16() const
	{
		Assert( false );
		return String16();
	}

	template<>
	String8 StringT< Char16 >::asString8() const
	{
		//get the length of the buffer required
		size_t newLength;
		wcstombs_s( &newLength, 0, 0, c_ptr(), Size16_Max );

		//Create the new string
		String8 str( (Size16)newLength-1, '-' );
		wcstombs_s( &newLength, (Char8*)str.c_str(), str.capacity() * sizeof(Char8),
					c_ptr(), capacity() * sizeof(Char16) );

		return str;				
	}

	template<>
	String16 StringT< Char16 >::asString16() const
	{ return *this; }

	template<>
	String StringT< Char8 >::asString() const
	{ 
#ifdef  _UNICODE
		return asString16();
#else
		return *this;
#endif
	}

	template<>
	String StringT< Char16 >::asString() const
	{ 
#ifdef  _UNICODE
		return *this;
#else
		return asString8();
#endif
	}

	//template<>
	//StringT asStringT() const
	//{
	//}

	//void to( CString& out ) const { out.assign( c_ptr() ); }
	//void to( int& out ) const { out = _tstoi( c_ptr() ); }
	//void to( long& out ) const { out = atol( c_ptr() ); }
	//void to( BYTE& out ) const { out = (BYTE)_tstoi( c_ptr() ); }
	//void to( UINT& out ) const { TCHAR* pEnd; out = (UINT)strtoul( c_ptr(), &pEnd, 10 ); }
	//void to( ULONG& out ) const { TCHAR* pEnd; out = (ULONG)strtoul( c_ptr(), &pEnd, 10 ); }
	//void to( FLOAT& out ) const { out = (FLOAT)_tstof( c_ptr() ); }
	//void to( LFLOAT& out ) const { out = (LFLOAT)_tstof( c_ptr() );}
	//void to( bool& out ) const { out = _tstoi( c_ptr() ) != 0;}

	template<> UInt32 StringT< Char8 >::asUInt32() const
	{ return strtoul( c_ptr(), NULL, 0 ); }
	template<> UInt32 StringT< Char16 >::asUInt32() const
	{ return wcstoul( c_ptr(), NULL, 0 ); }

	template<> Int32 StringT< Char8 >::asInt32() const
	{ return strtol( c_ptr(), NULL, 0 ); }
	template<> Int32 StringT< Char16 >::asInt32() const
	{ return wcstol( c_ptr(), NULL, 0 ); }

	template< class _TChar >
	Bool StringT< _TChar >::asBool() const
	{
		Assert( false );
		return false;
	}

	template<> Float32 StringT< Char8 >::asFloat32() const
	{ return (Float32)strtod( c_ptr(), NULL ); }
	template<> Float32 StringT< Char16 >::asFloat32() const
	{ return (Float32)wcstod( c_ptr(), NULL ); }

	template<> Float64 StringT< Char8 >::asFloat64() const
	{ return strtod( c_ptr(), NULL ); }
	template<> Float64 StringT< Char16 >::asFloat64() const
	{ return wcstod( c_ptr(), NULL ); }

	template<> Real StringT< Char8 >::asReal() const
	{ return (Real)strtod( c_ptr(), NULL ); }
	template<> Real StringT< Char16 >::asReal() const
	{ return (Real)wcstod( c_ptr(), NULL ); }

	template<>
	const Char8& StringT< Char8 >::terminator()
	{ return char8Terminator; }

	template<>
	const Char16&  StringT< Char16 >::terminator()
	{ return char16Terminator; }

	template<>
	Count StringT< Char8 >::measure( const Char8* str )
	{ return (Count)strlen(str); }

	template<>
	Count StringT< Char16 >::measure( const Char16* str)
	{ return (Count)wcslen(str); }

	//template<>
	//bool StringT< Char8 >::scan( const Char8* format, ... ) const
	//{
	//	va_list argList;

	//	va_start(argList, format);
	//	return sscanf_s( c_ptr(), format, argList) != EOF;
	//}

	//template<>
	//bool StringT< Char16 >::scan( const Char16* format, ... ) const
	//{
	//	va_list argList;

	//	va_start(argList, format);
	//	return swscanf_s( c_ptr(), format, argList) != EOF;
	//}

	template class StringT< Char8 >;
	template class StringT< Char16 >;

#endif
	
	ConvertString< String16, String8 >::ConvertString( const String16& src )  
	{ 
		//get the length of the buffer required
		size_t newLength;
		wcstombs_s( &newLength, 0, 0, src.c_str(), Size16_Max );

		//Create the new string
		m_dest.resize( (Size16)newLength-1, '-' );
		wcstombs_s( &newLength, (Char8*)m_dest.c_str(), m_dest.capacity() * sizeof(Char8),
					src.c_str(), src.capacity() * sizeof(Char16) );			
	}	

	ConvertString< String8, String16 >::ConvertString( const String8& src )  
	{ 
		AssertEx( false, "Implement conversion from char8 to char16!" );
	}	

} //END: kismo
