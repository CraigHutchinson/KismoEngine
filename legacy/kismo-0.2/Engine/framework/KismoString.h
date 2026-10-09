/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\framework\KismoString.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef _KISMO_STRING_H
#define _KISMO_STRING_H

#include "framework/KismoTypes.h"
#include "framework/KismoMath.h"
#include "framework/KismoPlatform.h"

namespace kismo
{
	namespace frx
	{
		/** Kismo String wrapper class */
		template < class _TChar >
		class _KismoExport String
		{
		public:			
			typedef k_Vector< String > Array;
			typedef k_UInt16 Size_T;
			typedef k_UInt16 Index_T;
			static const Index_T Index_MAX = kc_UInt16_MAX-1; //kc_UInt16_MAX is invalid 
			static const Size_T Size_MAX = kc_UInt16_MAX-2;
			static const Index_T npos = Index_MAX; ///< STL Compliant end terminator/invalid index
		public:
			static String number( k_UInt32 num );
		public:
			/** Default constructor to create an empty string */
			String();		
			/** Constructor creates an empty string allocating a chunk of memory for future use
			\param[in] length The number fo characters to reserve */
			explicit String( Size_T capacity );
			/** Constructor creates a string filled with the specified character 
			\param[in] length The length of the new  string 
			\param[in] fillChar Character to fill the string with */
			explicit String( Size_T length, _TChar fillChar );	
			/** Copy constructor
			\params[in] str The string that holds the original string */
			String( const String& str );		
			/** Construct the string from a C style string
			\param[in] str C Stye string array */
			String( const _TChar* str );
			/** destructor frees the memory used to store this string */
			~String();							
			/** Return a pointer to the internal string representation */
			//TODO: THis has issues!
			//operator const _TChar* () const;
			/** Return a pointer to the internal string representation */
			const _TChar* c_str() const;
			/** Whether the string is holding any data */
			bool isNull() const;			
			/** Destroy the interna string representation freeing any used memory */
			void destroy();
			/** Get the length of the string in characters (excludes the null terminator) */
			Size_T length() const;
			/** Get the capacity of the string buffer in characters */
			Size_T capacity() const;
			/** Returns the maximum string length allowed */
			Size_T maxLength() const;
			
			/** Set the string to that of another String
			\param[in] str original string */
			void setString( const String& str );	
			/** Set the string to that of another C-Style String
			\param[in] str Original C-Style string */
			void setString( const _TChar* str );

			//Assignment operator
			void operator = ( const String& rhs ) 
			{ setString( rhs ); }
			void operator = ( const _TChar* rhs ) 
			{ setString( rhs ); }
			//void operator = ( const int value ) { setString( value ); }
			//void operator = ( const long value ) { setString( value ); }
			//void operator = ( const UINT value ) { setString( value ); }
			//void operator = ( const ULONG value ) { setString( value ); }
			//void operator = ( const FLOAT value ) { setString( value ); }
			//void operator = ( const LFLOAT value ) { setString( value ); }

			
			/** Reserve sets the number of characters to allocate for the string
			\param[in] count The minimum required string capacity (this should include the null terminator)
			\param[in] preserve Whether the current string is preserved and copied into the new memory 
			if the capacity has to be expanded
			\remarks If count is lower or equal to the current capacity no change will occur*/
			void reserve( Size_T count, k_Bool preserve = true );
			/** Add extra storage at the end of the allocated string buffer */
			void grow( Size_T increaseBy );
			/** Shrink the internal array to the length of the string */
			void compact();		

			/** Get the character at a position in the string */
			_TChar& operator [] ( Index_T index );
			/** Get the character at a position in the string */
			const _TChar& operator [] ( Index_T index ) const;

			/** Find the index for a character in the string searching forwards
			\param[in] char2find Character to match
			\param[in] startIndex_T Index_T of the first character to compare
			\return Index_T of the first character found or Index_MAX if no character was found */
			Index_T findFromStart( _TChar char2find, Index_T startIndex_T = 0 ) const;
			/** Find the index for a character in the string searching backwards
			\param[in] char2find Character to match
			\param[in] startIndex_T Index_T of the first character to compare 
			\return Index_T of the first character found or Index_MAX if no character was found */
			Index_T findFromEnd( _TChar char2find, Index_T startIndex_T = Index_MAX ) const;
			
			/** Find the index for a sub-string searching forwards
			\params[in] str2find String to match
			\params[in] startIndex_T Index_T of the first character to compare
			\return Index_T of the first character of the sub-string found or Index_MAX if no match was found */
			Index_T findFromStart( const String& str2find, Index_T startIndex_T = 0 ) const;
			/** Find the index for a sub-string searching backwards
			\param[in] str2find String to match
			\param[in] startIndex_T Index_T of the first character to compare 
			\return Index_T of the first character of the sub-string found or Index_MAX if no match was found */
			Index_T findFromEnd( const String& str2find, Index_T startIndex_T = Index_MAX ) const;

