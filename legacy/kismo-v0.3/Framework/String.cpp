/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\Framework\String.cpp
 * Status: superseded by kismo-core/_SOURCE_/KismoFramework/String.cpp (same name, content differs)
 */
#include "String.h"
#include "Common.h"

#include <stdarg.h>
#include <stdlib.h>

namespace frx
{			
	const Char8 char8Terminator = '\0';
	const Char16 char16Terminator = L'\0';

	template< class _TChar >
	TString< _TChar > TString< _TChar >::number( UInt32 num )
	{
		return TString< _TChar >();
	}

	template< class _TChar >
	TString< _TChar >::TString() :  
		m_text( 0 ), m_length( 0 ), m_capacity( 0 )
	{
	}	

	template< class _TChar >
	TString< _TChar >::TString( Size_T capacity ) :
		m_text( 0 ), m_length( 0 )
	{
		reserve( capacity );
	}

	template< class _TChar >
	TString< _TChar >::TString( Size_T length, _TChar fillChar = '\0' ) :
		m_length( length ), m_capacity( length + 1 )
	{
		m_text = new _TChar[m_capacity];
		frx::fillArray( m_text, length, fillChar );
		*(m_text + m_length) = '\0';
	}	

	template< class _TChar >
	TString< _TChar >::TString( const TString& str ) : 
		m_text( 0 ) 
	{ 
		setString( str ); 
	}		

	template< class _TChar >
	TString< _TChar >::TString( const _TChar* str ) : 
		m_text( 0 ) 
	{ 
		setString( str ); 
	}

	template< class _TChar >
	TString< _TChar >::~TString()
	{
		destroy();			
	}							

	template< class _TChar >
	const _TChar* TString< _TChar >::c_str() const 
	{
		if ( isNull() )
			return &terminator();
		return m_text; 
	}

	template< class _TChar >
	bool TString< _TChar >::isNull() const
	{
		return !isValidPointer( m_text );
	}

	template< class _TChar >
	void TString< _TChar >::destroy()
	{
		safeDeleteArray( m_text );
		m_length = m_capacity = 0;
	}

	template< class _TChar >
	typename TString< _TChar >::Size_T TString< _TChar >::length() const 
	{ 
		return m_length; 
	}

	template< class _TChar >
	typename TString< _TChar >::Size_T TString< _TChar >::capacity() const 
	{ 
		return m_capacity; 
	}

	template< class _TChar >
	typename TString< _TChar >::Size_T TString< _TChar >::maxLength() const
	{ 
		return Size_MAX;
	}

	template< class _TChar >
	void TString< _TChar >::setString( const TString& str )
	{	
		if ( str.m_text )
		{
			m_length = str.m_length;
			reserve( m_length + 1, false );
			memcpy( m_text, str.m_text, (m_length + 1) * sizeof( _TChar ) );
		}
		else
			destroy();
	}

	template<>
	void TString< Char8 >::setString( const Char8* str )
	{
		if ( str )
		{
			m_length = (Size_T)strlen( str );
			reserve( m_length + 1, false );
			memcpy( m_text, str, (m_length + 1) * sizeof( Char8 ) );	
		}
		else
		{
			m_length = 0;
			if ( m_text ) //If there is a text array we clear it to null
			{			
				m_text[0] = terminator();
			}
		}
	}

	template<>
	void TString< Char16 >::setString( const Char16* str )
	{
		if ( str )
		{			
			m_length = (Size_T)wcslen( str );
			reserve( m_length + 1, false );
			memcpy( m_text, str, (m_length + 1) * sizeof( Char16 ) );	
		}
		else
		{
			m_length = 0;
			if ( m_text ) //If there is a text array we clear it to null
			{			
				m_text[0] = terminator();
			}
		}
	}

	template< class _TChar >
	void TString< _TChar >::reserve( Size_T count, Bool preserve )
	{
		if ( isNull() ) //imply create the memory
		{
			m_text = new _TChar[ count ];
			*m_text = terminator();
			m_capacity  = count;
		}
		else
		if ( m_capacity < count )
		{
			_TChar* newText = new _TChar[ count ]; //Create the new string
			if ( preserve )
			{
				memcpy( newText, m_text, (m_length + 1) * sizeof(_TChar) ); //Copy the current into the new string
				DEBUG_ONLY( memset( newText + (m_length + 1), 'R', (count - (m_length + 1)) * sizeof(_TChar) ) ); //Fill to the end
			}
			delete [] m_text; //Delete the old text
			m_capacity = count;
			m_text = newText;
		}
		else
		{
			/* Do Nothing */
		}
	}

	template< class _TChar >
	void TString< _TChar >::grow( Size_T increaseBy ) 
	{ 
		reserve( length() + increaseBy );
	}

	template< class _TChar >
	void TString< _TChar >::compact()
	{
		
	}		

