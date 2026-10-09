/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\KismoCore\KismoFramework\String.h
 * Status: duplicate of kismo-core/_SOURCE_/KismoFramework/String.h
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

#ifndef KISMOFRAMEWORK_STRING_H
#define KISMOFRAMEWORK_STRING_H

#include "KismoFramework/Types.h"
#include "KismoFramework/Vector.h"


/**
TODO: Add string hash and hash-table implementation
http://en.literateprograms.org/Hash_function_comparison_(C,_sh)
http://www.partow.net/programming/hashfunctions/#top

unsigned int SDBMHash(const std::string& str)
{
   unsigned int hash = 0;

   for(std::size_t i = 0; i < str.length(); i++)
   {
      hash = str[i] + (hash << 6) + (hash << 16) - hash;
   }

   return hash;
}
*/

#if 1


#include <string>
namespace kismo
{
	//Typedef the string types
	typedef std::string String8;
	typedef std::wstring String16;
	typedef std::basic_string<Char> String;
		
	Count measure( const Char8* str );

	Count measure( const Char16* str);

	Count measure( const String8& str );

	Count measure( const String16& str);

	Char8 terminator( const Char8* str );

	Char16 terminator( const Char16* str);

	template< bool _CaseSensitive >
	Int32 compare( const String8& lhs, const String8& rhs );

	template< bool _CaseSensitive >
	Int32 compare( const String16& lhs, const String16& rhs );

	template< bool _CaseSensitive >
	Int32 compare( const Char8* lhs, const Char8* rhs );

	template< bool _CaseSensitive >
	Int32 compare( const Char16* lhs, const Char16* rhs );

	/** Convert a String8>String16, String16>String8 or from String8/16>String8/16 via direct reference
		for performance in template scenarios
	*/
	template< class _TStringFrom, class _TStringTo >
	struct ConvertString
	{
		ConvertString( const _TStringFrom& src );

		/** Returns the resulting converted string
			\remarks If conversion failed for any reason the string will be null/empty
		*/
		const String& str() const;
	};

} //END: kismo

#include "String.inl"
#else

namespace kismo
{
	/** 
		\brief StringT wrapper class
	*/
	template < class _TChar >
	class StringT : public Vector<_TChar, true, Size16 >
	{
	public:			
		typedef std::vector< StringT > Array;

		static const Index16 npos = Index16_Max; ///< STL Compliant end terminator/invalid index
	public:
		static StringT number( UInt32 num );
		
		//typedef std::basic_ostream< _TChar > _TOutStream;
		//typedef std::basic_istream< _TChar > _TInStream;

		/** \brief Returns the terminating character for the string **/
		static const _TChar& terminator();

		static Count measure( const _TChar* str );
	public:

		/** \brief Default constructor to create an empty string 
		*/
		StringT();		

		/** \brief Constructor creates an empty string allocating a chunk of memory for future use
		\param[in] length The number fo characters to reserve 
		*/
		explicit StringT( Size16 capacity );

		/** \brief Constructor creates a string filled with the specified character 
		\param[in] length The length of the new  string 
		\param[in] fillChar Character to fill the string with 
		*/
		explicit StringT( Size16 length, _TChar fillChar );	

		/** \brief Copy constructor
		\param[in] str The string that holds the original string 
		*/
		StringT( const StringT& str );	

		/** \brief Construct the string from a C style string
		\param[in] str C Stye string array 
		*/
		StringT( const _TChar* str );

		
		/** \brief Construct the string from a C style string of known length
			\param[in] str C Stye string array 
			\param[in] length	Known length of stirng excluding the null temrinator
		*/
		StringT( const _TChar* str, Count length );

		/** \brief destructor frees the memory used to store this string 
		*/
		 ~StringT();		

		/** \brief Return a pointer to the internal string representation 
		*/
		const _TChar* c_str() const;

		/** \brief Whether the string is holding any data 
		*/
		bool isNull() const;			

		/** \brief Get the length of the string in characters 
			\note excludes the null terminator
		*/
		Size16 length() const { return size() ? size()-1 : 0; }
		//Size16 size() const { return length(); }