			/** Gets a sub-string inclusive of the start & end characters
			\param[out] ostr String to fill with the sub string
			\param[in] start Start index of the sub string
			\param[in] end End index of the sub string */
			void subString( String& ostr, Index_T start = 0, Index_T end = Index_MAX ) const; 
			/** Gets a sub-string inclusive of the start & end characters
			\param[in] start Start index of the sub string
			\param[in] end End index of the sub string */
			String subString( Index_T start = 0, Index_T end = Index_MAX ) const;
			/** Gets a sub-string inclusive of the start & end characters into a C-Style buffer
			including the null terminator at the end of the string or and of the buffer if it over-runs
			\param[out] ostr String to fill with the sub string
			\param[in] ostrSize_T Length of the output string buffer
			\param[in] start Start index of the sub string
			\param[in] end End index of the sub string 
			\return The number of characters copied to the target buffer */
			Size_T subString( _TChar* ostr, Size_T ostrSize_T, Index_T start = 0, Index_T end = Index_MAX ) const; 

			/** Returns the start/left of the string 
			\param[out] ostr The string containg the start of the string
			\param[in] index Index_T of last character to extract  */
			void left( String& ostr, Index_T index ) const;
			/** Returns the start/left of the string 
			\param[in] index Index_T of last character to extract
			\return The string containg the start of the string */
			String left( Index_T index ) const;
			/** Returns the end/right of the string 
			\param[out] ostr The string containg the start of the string
			\param[in] index Index_T of first character to extract  */
			void right( String& ostr, Index_T index ) const;
			/** Returns the end/right of the string 
			\param[in] index Index_T of first character to extract
			\return The string containg the start of the string */
			String right( Index_T index ) const;

			/** Replace all occurances of a character with another
			\param[in] char2find Character to match
			\param[in] replacement Character to replace with 
			\return Number of characters replaced */
			Size_T replace( _TChar char2find, _TChar replacement );
			/** Replace all occurances of a string with another
			\param[in] str2find The string to match
			\param[in] replacement String to replace with 
			\return Number of string instances replaced */
			Size_T replace( const String& str2find, const String& replacement );

			/** Splits the string
			\param[out] strs Reference to a string array which will b filled with the string sections
			\param[in] splitChar The character to split the string at
			\return Number of strings found */
			Size_T split( typename String::Array& strs, _TChar splitChar ) const;

			/** Trims all white space characters from the start of the string 
			\param[in] char2find Specifies specific character to check for. If \0 is used all white space 
			characters including tabs and new lines will be deleted */
			void trimLeft( _TChar char2find = '\0' );
			/** Trims all white space characters from the end of the string
			\param[in] char2find Specifies specific character to check for. If \0 is used all white space 
			characters including tabs and new lines will be deleted */
			void trimRight( _TChar char2find = '\0' );
			/** Trims all white space characters from the start & end of the string 
			\param[in] char2find Specifies specific character to check for. If \0 is used all white space 
			characters including tabs and new lines will be deleted */
			void trim( _TChar char2find = '\0');

			/** Compare two strings & return values like strcmp() starting from the left 
			\param[in] str String to compare against 
			\param[in] caseSensitive Whether the compare will be case sensitive or not
			\param[in] start Start index of the sub string to compare with
			\param[in] end End index of the sub string to compare with
			\return A zero value indicates that both strings are equal.
			\return A value greater than zero indicates that the first character that does not match has
			a greater value in str1 than in str2; And a value less than zero indicates the opposite.*/
			k_Int8 compare( const String& str, bool caseSensitive = false/*, Index_T start = 0, Index_T end = Index_MAX*/ ) const;	
			/** Compare two strings & return values like strcmp() starting from the left 
			\param[in] str C-Style string to compare against 
			\param[in] caseSensitive Whether the compare will be case sensitive or 
			\param[in] start Start index of the sub string to compare with
			\param[in] end End index of the sub string to compare with 
			\return A zero value indicates that both strings are equal.
			\return A value greater than zero indicates that the first character that does not match has
			a greater value in str1 than in str2; And a value less than zero indicates the opposite.*/
			k_Int8 compare( const _TChar* str, bool caseSensitive = false/*, Index_T start = 0, Index_T end = Index_MAX*/ ) const;