	template< class _TChar >
	inline _TChar& TString< _TChar >::operator [] ( Index_T index ) 
	{ 
		assert( index < length() ); 
		return m_text[index]; 
	} 

	template< class _TChar >
	inline const _TChar& TString< _TChar >::operator [] ( Index_T index ) const
	{ 
		assert( index < length() ); 
		return m_text[index];
	} 

	template< class _TChar >
	typename TString< _TChar >::Index_T TString< _TChar >::findFromStart( _TChar char2find, Index_T startIndex_T ) const
	{
		return Index_MAX;
	}

	template< class _TChar >
	typename TString< _TChar >::Index_T TString< _TChar >::findFromEnd( _TChar char2find, Index_T startIndex_T ) const
	{
		return Index_MAX;			
	}
	
	template< class _TChar >
	typename TString< _TChar >::Index_T TString< _TChar >::findFromStart( const TString< _TChar >& str2find, Index_T startIndex_T ) const
	{
		return Index_MAX;
		
	}
	
	template< class _TChar >
	typename TString< _TChar >::Index_T TString< _TChar >::findFromEnd( const TString< _TChar >& str2find, Index_T startIndex_T ) const
	{
		return Index_MAX;
		
	}

	template< class _TChar >
	void TString< _TChar >::subString( TString< _TChar >& ostr, Index_T start, Index_T end ) const
	{
		if ( end >= m_length ) end = m_length-1; //make sure we don't go past the end of the string
		if ( start > end ) start = end; ///Make sure we get a positive length!

		Size_T newLength = end - start;
		ostr.reserve( newLength + 1 ); // reserve space for the new string
		memcpy( ostr.m_text, m_text + start, newLength * sizeof( _TChar ) );
		ostr.m_text[ newLength ] = terminator(); //set the terminator
		ostr.m_length = newLength;
	}
	
	template< class _TChar >
	TString< _TChar > TString< _TChar >::subString( Index_T start, Index_T end ) const
	{ 
		TString str; 
		subString( str, start, end ); 
		return str;
	}
	
	template< class _TChar >
	typename TString< _TChar >::Size_T TString< _TChar >::subString( _TChar* ostr, Size_T ostrSize_T, Index_T start, Index_T end ) const
	{
		return 0;			
	}

	template< class _TChar >
	void TString< _TChar >::left( TString< _TChar >& ostr, Index_T index ) const
	{
		if ( index > m_length ) index = m_length; //make sure we don't go past the end of the string
		ostr.reserve( index + 1 ); // reserve space for the new string
		memcpy( ostr.m_text, m_text, index * sizeof( _TChar ) );
		ostr.m_text[ index ] = terminator(); //set the terminator
		ostr.m_length = index;
	}
	
	template< class _TChar >
	TString< _TChar > TString< _TChar >::left( Index_T index ) const
	{ 
		TString str; 
		left( str, index ); 
		return str; 
	}
	
	template< class _TChar >
	void TString< _TChar >::right( TString< _TChar >& ostr, Index_T index ) const
	{
		if ( index >= m_length ) index = m_length; //make sure we don't go past the end of the string
		ostr.reserve( (m_length - index) + 1 ); // reserve space for the new string
		ostr.m_length = (m_length - index);
		if ( ostr.m_length )
			memcpy( ostr.m_text, m_text + index, (ostr.m_length + 1) * sizeof( _TChar ) );
		else
			m_text[ 0 ] = terminator();
	}
	
	template< class _TChar >
	TString< _TChar > TString< _TChar >::right( Index_T index ) const
	{ 
		TString str; 
		right( str, index ); 
		return str; 
	}

	template< class _TChar >
	typename TString< _TChar >::Size_T TString< _TChar >::replace( _TChar char2find, _TChar replacement )
	{
		return 0;
	}
	
	template< class _TChar >
	typename TString< _TChar >::Size_T TString< _TChar >::replace( const TString< _TChar >& str2find, const TString& replacement )
	{
		return 0;
	}

	template< class _TChar >
	typename TString< _TChar >::Size_T TString< _TChar >::split( typename TString< _TChar >::Array& strs, _TChar splitChar ) const
	{
		return 0;
	}

	template< class _TChar >
	void TString< _TChar >::trimLeft( _TChar char2find )
	{
		if ( m_length )
		{
			Size_T pos = 0;
			while(	pos < m_length 
					&& m_text[ pos ] == char2find )
				++pos;
			
			if ( pos == 0 )
			{
				return;
			}
			if ( pos == m_length )
			{
				m_length = 0;
				m_text[0] = terminator();
			}
			else
			{
				m_length = m_length - pos ;
				memmove_s( m_text, (m_length+1) * sizeof( _TChar), m_text + pos, (m_length + 1) * sizeof( _TChar) );
			}
		}
	}
	
