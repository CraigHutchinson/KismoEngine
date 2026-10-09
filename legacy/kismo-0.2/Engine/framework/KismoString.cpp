/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\framework\KismoString.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
#include "KismoString.h"
#include "KismoCommon.h"

#include <stdlib.h>

namespace kismo
{
	namespace frx
	{			

		//Explicitly declare the template implimentations
		//#pragma warning ( push )
		//#pragma warning (disable : 4231)//disable warnings on extern before template instantiation
		//extern template class _KismoExport String< k_Char8 >;
		//extern template class _KismoExport String< k_Char16 >;
		//#pragma warning ( pop )

		template< class _TChar >
		String< _TChar > String< _TChar >::number( k_UInt32 num )
		{
			return String< _TChar >();
		}

		template< class _TChar >
		String< _TChar >::String() :  
			m_text( 0 ), m_length( 0 ), m_capacity( 0 )
		{
		}	

		template< class _TChar >
		String< _TChar >::String( Size_T capacity ) :
			m_text( 0 ), m_length( 0 )
		{
			reserve( capacity );
		}

		template< class _TChar >
		String< _TChar >::String( Size_T length, _TChar fillChar = '\0' ) :
			m_length( length ), m_capacity( length + 1 )
		{
			m_text = new _TChar[m_capacity];
			frx::fillArray( m_text, length, fillChar );
			*(m_text + m_length) = '\0';
		}	

		template< class _TChar >
		String< _TChar >::String( const String& str ) : 
			m_text( 0 ) 
		{ 
			setString( str ); 
		}		

		template< class _TChar >
		String< _TChar >::String( const _TChar* str ) : 
			m_text( 0 ) 
		{ 
			setString( str ); 
		}

		template< class _TChar >
		String< _TChar >::~String()
		{
			destroy();			
		}							

		//template< class _TChar >
		//String< _TChar >::operator const _TChar* () const 
		//{
		//	return m_text; 
		//}

		template< class _TChar >
		const _TChar* String< _TChar >::c_str() const 
		{
			return m_text; 
		}

		template< class _TChar >
		bool String< _TChar >::isNull() const
		{
			return !isValidPointer( m_text );
		}

		template< class _TChar >
		void String< _TChar >::destroy()
		{
			if ( isValidPointer( m_text ) )
				safeDeleteArray( m_text );
			m_length = m_capacity = 0;
		}

		template< class _TChar >
		typename String< _TChar >::Size_T String< _TChar >::length() const 
		{ 
			return m_length; 
		}

		template< class _TChar >
		typename String< _TChar >::Size_T String< _TChar >::capacity() const 
		{ 
			return m_capacity; 
		}

		template< class _TChar >
		typename String< _TChar >::Size_T String< _TChar >::maxLength() const
		{ 
			return Size_MAX;
		}

		template< class _TChar >
		void String< _TChar >::setString( const String& str )
		{	
			m_length = str.m_length;
			reserve( m_length + 1, false );
			memcpy( m_text, str.m_text, (m_length + 1) * sizeof( _TChar ) );
		}

		template<>
		void String< k_Char8 >::setString( const k_Char8* str )
		{
			m_length = (Size_T)strlen( str );
			reserve( m_length + 1, false );
			memcpy( m_text, str, m_capacity * sizeof( k_Char8 ) );	
		}

		template<>
		void String< k_Char16 >::setString( const k_Char16* str )
		{
			m_length = (Size_T)wcslen( str );
			reserve( m_length + 1, false );
			memcpy( m_text, str, m_capacity * sizeof( k_Char16 ) );	
		}

