/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\Framework\String.h
 * Status: superseded by kismo-core/_SOURCE_/KismoFramework/String.h (same name, content differs)
 */
#ifndef FRX_STRING_H
#define FRX_STRING_H

#include "Platform/Platform.h"
#include "Framework/BaseTypes.h"
#include <string>

namespace frx
{
	/** \brief TString wrapper class */
	template < class _TChar >
	class _DllExport TString
	{
	public:			
		typedef Vector< TString > Array;
		typedef UInt16 Size_T;
		typedef UInt16 Index_T;
		static const Index_T Index_MAX = UInt16_MAX-1; //UInt16_MAX is invalid 
		static const Size_T Size_MAX = UInt16_MAX-2;
		static const Index_T npos = Index_MAX; ///< STL Compliant end terminator/invalid index
	public:
		static TString number( UInt32 num );
		
		typedef std::basic_ostream< _TChar > _TOutStream;
		typedef std::basic_istream< _TChar > _TInStream;
	public:

		/** \brief Default constructor to create an empty string 
		*/
		inline TString();		

		/** \brief Constructor creates an empty string allocating a chunk of memory for future use
		\param[in] length The number fo characters to reserve 
		*/
		inline explicit TString( Size_T capacity );

		/** \brief Constructor creates a string filled with the specified character 
		\param[in] length The length of the new  string 
		\param[in] fillChar Character to fill the string with 
		*/
		inline explicit TString( Size_T length, _TChar fillChar );	

		/** \brief Copy constructor
		\param[in] str The string that holds the original string 
		*/
		inline TString( const TString& str );	

		/** \brief Construct the string from a C style string
		\param[in] str C Stye string array 
		*/
		inline TString( const _TChar* str );

		/** \brief destructor frees the memory used to store this string 
		*/
		inline ~TString();		

		/** \brief Return a pointer to the internal string representation 
		*/
		inline const _TChar* c_str() const;

		/** \brief Whether the string is holding any data 
		*/
		inline bool isNull() const;			

		/** \brief Destroy the interna string representation freeing any used memory
		*/
		inline void destroy();

		/** \brief Get the length of the string in characters (excludes the null terminator)
		*/
		inline Size_T length() const;

		/** \brief Get the capacity of the string buffer in characters 
		*/
		inline Size_T capacity() const;

		/** \brief Returns the maximum string length allowed 
		*/
		inline Size_T maxLength() const;
		
		/** \brief Set the string to that of another TString
		\param[in] str original string 
		*/
		inline void setString( const TString& str );

		/** \brief Set the string to that of another C-Style TString
		\param[in] str Original C-Style string 
		*/
		inline void setString( const _TChar* str );

		//Assignment operator
		inline void operator = ( const TString& rhs ) 
		{ setString( rhs ); }
		inline void operator = ( const _TChar* rhs ) 
		{ setString( rhs ); }
		
		/** \brief Reserve sets the number of characters to allocate for the string
		\param[in] count The minimum required string capacity (this should include the null terminator)
		\param[in] preserve Whether the current string is preserved and copied into the new memory 
		if the capacity has to be expanded
		\remarks 
			If count is lower or equal to the current capacity no change will occur
		*/
		inline void reserve( Size_T count, Bool preserve = true );

		/** \brief Add extra storage at the end of the allocated string buffer 
		*/
		inline void grow( Size_T increaseBy );

		/** \brief Shrink the internal array to the length of the string 
		*/
		inline void compact();		

		/** \brief Get the character at a position in the string 
		*/
		inline _TChar& operator [] ( Index_T index );

		/** \brief Get the character at a position in the string 
		*/
		inline const _TChar& operator [] ( Index_T index ) const;

		/** \brief Find the index for a character in the string searching forwards
		\param[in] char2find Character to match
		\param[in] startIndex_T Index_T of the first character to compare
		\return 
			Index_T of the first character found or Index_MAX if no character was found 
		*/
		inline Index_T findFromStart( _TChar char2find, Index_T startIndex_T = 0 ) const;