	template< class _TChar >
	void TString< _TChar >::trimRight( _TChar char2find )
	{
		if ( m_length )
		{
			Size_T pos = m_length-1;
			while(	pos < m_length 
					&& m_text[ pos ] == char2find )
				--pos;
			
			m_text[ ++pos ] = terminator();//NOTE: If the pos has wrapped below 0 it will wrap back here
			m_length = pos;
		}
	}
	
	template< class _TChar >
	void TString< _TChar >::trim( _TChar char2find ) 
	{ 
		trimRight(char2find); 
		trimLeft(char2find);
	}

	template< class _TChar >
	Int8 TString< _TChar >::compare( const TString& str, bool caseSensitive/*, Index_T start, Index_T end*/ ) const
	{
		return compare( str.m_text, caseSensitive );	
	}	

	template<>
	Int8 TString< Char8 >::compare( const Char8* str, bool caseSensitive/*, Index_T start, Index_T end*/ ) const
	{
		if ( isNull() )
			return ( isValidPointer( str ) ? -1 : 0 ); //This string is less ( -1 ) | this string is equal ( 0 )
		if ( !isValidPointer( str ) ) //If the second string is null
			return 1; //this string is greater ( 1 )
		
		//perform the string comparison
		if ( caseSensitive )
			return strcmp( m_text, str );
		else
			return _stricmp( m_text, str );	
	}

	template<>
	Int8 TString< Char16 >::compare( const Char16* str, bool caseSensitive/*, Index_T start, Index_T end*/ ) const
	{
		if ( isNull() )
			return ( isValidPointer( str ) ? -1 : 0 ); //This string is less ( -1 ) | this string is equal ( 0 )
		if ( !isValidPointer( str ) ) //If the second string is null
			return -1; //this string is greater ( 1 )
		
		//perform the string comparison
		if ( caseSensitive )
			return wcscmp( m_text, str );
		else
			return _wcsicmp( m_text, str );		
	}

	template< class _TChar >
	void TString< _TChar >::append( const TString& str )
	{
		if ( str.isNull() ) return;
		reserve( m_length + str.m_length + 1 ); //reserve the required space
		memcpy( m_text + m_length, str.m_text, (str.m_length + 1) * sizeof( _TChar ) ); //Copy + terminator
		m_length += str.m_length;
	}

	template<>
	void TString< Char8 >::append( const Char8* str )
	{
		if ( !isValidPointer( str ) ) return;
		Size_T tlen = (Size_T)strlen( str );
		reserve( m_length + tlen + 1 ); //reserve the required space	
		memcpy( m_text + m_length, str, (tlen + 1) * sizeof( Char8 ) ); //Copy + terminator
		m_length += tlen;
	}

	template<>
	void TString< Char16 >::append( const Char16* str )
	{
		if ( !isValidPointer( str ) ) return;
		Size_T tlen = (Size_T)wcslen( str ); //get the lenght of the string + the terminator
		reserve( m_length + tlen + 1 ); //reserve the required space
		memcpy( m_text + m_length, str, (tlen + 1) * sizeof( Char16 ) ); //Copy + terminator
		m_length += tlen;
	}

	template< class _TChar >
	void TString< _TChar >::append( const _TChar chr )
	{
		m_text[ m_length ] = chr; //set the null terminator to the new char
		reserve( (m_length + 1) + 1 ); //reserve the required space, includes null character
		++m_length; ///Incement the length for the new character
		m_text[ m_length ] = terminator(); //Set the null terminator
	}

	template< class _TChar >
	TString< _TChar > TString< _TChar >::operator + ( const TString< _TChar >& str2 ) const
	{
		TString tstr( m_length + str2.m_length + 1 ); //Create a string with the required capacity (Saves a resize & copy)
		tstr.setString( *this ); tstr.append( str2 ); return tstr; //set, append, and return
	}

	TString< Char8 > operator + ( const Char8* str1, const TString< Char8 >& str2 )
	{
		TString< Char8 > tstr( (TString< Char8 >::Size_T)strlen( str1 ) + str2.length() + 1 ); //create a string of the required size (Saves a resize & copy)
		tstr.setString( str1 ); tstr.append( str2 ); return tstr; //set, append, and return
	}

