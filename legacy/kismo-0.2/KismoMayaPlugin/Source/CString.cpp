/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\KismoMayaPlugin\Source\CString.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
#include "CString.h"

#include <strsafe.h>
#include <stdlib.h>

CString::CString( ULONG length, TCHAR fillChar  ) 
{
	//check if the length is valid
	if ( length > 0 )
	{
		//set all values to zero
		m_text = 0;
		m_length = 0;
		return;
	}

	m_text = new TCHAR[ length ];
	memset( m_text, fillChar, length * sizeof( TCHAR ) );
	m_length = length;

}

CString CString::varArgs( const TCHAR* str, ... )
{
	//check if the string is valid
	if ( !str )
	{
		return CString();
	}

	//Variable to hold the passed variable list
	va_list list;	
	va_start( list, str);

	//Character array to store the output string
	TCHAR buff[ KMaxStringLength ]; 

	//Create the output string from the argument list & check if it was successfull or not
	if ( SUCCEEDED( StringCbVPrintf( buff, KMaxStringLength, str, list) ) )
	{
		return CString( buff);
	}
	else
	{
		return CString();
	}
}


//destructor frees the memory used to store this string
CString::~CString()
{
	if ( m_text )
		delete [] m_text;
}


//returns whether the string is holding any data
bool CString::isNull() const
{
	return ( !m_text || !_tcslen( m_text ) );
}

//Compare two string & return values liek strcmp()
int CString::compare( const CString& str, bool caseSensitive ) const
{
	if ( isNull() && !str.isNull() ) //if the second string is null
		return 1; //This string is greater ( 1 )
	else 

	if ( !isNull() && str.isNull() ) //If the first string is null
		return -1; //this string is less ( -1 )
	else 

	if ( isNull() && str.isNull() )
		return 0; //this string is equal ( 0 )
	
	//perform the string comparison
	if ( caseSensitive )
		return _tcscmp( m_text, str.m_text );
	else
		return _tcsicmp( m_text, str.m_text );
}

int CString::compare( const TCHAR* str, bool caseSensitive ) const
{
	if ( !isNull() && (str==0) ) //if the second string is null
		return -1; //This string is less ( -1 )
	else 

	if ( isNull() && (str != 0)  ) //If the first string is null
		return 1; //this string is greater ( 1 )
	else 

	if ( isNull() && (str == 0) )
		return 0; //this string is equal ( 0 )
	
	//perform the string comparison
	if ( caseSensitive )
		return _tcscmp( m_text, str );
	else
		return _tcsicmp( m_text, str );
}


//Gets a string segment & puts it into the destination string
void CString::getSubString( ULONG start, ULONG end, CString& dest ) const
{
	ULONG count = end - start;

	//check if the parameters is valid
	if ( start < 0 || start > m_length || count <= 0 || end > m_length )
	{
		dest = _T("");
		return;
	}

	//create storage for the news tring
	TCHAR* temp = new TCHAR[ count + 1 ];

	//copy the sub string
	_tcsncpy( temp, &m_text[ start ], count );

	//terminate the sub string
	temp[ count ] = _T( '\0' );

	//check if a destination string has been set

	//put the string into the destination string
	dest = temp;
	
	//delete the temporary array
	delete [] temp;

}

//removes all white spaces (tabs & spaces) from the start of the string
void CString::lTrim()
{
	if ( isNull() )
		return;

	for ( ULONG i = 0; i < m_length ; i++ )
	{
		if ( (m_text[i] != _T( ' ' )) && (m_text[i] != _T( '\t' )) )
			break;
	}
//	Delete(0, i);
}

//removes all white spaces (tabs & spaces) from the end of the string
void CString::rTrim()
{
	if ( isNull() )
		return;

	for ( ULONG i = m_length - 1 ; i >= 0 ; i-- )
	{
		if ( m_text[i] != _T( ' ' ) && m_text[i] != _T( '\t' ) )
			break;
	}

	//Delete(i+1, Len-i-1);
}

int CString::findFromStart( TCHAR char2find, ULONG skip ) const
{
	for ( ULONG i = skip; i < m_length; ++i)
	{
		if (m_text[i] == char2find)
			return i;
	}

	return -1;
}

int CString::findFromEnd( TCHAR char2find, ULONG skip ) const
{
	for ( int i = m_length - skip; i >= 0 ; --i)
	{
		if (m_text[i] == char2find)
			return i;
	}

	return -1;
}

//adds a string onto the end of the current one (Concatenates)
void CString::operator +=( const CString& str )
{
	if ( str.m_length > 0)
	{
		int len = m_length;

		//create storage for the new string
		grow( str.m_length );

		_tcsncpy( m_text + len, str.m_text, str.m_length + 1 );
		
	}
}