		/** \brief Get the capacity of the string buffer in characters 
		*/
		//Size16 capacity() const;
		
		/** \brief Set the string to that of another StringT
		\param[in] str original string 
		*/
		void assign( const StringT& str );

		/** \brief Set the string to that of another C-Style StringT
		\param[in] str Original C-Style string 
		*/
		void assign( const _TChar* str );
		
		/**
			\brief	Set the string to that of a C-Style string of known length
			\param[in]	str		Original C-Style string 
			\param[in]	length	length of the string excluding null terminator
		*/
		void assign( const _TChar* str, Count length );

		//Assignment operator
		void operator = ( const StringT& rhs ) 
		{ assign( rhs ); }
		void operator = ( const _TChar* rhs ) 
		{ assign( rhs ); }
		
		/** \brief Add extra storage at the end of the allocated string buffer 
		*/
		void grow( Size16 increaseBy );

		/** \brief Shrink the internal array to the length of the string 
		*/
		void compact();		

		///** \brief Get the character at a position in the string 
		//*/
		//_TChar& operator [] ( Index16 index );

		///** \brief Get the character at a position in the string 
		//*/
		//const _TChar& operator [] ( Index16 index ) const;

		/** \brief		Find the index for a character in the string searching forwards
			\param[in]	char2find		Character to match
			\param[in]	startIndex_T	Index16 of the first character to compare
			\returns	Index16 of the first character found or Index16_Max if no character was found 
		*/
		Index16 findFromStart( _TChar char2find, Index16 startIndex_T = 0 ) const;

		/** \brief Find the index for a character in the string searching backwards
		\param[in] char2find Character to match
		\param[in] startIndex_T Index16 of the first character to compare 
		\return 
			Index16 of the first character found or Index16_Max if no character was found
		*/
		Index16 findFromEnd( _TChar char2find, Index16 startIndex_T = Index16_Max ) const;

		
		/** \brief Find the index for a sub-string searching forwards
		\param[in] str2find StringT to match
		\param[in] startIndex_T Index16 of the first character to compare
		\return 
			Index16 of the first character of the sub-string found or Index16_Max if no match was found 
		*/
		Index16 findFromStart( const StringT& str2find, Index16 startIndex_T = 0 ) const;

		/** \brief Find the index for a sub-string searching backwards
		\param[in] str2find StringT to match
		\param[in] startIndex_T Index16 of the first character to compare 
		\return Index16 of the first character of the sub-string found or Index16_Max if no match was found 
		*/
		Index16 findFromEnd( const StringT& str2find, Index16 startIndex_T = Index16_Max ) const;

		/** \brief		Gets a sub-string inclusive of the start & end characters
			\param[out]	ostr	StringT to fill with the sub string
			\param[in]	start	Start index of the sub string
			\param[in]	end		End index of the sub string 
		*/
		void subString( StringT& ostr, Index16 start = 0, Index16 end = Index16_Max ) const; 

		/** \brief Gets a sub-string inclusive of the start & end characters
			\param[in] start Start index of the sub string
			\param[in] end End index of the sub string 
		*/
		StringT subString( Index16 start = 0, Index16 end = Index16_Max ) const;

		/** \brief		Gets a sub-string inclusive of the start & end characters into a C-Style buffer
						including the null terminator at the end of the string or and of the buffer if it over-runs
			\param[out] ostr		StringT to fill with the sub string
			\param[in]	ostrSize_T	Length of the output string buffer
			\param[in]	start		Start index of the sub string
			\param[in]	end			End index of the sub string 
			\return		The number of characters copied to the target buffer 
		*/
		Size16 subString( _TChar* ostr, Size16 ostrSize_T, Index16 start = 0, Index16 end = Index16_Max ) const; 

		/** \brief		Returns the start/left of the string 
			\param[out]	ostr	The string containg the start of the string
			\param[in]	index	Index16 of last character to extract  
		*/
		void left( StringT& ostr, Index16 index ) const;

		/** \brief		Returns the start/left of the string 
			\param[in]	index	Index16 of last character to extract
			\return		The string containg the start of the string 
		*/
		StringT left( Index16 index ) const;