		/** \brief Find the index for a character in the string searching backwards
		\param[in] char2find Character to match
		\param[in] startIndex_T Index_T of the first character to compare 
		\return 
			Index_T of the first character found or Index_MAX if no character was found
		*/
		inline Index_T findFromEnd( _TChar char2find, Index_T startIndex_T = Index_MAX ) const;

		
		/** \brief Find the index for a sub-string searching forwards
		\param[in] str2find TString to match
		\param[in] startIndex_T Index_T of the first character to compare
		\return 
			Index_T of the first character of the sub-string found or Index_MAX if no match was found 
		*/
		inline Index_T findFromStart( const TString& str2find, Index_T startIndex_T = 0 ) const;

		/** \brief Find the index for a sub-string searching backwards
		\param[in] str2find TString to match
		\param[in] startIndex_T Index_T of the first character to compare 
		\return Index_T of the first character of the sub-string found or Index_MAX if no match was found 
		*/
		inline Index_T findFromEnd( const TString& str2find, Index_T startIndex_T = Index_MAX ) const;

		/** \brief Gets a sub-string inclusive of the start & end characters
		\param[out] ostr TString to fill with the sub string
		\param[in] start Start index of the sub string
		\param[in] end End index of the sub string 
		*/
		inline void subString( TString& ostr, Index_T start = 0, Index_T end = Index_MAX ) const; 

		/** \brief Gets a sub-string inclusive of the start & end characters
		\param[in] start Start index of the sub string
		\param[in] end End index of the sub string 
		*/
		inline TString subString( Index_T start = 0, Index_T end = Index_MAX ) const;
		inline TString substr( Index_T start = 0, Index_T end = Index_MAX ) const
		{ return subString( start, end ); }

		/** \brief Gets a sub-string inclusive of the start & end characters into a C-Style buffer
		including the null terminator at the end of the string or and of the buffer if it over-runs
		\param[out] ostr TString to fill with the sub string
		\param[in] ostrSize_T Length of the output string buffer
		\param[in] start Start index of the sub string
		\param[in] end End index of the sub string 
		\return The number of characters copied to the target buffer 
		*/
		inline Size_T subString( _TChar* ostr, Size_T ostrSize_T, Index_T start = 0, Index_T end = Index_MAX ) const; 

		/** \brief Returns the start/left of the string 
		\param[out] ostr The string containg the start of the string
		\param[in] index Index_T of last character to extract  
		*/
		inline void left( TString& ostr, Index_T index ) const;

		/** \brief Returns the start/left of the string 
		\param[in] index Index_T of last character to extract
		\return The string containg the start of the string 
		*/
		inline TString left( Index_T index ) const;

		/** \brief Returns the end/right of the string 
		\param[out] ostr The string containg the start of the string
		\param[in] index Index_T of first character to extract  
		*/
		inline void right( TString& ostr, Index_T index ) const;

		/** \brief Returns the end/right of the string 
		\param[in] index Index_T of first character to extract
		\return The string containg the start of the string 
		*/
		inline TString right( Index_T index ) const;

		/** \brief Replace all occurances of a character with another
		\param[in] char2find Character to match
		\param[in] replacement Character to replace with 
		\return Number of characters replaced 
		*/
		inline Size_T replace( _TChar char2find, _TChar replacement );

		/** \brief Replace all occurances of a string with another
		\param[in] str2find The string to match
		\param[in] replacement TString to replace with 
		\return Number of string instances replaced 
		*/
		inline Size_T replace( const TString& str2find, const TString& replacement );

		/** \brief Splits the string
		\param[out] strs Reference to a string array which will b filled with the string sections
		\param[in] splitChar The character to split the string at
		\return Number of strings found 
		*/
		inline Size_T split( typename TString::Array& strs, _TChar splitChar ) const;