TCHAR& CString::operator []( ULONG index )	// Returns a character reference at a
{	
	//If the index is too low simply return the 1st character
	if ( index < 0 )
	{
		return m_text[ 0 ];
	}
	//if the index is past the end of the string then grow the string
	else if ( index > m_length)
	{
		grow( (m_length - index) + 2 );
		return m_text[ index ];
	}
	else											// Otherwise, just return character
		return m_text[ index ];
}



void CString::grow( ULONG increaseBy )
{

	//increase the length
	m_length += increaseBy;

	//create memory space of the new size
	TCHAR* temp = new TCHAR[ m_length + 1 ];

	if ( m_text )
	{
		//copy the text into the new memory space
		StringCbCopy( temp, m_length + 1, m_text);

		//delete the original memory
		delete [] m_text;
	}

	//store the new pointer
	m_text = temp;

}

void CString::setStringVarArgs( const TCHAR* text, ...)
{
	//check if the string is valid
	if ( !text )
	{
		//set all values to zero
		m_text = 0;
		m_length = 0;
		return;
	}

	if (m_text)
	{
		delete [] m_text;
		m_text = 0;
	}

	//Character array to store the output string
	TCHAR buff[ KMaxStringLength ]; 

	//Variable to hold the passed variable list
	va_list list;
	va_start( list, text);

	//Create the output string from the argument list & check if it was successfull or not
	if ( SUCCEEDED( StringCbVPrintf( buff, KMaxStringLength, text, list) ) )
	{
		//get the length of the new string in the buffer
		m_length = (ULONG)_tcslen( buff );	

		//create storage for the news string
		m_text = new TCHAR[ m_length + 1 ];

		if ( m_length > 0 )
			//Copy the buffer into the string
			StringCbCopy( m_text, m_length + 1, buff);
		else
			//Null terminate the string
			m_text[0] = _T( '\0' );
	}
	else
	{
		m_text = 0;
	}
}

void CString::setString( const CString& str)
{

	if (m_text)
	{
		delete [] m_text;
		m_text = 0;
	}

	//store the length
	m_length = str.m_length;
	
	//check if the length is greater than 1
	if ( m_length )
	{
		//create storage for the string
		m_text = new TCHAR[ m_length + 1 ];

		//copy the string into this instance
		StringCbCopy( m_text, m_length + 1, str.m_text );

	}
	else
	{
		//set the string pointer to null
		m_text = 0;
	}
}

void CString::setString( const TCHAR* str )
{
	if (m_text)
	{
		delete [] m_text;
		m_text = 0;
	}

	//check if the pointer is valid
	if ( !str )
	{
		//set all values to zero
		m_text = 0;
		m_length = 0;
		return;
	}

	//get the length of the input string
	m_length = (ULONG)_tcslen( str );

	if ( m_length > 0 )
	{
		//create storage for the string
		m_text = new TCHAR[ m_length + 1 ];

		//copy the string into the array
		StringCbCopy( m_text, (m_length + 1) * sizeof(TCHAR), str);
	}
}

CString operator +(const CString& str1, const CString& str2)
{
	CString t( str1 );
	t += str2;
	return t;
}


bool CString::right( const TCHAR* str ) const
{
	if ( !str )
		return false;

	//get the length of the string
	ULONG len = (ULONG)_tcslen( str );

	//check if the search string isn't longer than this string
	if (len > m_length)
		return false;

	//start at the end of the string & work backwards
	for (ULONG i = m_length -1; i >= m_length - len; --i)
	{
		if ( m_text[ i ] != str[ i - (m_length - len) ] )
			return false;
	}

	return true;

}

//disection functions
COUNT CString::split( CString*& arrOut, COUNT& count, TCHAR splitCharacter ) const
{
	//we start with 1 word
	ULONG wordCount = 1;
	
	//count the number of items in the string
	for( ULONG i = 0; i < m_length; ++i)
	{
		if( m_text[ i ] == splitCharacter) 
		{
			wordCount++;
		}
	}

	count = wordCount;

	arrOut = new CString[ count ];
	
	//the start index for the current word
	ULONG wordStart = 0;

	//reset the word counter
	wordCount = 0;

	TCHAR buff[ KMaxStringLength ];

	//loop through the entire string
	for(ULONG i = 0; i <= m_length; ++i)
	{	
		//check if we hav met a split character
		if( m_text[ i ] == splitCharacter || m_length == i ) 
		{
			if (wordStart == i) //If 2splits next to each other set a zero sting
				arrOut[ wordCount ].setString( "" );
			else
			{
				buff[ i - wordStart ] = '\0';
				arrOut[ wordCount ].setString( buff );
			}

			wordStart = i + 1;

			wordCount++;

			//if at the end of the string then break out
			if(m_length == i)
				break;
		}
		else
		{
			//Store the charcter into the buffer
			buff[ i - wordStart ] = m_text[ i ];
		}
	}

	return count;

}

