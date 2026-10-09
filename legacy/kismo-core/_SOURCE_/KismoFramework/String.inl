/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\KismoCore\_SOURCE_\KismoFramework\String.inl
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

namespace kismo
{	
	inline Count measure( const Char8* str )
	{ return (Count)strlen(str); }

	inline Count measure( const Char16* str)
	{ return (Count)wcslen(str); }

	inline Count measure( const String8& str )
	{ return (Count)str.length(); }

	inline Count measure( const String16& str)
	{ return (Count)str.length(); }
	
	inline Char8 terminator( const Char8* str )
	{ return '\0'; }

	inline Char16 terminator( const Char16* str)
	{ return L'\0'; }

	template< bool _CaseSensitive >
	inline Int32 compare( const String8& lhs, const String8& rhs )
	{ return compare<_CaseSensitive>( lhs.c_str(), rhs.c_str() ); }	

	template< bool _CaseSensitive >
	inline Int32 compare( const String16& lhs, const String16& rhs )
	{ return compare<_CaseSensitive>( lhs.c_str(), rhs.c_str() ); }	
	
	template<>
	inline Int32 compare<true>( const Char8* lhs, const Char8* rhs )
	{
		return ( lhs == 0)
			? (rhs!=0 ? -1 : 0 ) //Lhs string is less ( -1 ) | lhs string is equal ( 0 )
			: ( !rhs ) //If the rhs string is null
				? 1 //lhs string is greater ( 1 )
				: strcmp( lhs, rhs ); //, Case sentiive compare
	}
		
	template<>
	inline Int32 compare<false>( const Char8* lhs, const Char8* rhs )
	{
		return ( lhs == 0)
			? (rhs!=0 ? -1 : 0 ) //Lhs string is less ( -1 ) | lhs string is equal ( 0 )
			: ( !rhs ) //If the rhs string is null
				? 1 //lhs string is greater ( 1 )
				: _stricmp( lhs, rhs ); //, Case insentiive compare
	}		
		
	template<>
	inline Int32 compare<true>( const Char16* lhs, const Char16* rhs )
	{
		return ( lhs == 0)
			? (rhs!=0 ? -1 : 0 ) //Lhs string is less ( -1 ) | lhs string is equal ( 0 )
			: ( !rhs ) //If the rhs string is null
				? 1 //lhs string is greater ( 1 )
				: wcscmp( lhs, rhs ); //, Case sentiive compare
	}
		
	template<>
	inline Int32 compare<false>( const Char16* lhs, const Char16* rhs )
	{
		return ( lhs == 0)
			? (rhs!=0 ? -1 : 0 ) //Lhs string is less ( -1 ) | lhs string is equal ( 0 )
			: ( !rhs ) //If the rhs string is null
				? 1 //lhs string is greater ( 1 )
				: _wcsicmp( lhs, rhs ); //, Case insentiive compare
	}	

	template< class _TStringFrom >
	struct ConvertString< _TStringFrom, _TStringFrom >
	{
		ConvertString( const _TStringFrom& src ) 
			: m_dest(src) 
		{}	

		const String& str() const
		{ return m_dest; }
	private:
		const _TStringFrom& m_dest;
	};

	template<>
	struct ConvertString< String16, String8 >
	{
		ConvertString( const String16& src )  ;

		const String8& str() const
		{ return m_dest; }
	private:
		String8 m_dest;
	};


	template<>
	struct ConvertString< String8, String16 >
	{
		ConvertString( const String8& src );
		
		const String16& str() const
		{ return m_dest; }

	private:
		String16 m_dest;
	};

} //END: kismo