		/** \brief Returns the end/right of the string 
		\param[out] ostr The string containg the start of the string
		\param[in] index Index16 of first character to extract  
		*/
		void right( StringT& ostr, Index16 index ) const;

		/** \brief		Returns the end/right of the string 
			\param[in]	index	Index16 of first character to extract
			\returns	The string containg the start of the string 
		*/
		const _TChar* right( Index16 index ) const;

		/** \brief		Replace all occurances of a character with another
			\param[in]	char2find	Character to match
			\param[in]	replacement Character to replace with 
			\return		Number of characters replaced 
		*/
		Size16 replace( _TChar char2find, _TChar replacement );

		/** \brief Replace all occurances of a string with another
		\param[in] str2find The string to match
		\param[in] replacement StringT to replace with 
		\return Number of string instances replaced 
		*/
		Size16 replace( const StringT& str2find, const StringT& replacement );

		/** \brief Splits the string
		\param[out] strs Reference to a string array which will b filled with the string sections
		\param[in] splitChar The character to split the string at
		\return Number of strings found 
		*/
		Size16 split( typename StringT::Array& strs, _TChar splitChar ) const;

		/** \brief Trims all white space characters from the start of the string 
		\param[in] char2find Specifies specific character to check for. If \0 is used all white space 
		characters including tabs and new lines will be deleted 
		*/
		void trimLeft( _TChar char2find = '\0' );

		/** \brief Trims all white space characters from the end of the string
		\param[in] char2find Specifies specific character to check for. If \0 is used all white space 
		characters including tabs and new lines will be deleted 
		*/
		void trimRight( _TChar char2find = '\0' );

		/** \brief Trims all white space characters from the start & end of the string 
		\param[in] char2find Specifies specific character to check for. If \0 is used all white space 
		characters including tabs and new lines will be deleted 
		*/
		void trim( _TChar char2find = '\0');

		/** \brief Compare two strings & return values like strcmp() starting from the left 
		\param[in] str StringT to compare against 
		\param[in] caseSensitive Whether the compare will be case sensitive or not
		\param[in] start Start index of the sub string to compare with
		\param[in] end End index of the sub string to compare with
		\return A zero value indicates that both strings are equal.
		\return A value greater than zero indicates that the first character that does not match has
		a greater value in str1 than in str2; And a value less than zero indicates the opposite.
		*/
		Int8 compare( const StringT& str, bool caseSensitive = false/*, Index16 start = 0, Index16 end = Index16_Max*/ ) const;	

		/** \brief Compare two strings & return values like strcmp() starting from the left 
		\param[in] str C-Style string to compare against 
		\param[in] caseSensitive Whether the compare will be case sensitive or 
		\param[in] start Start index of the sub string to compare with
		\param[in] end End index of the sub string to compare with 
		\return A zero value indicates that both strings are equal.
		\return A value greater than zero indicates that the first character that does not match has
		a greater value in str1 than in str2; And a value less than zero indicates the opposite.
		*/
		Int8 compare( const _TChar* str, bool caseSensitive = false/*, Index16 start = 0, Index16 end = Index16_Max*/ ) const;
		
		/** \brief Append another string to the end
		\param[in] str The string to append to the end 
		*/
		void append( const StringT& str );	

		/** \brief Append a C-Style string to the end
		\param[in] str The null terminated C-Style string 
		*/
		void append( const _TChar* str );
		void append( const _TChar* str, Count strLen );

		/** \brief Append a character to the end
		\param[in] chr The character 
		*/
		void append( _TChar str );	

		/** \brief Concatenates a string onto the end of this string
		\param[in] str StringT to concatenate with  
		*/
		void operator += ( const StringT& str ) 
		{ append( str ); }
		StringT& operator <<= ( const StringT& str )
		{ append( str ); return *this; }

		/** \brief Concatenates a string onto the end of this string 
		\param[in] str Pointer to the c-stle string to concatenate with 
		*/
		void operator += ( const _TChar* str ) 
		{ append( str ); }
		StringT& operator <<= ( const _TChar* str )
		{ append( str ); return *this; }