			/** Append another string to the end
			\param[in] str The string to append to the end */
			void append( const String& str );	
			/** Append a C-Style string to the end
			\param[in] str The null terminated C-Style string */
			void append( const _TChar* str );	
			/** Append a character to the end
			\param[in] chr The character */
			void append( _TChar str );	

			/** Concatenates a string onto the end of this string
			\param[in] str String to concatenate with  */
			inline void operator += ( const String& str ) 
			{ append( str ); }
			inline String& operator << ( const String& str )
			{ append( str ); return *this; }
			/** Concatenates a string onto the end of this string 
			\param[in] str Pointer to the c-stle string to concatenate with */
			inline void operator += ( const _TChar* str ) 
			{ append( str ); }
			inline String& operator << ( const _TChar* str )
			{ append( str ); return *this; }
			/** Concatenates a character onto the end of this string 
			\param[in] chr Character to concatenate with */
			inline void operator += ( const _TChar str ) 
			{ append( str ); }
			inline String& operator << ( const _TChar str )
			{ append( str ); return *this; }

			/** Concatenates two strings to create a new temporary string */
			String operator + ( const String& str2 ) const;
			/** Concatenates two strings to create a new temporary string */
			friend String operator + (const _TChar* str1, const String& str2 );
			/** Concatenates two strings to create a new temporary string */
			String operator + ( const _TChar* str2 ) const;


			/** Not operator to check if the string is valid */
			inline k_Bool operator !() { return isNull(); }

			/** Equality operator
			\param[in] str (Right hand side) Reference to the string to compare */
			inline k_Bool operator == ( const String& str ) const { return compare( str ) == 0; }
			/** non-Equality operator
			\param[in] str (Right hand side) Reference to the string to compare */
			inline k_Bool operator != ( const String& str ) const { return compare( str ) != 0; }
			/** Less-than operator
			\param[in] str (Right hand side) Reference to the string to compare */
			inline k_Bool operator < ( const String& str ) const { return compare( str ) < 0; }
			/** Greater than operator
			\param[in] str (Right hand side) Reference to the string to compare */
			inline k_Bool operator > ( const String& str ) const { return compare( str ) > 0; }
			/** Equality operator
			\param[in] (Right hand side) C-Style string pointer  */
			inline k_Bool operator == ( const _TChar* str ) const { return compare( str ) == 0; }
			/** Non-Equality operator
			\param[in] (Right hand side) C-Style string pointer  */
			inline k_Bool operator != ( const _TChar* str ) const { return compare( str ) != 0; }
			/** Less-than operator
			\param[in] (Right hand side) C-Style string pointer  */
			inline k_Bool operator < ( const _TChar* str ) const { return compare( str ) < 0; }
			/** Greater than operator
			\param[in] (Right hand side) C-Style string pointer */
			inline k_Bool operator > ( const _TChar* str ) const { return compare( str ) > 0; }

			/** Conversion operator to String8 */
			String< k_Char8 > asString8() const;
			/** Conversion operator to String16 */
			String< k_Char16 > asString16() const;
			/** Conversion operator to StringT (Current compiler character type) */
			//StringT asStringT() const;

			/** \brief orphans the internal string representation and DOES NOT delete the
			string from memory
			\remarks This function will cause a memory leak if not managed
			\remarks use in conjunction with c_str() to ge the original string representation so
			memory may be freed later **/
			void orphan() 
			{
				m_text = 0;
				m_length = m_capacity = 0;
			}

		private:

			/** \brief Returns the terminating character for the string **/
			_TChar terminator() const;

		private:			
			_TChar* m_text; ///< Pointer to the start of the string	
			Size_T m_length; ///< The length of the text in the buffer in characters (excluding the null terminator)
			Size_T m_capacity; ///< The length of the string data array
		};					


		//Typedef the string types
		typedef String< k_Char > StringT;
		typedef String< k_Char8 > String8;
		typedef String< k_Char16 > String16;



		
			//Conversion functions
			//void to( CString& out ) const { out.setString( m_text ); }
			//void to( int& out ) const { out = atoi( m_text ); }
			//void to( long& out ) const { out = atol( m_text ); }
			//void to( BYTE& out ) const { out = (BYTE)atoi( m_text ); }
			//void to( UINT& out ) const { TCHAR* pEnd; out = (UINT)strtoul( m_text, &pEnd, 10 ); }
			//void to( ULONG& out ) const { TCHAR* pEnd; out = (ULONG)strtoul( m_text, &pEnd, 10 ); }
			//void to( FLOAT& out ) const { out = (FLOAT)atof( m_text ); }
			//void to( LFLOAT& out ) const { out = (LFLOAT)atof( m_text );}
			//void to( bool& out ) const { out = atoi( m_text ) != 0;}

	} //END: frx

} //END: kismo

#endif