		template< class _TChar >
		void String< _TChar >::reserve( Size_T count, k_Bool preserve )
		{
			if ( isNull() ) //imply create the memory
			{
				m_text = new _TChar[ count ];
				*m_text = '\0';
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
		void String< _TChar >::grow( Size_T increaseBy ) 
		{ 
			reserve( length() + increaseBy );
		}

		template< class _TChar >
		void String< _TChar >::compact()
		{
			
		}		

		template< class _TChar >
		inline _TChar& String< _TChar >::operator [] ( Index_T index ) 
		{ 
			assert( index < length() ); 
			return m_text[index]; 
		} 

		template< class _TChar >
		inline const _TChar& String< _TChar >::operator [] ( Index_T index ) const
		{ 
			assert( index < length() ); 
			return m_text[index];
		} 

		template< class _TChar >
		typename String< _TChar >::Index_T String< _TChar >::findFromStart( _TChar char2find, Index_T startIndex_T ) const
		{
			return Index_MAX;
		}

		template< class _TChar >
		typename String< _TChar >::Index_T String< _TChar >::findFromEnd( _TChar char2find, Index_T startIndex_T ) const
		{
			return Index_MAX;			
		}
		
		template< class _TChar >
		typename String< _TChar >::Index_T String< _TChar >::findFromStart( const String< _TChar >& str2find, Index_T startIndex_T ) const
		{
			return Index_MAX;
			
		}
		
		template< class _TChar >
		typename String< _TChar >::Index_T String< _TChar >::findFromEnd( const String< _TChar >& str2find, Index_T startIndex_T ) const
		{
			return Index_MAX;
			
		}

		template< class _TChar >
		void String< _TChar >::subString( String< _TChar >& ostr, Index_T start, Index_T end ) const
		{
			
		}
		
		template< class _TChar >
		String< _TChar > String< _TChar >::subString( Index_T start, Index_T end ) const
		{ 
			String str; 
			subString( str, start, end ); 
			return str;
		}
		
		template< class _TChar >
		typename String< _TChar >::Size_T String< _TChar >::subString( _TChar* ostr, Size_T ostrSize_T, Index_T start, Index_T end ) const
		{
			return 0;			
		}

		template< class _TChar >
		void String< _TChar >::left( String< _TChar >& ostr, Index_T index ) const
		{
		}
		
		template< class _TChar >
		String< _TChar > String< _TChar >::left( Index_T index ) const
		{ 
			String str; 
			left( str, index ); 
			return str; 
		}
		
		template< class _TChar >
		void String< _TChar >::right( String< _TChar >& ostr, Index_T index ) const
		{
		}
		
		template< class _TChar >
		String< _TChar > String< _TChar >::right( Index_T index ) const
		{ 
			String str; 
			right( str, index ); 
			return str; 
		}

		template< class _TChar >
		typename String< _TChar >::Size_T String< _TChar >::replace( _TChar char2find, _TChar replacement )
		{
			return 0;
		}
		
		template< class _TChar >
		typename String< _TChar >::Size_T String< _TChar >::replace( const String< _TChar >& str2find, const String& replacement )
		{
			return 0;
		}

		template< class _TChar >
		typename String< _TChar >::Size_T String< _TChar >::split( typename String< _TChar >::Array& strs, _TChar splitChar ) const
		{
			return 0;
		}

		template< class _TChar >
		void String< _TChar >::trimLeft()
		{
		}
		
		template< class _TChar >
		void String< _TChar >::trimRight()
		{
		}
		
		template< class _TChar >
		void String< _TChar >::trim() 
		{ 
			trimRight(); 
			trimLeft();
		}

		template< class _TChar >
		k_Int8 String< _TChar >::compare( const String& str, bool caseSensitive/*, Index_T start, Index_T end*/ ) const
		{
			return compare( str.m_text, caseSensitive );	
		}	

		template<>
		k_Int8 String< k_Char8 >::compare( const k_Char8* str, bool caseSensitive/*, Index_T start, Index_T end*/ ) const
		{
			if ( isNull() )
				return ( isValidPointer( str ) ? -1 : 0 ); //This string is less ( -1 ) | this string is equal ( 0 )
			if ( !isValidPointer( str ) ) //If the second string is null
				return -1; //this string is greater ( 1 )
			
			//perform the string comparison
			if ( caseSensitive )
				return strcmp( m_text, str );
			else
				return _stricmp( m_text, str );	
		}

		template<>
		k_Int8 String< k_Char16 >::compare( const k_Char16* str, bool caseSensitive/*, Index_T start, Index_T end*/ ) const
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
		void String< _TChar >::append( const String& str )
		{
			if ( str.isNull() ) return;
			reserve( m_length + str.m_length + 1 ); //reserve the required space
			memcpy( m_text + m_length, str.m_text, (str.m_length + 1) * sizeof( _TChar ) ); //Copy + terminator
			m_length += str.m_length;
		}

		template<>
		void String< k_Char8 >::append( const k_Char8* str )
		{
			if ( !isValidPointer( str ) ) return;
			Size_T tlen = (Size_T)strlen( str );
			reserve( m_length + tlen + 1 ); //reserve the required space	
			memcpy( m_text + m_length, str, (tlen + 1) * sizeof( k_Char8 ) ); //Copy + terminator
			m_length += tlen;
		}

		template<>
		void String< k_Char16 >::append( const k_Char16* str )
		{
			if ( !isValidPointer( str ) ) return;
			Size_T tlen = (Size_T)wcslen( str ); //get the lenght of the string + the terminator
			reserve( m_length + tlen + 1 ); //reserve the required space
			memcpy( m_text + m_length, str, (tlen + 1) * sizeof( k_Char16 ) ); //Copy + terminator
			m_length += tlen;
		}
	
		template< class _TChar >
		void String< _TChar >::append( const _TChar chr )
		{
			m_text[ m_length ] = chr; //set the null terminator to the new char
			reserve( m_length + 1 ); //reserve the required space
			m_text[ m_length ] = terminator(); //Set the null terminator
		}

		template< class _TChar >
		String< _TChar > String< _TChar >::operator + ( const String< _TChar >& str2 ) const
		{
			String tstr( m_length + str2.m_length + 1 ); //Create a string with the required capacity (Saves a resize & copy)
			tstr.setString( *this ); tstr.append( str2 ); return tstr; //set, append, and return
		}
		//template<>
		//String< k_Char8 > operator + ( const k_Char8* str1, const String< k_Char8 >& str2 )
		//{
		//	String tstr( (Size_T)strlen( str1 ) + str2.length() + 1 ); //create a string of the required size (Saves a resize & copy)
		//	tstr.setString( str1 ); tstr.append( str2 ); return tstr; //set, append, and return
		//}
		//template<>
		//String< k_Char16 > operator + ( const k_Char16* str1, const String< k_Char16 >& str2 )
		//{
		//	String tstr( (Size_T)wcslen( str1 ) + str2.length() + 1 ); //create a string of the required size (Saves a resize & copy)
		//	tstr.setString( str1 ); tstr.append( str2 ); return tstr; //set, append, and return
		//}
		template<>
		String< k_Char8 > String< k_Char8 >::operator + ( const k_Char8* str2 ) const
		{
			String tstr( m_length + (Size_T)strlen( str2 ) + 1 ); //create a string of the required size (Saves a resize & copy)
			tstr.setString( *this ); tstr.append( str2 ); return tstr; //set, append, and return
		}
		template<>
		String< k_Char16 > String< k_Char16 >::operator + ( const k_Char16* str2 ) const
		{
			String tstr( m_length + (Size_T)wcslen( str2 ) + 1 ); //create a string of the required size (Saves a resize & copy)
			tstr.setString( *this ); tstr.append( str2 ); return tstr; //set, append, and return
		}

		template<>
		String8 String< k_Char8 >::asString8() const
		{ return *this; }

		template<>
		String16 String< k_Char8 >::asString16() const
		{
			assert( false );
			return String16();
		}

		template<>
		String8 String< k_Char16 >::asString8() const
		{
			//get the length of the buffer required
			size_t newLength;
			wcstombs_s( &newLength, 0, 0, m_text, Size_MAX );

			//Create the new string
			String8 str( (String8::Size_T)newLength );
			wcstombs_s( &newLength, (k_Char8*)str.c_str(), str.capacity() * sizeof(k_Char8),
						m_text, m_capacity * sizeof(k_Char16) );

			return str;				
		}

		template<>
		String16 String< k_Char16 >::asString16() const
		{ return *this; }

		//template<>
		//StringT asStringT() const
		//{
		//}

		template<>
		k_Char8 String< k_Char8 >::terminator() const
		{ return '\n'; }

		template<>
		k_Char16 String< k_Char16 >::terminator() const
		{ return 0; }

		template class String< k_Char8 >;
		template class String< k_Char16 >;

	} //END: frx

} //END: kismo