		/** \brief Concatenates a character onto the end of this string 
		\param[in] chr Character to concatenate with
		*/
		void operator += ( const _TChar str ) 
		{ append( str ); }
		StringT& operator <<= ( const _TChar str )
		{ append( str ); return *this; }

		/** \brief Concatenates two strings to create a new temporary string 
		*/
		StringT operator + ( const StringT& str2 ) const;

		/** \brief Concatenates two strings to create a new temporary string 
		*/
		friend StringT operator + (const _TChar* str1, const StringT& str2 );

		/** \brief Concatenates two strings to create a new temporary string 
		*/
		StringT operator + ( const _TChar* str2 ) const;

		/** \brief Concatenates a string and char to create a new temporary string 
		*/
		StringT operator + ( const _TChar chr2 ) const;


		/** \brief Not operator to check if the string is valid 
		*/
		Bool operator !() 
		{ return isNull(); }

		/** \brief Equality operator
		\param[in] str (Right hand side) Reference to the string to compare 
		*/
		Bool operator == ( const StringT& str ) const 
		{ return compare( str ) == 0; }
		
		/** \brief Equality on a C-Style string to a string
		*/
		friend Bool operator == (const _TChar* str1, const StringT& str2 )
		{ return str2 == str1; }

		/** \brief non-Equality operator
		\param[in] str (Right hand side) Reference to the string to compare 
		*/
		Bool operator != ( const StringT& str ) const 
		{ return compare( str ) != 0; }

		/** \brief non-Equality on a C-Style string to a string
		*/
		friend Bool operator != (const _TChar* str1, const StringT& str2 )
		{ return str2 != str1; }

		/** \brief Less-than operator
		\param[in] str (Right hand side) Reference to the string to compare 
		*/
		Bool operator < ( const StringT& str ) const
		{ return compare( str ) < 0; }

		/** \brief Less-than operator on a C-Style string to a string
		*/
		friend Bool operator < (const _TChar* str1, const StringT& str2 )
		{ return str2 > str1; }

		/** \brief Greater than operator
		\param[in] str (Right hand side) Reference to the string to compare 
		*/
		Bool operator > ( const StringT& str ) const 
		{ return compare( str ) > 0; }

		/** \brief Greater-than operator on a C-Style string to a string
		*/
		friend Bool operator > (const _TChar* str1, const StringT& str2 )
		{ return str2 < str1; }

		/** \brief Equality operator
		\param[in] (Right hand side) C-Style string pointer  
		*/
		Bool operator == ( const _TChar* str ) const 
		{ return compare( str ) == 0; }

		/** \brief Non-Equality operator
		\param[in] (Right hand side) C-Style string pointer  
		*/
		Bool operator != ( const _TChar* str ) const 
		{ return compare( str ) != 0; }

		/** \brief Less-than operator
		\param[in] (Right hand side) C-Style string pointer  
		*/
		Bool operator < ( const _TChar* str ) const 
		{ return compare( str ) < 0; }

		/** \brief Greater than operator
		\param[in] (Right hand side) C-Style string pointer 
		*/
		Bool operator > ( const _TChar* str ) const 
		{ return compare( str ) > 0; }

		/** \brief Conversion operator to String8 
		*/
		StringT< Char8 > asString8() const;

		/** \brief Conversion operator to String16 
		*/
		StringT< Char16 > asString16() const;

		/** \brief Conversion operator to build type String16 or String8 depending on UNICODE preprocessor
		*/
		StringT< Char > asString() const;

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
		//friend _TOutStream& operator << ( _TOutStream& outStream, const StringT& s )
		//{
		//	outStream << s.c_str();
		//	return outStream;
		//}
		//friend _TInStream& operator >> ( _TInStream& inStream, StringT& s )
		//{
		//	std::basic_string< _TChar > tStr; //use the std insert implimentation for now
		//	inStream >> tStr;
		//	s.assign( tStr.c_str() );
		//	return inStream;
		//}

	private:

	};					

	//Typedef the string types
	typedef StringT< Char > String;
	typedef StringT< Char8 > String8;
	typedef StringT< Char16 > String16;

} //END: kismo
#endif

#endif