		/** \brief Trims all white space characters from the start of the string 
		\param[in] char2find Specifies specific character to check for. If \0 is used all white space 
		characters including tabs and new lines will be deleted 
		*/
		inline void trimLeft( _TChar char2find = '\0' );

		/** \brief Trims all white space characters from the end of the string
		\param[in] char2find Specifies specific character to check for. If \0 is used all white space 
		characters including tabs and new lines will be deleted 
		*/
		inline void trimRight( _TChar char2find = '\0' );

		/** \brief Trims all white space characters from the start & end of the string 
		\param[in] char2find Specifies specific character to check for. If \0 is used all white space 
		characters including tabs and new lines will be deleted 
		*/
		inline void trim( _TChar char2find = '\0');

		/** \brief Compare two strings & return values like strcmp() starting from the left 
		\param[in] str TString to compare against 
		\param[in] caseSensitive Whether the compare will be case sensitive or not
		\param[in] start Start index of the sub string to compare with
		\param[in] end End index of the sub string to compare with
		\return A zero value indicates that both strings are equal.
		\return A value greater than zero indicates that the first character that does not match has
		a greater value in str1 than in str2; And a value less than zero indicates the opposite.
		*/
		inline Int8 compare( const TString& str, bool caseSensitive = false/*, Index_T start = 0, Index_T end = Index_MAX*/ ) const;	

		/** \brief Compare two strings & return values like strcmp() starting from the left 
		\param[in] str C-Style string to compare against 
		\param[in] caseSensitive Whether the compare will be case sensitive or 
		\param[in] start Start index of the sub string to compare with
		\param[in] end End index of the sub string to compare with 
		\return A zero value indicates that both strings are equal.
		\return A value greater than zero indicates that the first character that does not match has
		a greater value in str1 than in str2; And a value less than zero indicates the opposite.
		*/
		inline Int8 compare( const _TChar* str, bool caseSensitive = false/*, Index_T start = 0, Index_T end = Index_MAX*/ ) const;
		
		/** \brief Append another string to the end
		\param[in] str The string to append to the end 
		*/
		inline void append( const TString& str );	

		/** \brief Append a C-Style string to the end
		\param[in] str The null terminated C-Style string 
		*/
		inline void append( const _TChar* str );

		/** \brief Append a character to the end
		\param[in] chr The character 
		*/
		inline void append( _TChar str );	

		/** \brief Concatenates a string onto the end of this string
		\param[in] str TString to concatenate with  
		*/
		inline void operator += ( const TString& str ) 
		{ append( str ); }
		inline TString& operator <<= ( const TString& str )
		{ append( str ); return *this; }

		/** \brief Concatenates a string onto the end of this string 
		\param[in] str Pointer to the c-stle string to concatenate with 
		*/
		inline void operator += ( const _TChar* str ) 
		{ append( str ); }
		inline TString& operator <<= ( const _TChar* str )
		{ append( str ); return *this; }

		/** \brief Concatenates a character onto the end of this string 
		\param[in] chr Character to concatenate with
		*/
		inline void operator += ( const _TChar str ) 
		{ append( str ); }
		inline TString& operator <<= ( const _TChar str )
		{ append( str ); return *this; }

		/** \brief Concatenates two strings to create a new temporary string 
		*/
		inline TString operator + ( const TString& str2 ) const;

		/** \brief Concatenates two strings to create a new temporary string 
		*/
		friend TString operator + (const _TChar* str1, const TString& str2 );

		/** \brief Concatenates two strings to create a new temporary string 
		*/
		inline TString operator + ( const _TChar* str2 ) const;

		/** \brief Concatenates a string and char to create a new temporary string 
		*/
		inline TString operator + ( const _TChar chr2 ) const;


		/** \brief Not operator to check if the string is valid 
		*/
		inline Bool operator !() 
		{ return isNull(); }

		/** \brief Equality operator
		\param[in] str (Right hand side) Reference to the string to compare 
		*/
		inline Bool operator == ( const TString& str ) const 
		{ return compare( str ) == 0; }
		