	TString< Char16 > operator + ( const Char16* str1, const TString< Char16 >& str2 )
	{
		TString< Char16 > tstr( (TString< Char16 >::Size_T)wcslen( str1 ) + str2.length() + 1 ); //create a string of the required size (Saves a resize & copy)
		tstr.setString( str1 ); tstr.append( str2 ); return tstr; //set, append, and return
	}
	template<>
	TString< Char8 > TString< Char8 >::operator + ( const Char8* str2 ) const
	{
		TString tstr( m_length + (Size_T)strlen( str2 ) + 1 ); //create a string of the required size (Saves a resize & copy)
		tstr.setString( *this ); tstr.append( str2 ); return tstr; //set, append, and return
	}
	template<>
	TString< Char16 > TString< Char16 >::operator + ( const Char16* str2 ) const
	{
		TString tstr( m_length + (Size_T)wcslen( str2 ) + 1 ); //create a string of the required size (Saves a resize & copy)
		tstr.setString( *this ); tstr.append( str2 ); return tstr; //set, append, and return
	}
	template<>
	TString< Char8 > TString< Char8 >::operator + ( const Char8 chr2 ) const
	{
		TString tstr( m_length + 1 + 1 ); //create a string of the required size (Saves a resize & copy)
		tstr.setString( *this ); tstr.append( chr2 ); return tstr; //set, append, and return
	}
	template<>
	TString< Char16 > TString< Char16 >::operator + ( const Char16 chr2 ) const
	{
		TString tstr( m_length + 1 + 1 ); //create a string of the required size (Saves a resize & copy)
		tstr.setString( *this ); tstr.append( chr2 ); return tstr; //set, append, and return
	}

	template<>
	String8 TString< Char8 >::asString8() const
	{ return *this; }

	template<>
	String16 TString< Char8 >::asString16() const
	{
		assert( false );
		return String16();
	}

	template<>
	String8 TString< Char16 >::asString8() const
	{
		//get the length of the buffer required
		size_t newLength;
		wcstombs_s( &newLength, 0, 0, m_text, Size_MAX );

		//Create the new string
		String8 str( (String8::Size_T)newLength, '-' );
		wcstombs_s( &newLength, (Char8*)str.c_str(), str.capacity() * sizeof(Char8),
					m_text, m_capacity * sizeof(Char16) );

		return str;				
	}

	template<>
	String16 TString< Char16 >::asString16() const
	{ return *this; }

	//template<>
	//StringT asStringT() const
	//{
	//}

	//void to( CString& out ) const { out.setString( m_text ); }
	//void to( int& out ) const { out = atoi( m_text ); }
	//void to( long& out ) const { out = atol( m_text ); }
	//void to( BYTE& out ) const { out = (BYTE)atoi( m_text ); }
	//void to( UINT& out ) const { TCHAR* pEnd; out = (UINT)strtoul( m_text, &pEnd, 10 ); }
	//void to( ULONG& out ) const { TCHAR* pEnd; out = (ULONG)strtoul( m_text, &pEnd, 10 ); }
	//void to( FLOAT& out ) const { out = (FLOAT)atof( m_text ); }
	//void to( LFLOAT& out ) const { out = (LFLOAT)atof( m_text );}
	//void to( bool& out ) const { out = atoi( m_text ) != 0;}

	template<> UInt32 TString< Char8 >::asUInt32() const
	{ return strtoul( m_text, NULL, 0 ); }
	template<> UInt32 TString< Char16 >::asUInt32() const
	{ return wcstoul( m_text, NULL, 0 ); }

	template<> Int32 TString< Char8 >::asInt32() const
	{ return strtol( m_text, NULL, 0 ); }
	template<> Int32 TString< Char16 >::asInt32() const
	{ return wcstol( m_text, NULL, 0 ); }

	template< class _TChar >
	Bool TString< _TChar >::asBool() const
	{
		assert( false );
		return false;
	}

	template<> Float32 TString< Char8 >::asFloat32() const
	{ return (Float32)strtod( m_text, NULL ); }
	template<> Float32 TString< Char16 >::asFloat32() const
	{ return (Float32)wcstod( m_text, NULL ); }

	template<> Float64 TString< Char8 >::asFloat64() const
	{ return strtod( m_text, NULL ); }
	template<> Float64 TString< Char16 >::asFloat64() const
	{ return wcstod( m_text, NULL ); }

	template<> Real TString< Char8 >::asReal() const
	{ return (Real)strtod( m_text, NULL ); }
	template<> Real TString< Char16 >::asReal() const
	{ return (Real)wcstod( m_text, NULL ); }

	template<>
	const Char8& TString< Char8 >::terminator() const
	{ return char8Terminator; }

	template<>
	const Char16&  TString< Char16 >::terminator() const
	{ return char16Terminator; }


	//template<>
	//bool TString< Char8 >::scan( const Char8* format, ... ) const
	//{
	//	va_list argList;

	//	va_start(argList, format);
	//	return sscanf_s( m_text, format, argList) != EOF;
	//}

	//template<>
	//bool TString< Char16 >::scan( const Char16* format, ... ) const
	//{
	//	va_list argList;

	//	va_start(argList, format);
	//	return swscanf_s( m_text, format, argList) != EOF;
	//}

	template class TString< Char8 >;
	template class TString< Char16 >;

} //END: frx