		/** \brief Equality on a C-Style string to a string
		*/
		friend Bool operator == (const _TChar* str1, const TString& str2 )
		{ return str2 == str1; }

		/** \brief non-Equality operator
		\param[in] str (Right hand side) Reference to the string to compare 
		*/
		inline Bool operator != ( const TString& str ) const 
		{ return compare( str ) != 0; }

		/** \brief non-Equality on a C-Style string to a string
		*/
		friend Bool operator != (const _TChar* str1, const TString& str2 )
		{ return str2 != str1; }

		/** \brief Less-than operator
		\param[in] str (Right hand side) Reference to the string to compare 
		*/
		inline Bool operator < ( const TString& str ) const
		{ return compare( str ) < 0; }

		/** \brief Less-than operator on a C-Style string to a string
		*/
		friend Bool operator < (const _TChar* str1, const TString& str2 )
		{ return str2 > str1; }

		/** \brief Greater than operator
		\param[in] str (Right hand side) Reference to the string to compare 
		*/
		inline Bool operator > ( const TString& str ) const 
		{ return compare( str ) > 0; }

		/** \brief Greater-than operator on a C-Style string to a string
		*/
		friend Bool operator > (const _TChar* str1, const TString& str2 )
		{ return str2 < str1; }

		/** \brief Equality operator
		\param[in] (Right hand side) C-Style string pointer  
		*/
		inline Bool operator == ( const _TChar* str ) const 
		{ return compare( str ) == 0; }

		/** \brief Non-Equality operator
		\param[in] (Right hand side) C-Style string pointer  
		*/
		inline Bool operator != ( const _TChar* str ) const 
		{ return compare( str ) != 0; }

		/** \brief Less-than operator
		\param[in] (Right hand side) C-Style string pointer  
		*/
		inline Bool operator < ( const _TChar* str ) const 
		{ return compare( str ) < 0; }

		/** \brief Greater than operator
		\param[in] (Right hand side) C-Style string pointer 
		*/
		inline Bool operator > ( const _TChar* str ) const 
		{ return compare( str ) > 0; }

		/** \brief Conversion operator to String8 
		*/
		TString< Char8 > asString8() const;

		/** \brief Conversion operator to String16 
		*/
		TString< Char16 > asString16() const;

		/** \brief Conversion operator to StringT (Current compiler character type) */
		//StringT asStringT() const;

		/** \brief Conversion operator to a value type
		*/
		UInt32 asUInt32() const;

		/** \brief Conversion operator to a value type
		*/
		Int32 asInt32() const;

		/** \brief Conversion operator to a value type
		*/
		Bool asBool() const;

		/** \brief Conversion operator to a value type
		*/
		Float32 asFloat32() const;

		/** \brief Conversion operator to a value type
		*/
		Float64 asFloat64() const;

		/** \brief Conversion operator to a value type
		*/
		Real asReal() const;

		/** \brief scans the string into the optional fields
			\remarks sscanf format syntax
		*/
		//bool scan( const _TChar* format, ... ) const;

		//Stream operator
		inline friend _TOutStream& operator << ( _TOutStream& outStream, const TString& s )
		{
			outStream << s.c_str();
			return outStream;
		}
		inline friend _TInStream& operator >> ( _TInStream& inStream, TString& s )
		{
			std::basic_string< _TChar > tStr; //use the std insert implimentation for now
			inStream >> tStr;
			s.setString( tStr.c_str() );
			return inStream;
		}

	private:

		/** \brief Returns the terminating character for the string **/
		const _TChar& terminator() const;

	private:			
		_TChar* m_text; ///< Pointer to the start of the string	
		Size_T m_length; ///< The length of the text in the buffer in characters (excluding the null terminator)
		Size_T m_capacity; ///< The length of the string data array
	};					

	//Typedef the string types
	typedef TString< Char > StringT;
	typedef TString< Char8 > String8;
	typedef TString< Char16 > String16;

} //END: frx

#endif