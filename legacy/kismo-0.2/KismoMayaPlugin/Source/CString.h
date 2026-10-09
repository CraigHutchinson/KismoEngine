/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\KismoMayaPlugin\Source\CString.h
 * Status: unique (no other copy of this file name in the archive)
 */
//String class - copyright Craig Hutchinson//
#ifndef CSTRING_H
#define	CSTRING_H

#include "types.h"

#include <tchar.h>


//maximum varArgs string length buffer allowed
const int KMaxStringLength = 2048;

#include <vector>
using std::vector;

//defines a string
class CString
{
public:
	CString() :
	  m_text( 0 ),
	  m_length( 0 )
	 {}
	
	CString( ULONG length, TCHAR fillChar = '\0' );//Constructor takes a length of which to initialise the text array to	
	CString( const CString& str ) : m_text( 0 ) { setString( str ); }//Constructs a copy of another string instance	
	CString( const TCHAR* str ) : m_text( 0 ) { setString( str ); }//Constructor takes a normal C style character array

	//Creates a string with a variable argument array
	static CString varArgs(const TCHAR* str, ...);

	//destructor frees the memory used to store this string
	virtual ~CString();	
	
	//For all other types they must have a CString cast to be converted to a string
	template < class T > void setString( const T& in) { *this = (CString)in; }

	//Sets the string like the sprintf function with variable arguments
	void setStringVarArgs( const TCHAR* text, ...);	
	
	TCHAR* getString() const { return m_text; }//returns a pointer to the internal string
	operator TCHAR *() const { return m_text; } //Returns the address of the contained string.

	//Returns the length of the string
	const ULONG getLength() const { return m_length; }

	//returns whether the string is holding any data
	bool isNull() const;


	//Split/divide mehtods
	void getSubString( ULONG start, ULONG end, CString& dest ) const; //Gets a string segment & puts it into the destination string
	COUNT split( CString*& arrOut, COUNT& numToGet, TCHAR splitCharacter = ' ' ) const; //Split into an array

	//trim & resize methods
	void lTrim();//removes all white spaces (tabs & spaces) from the start of the string
	void rTrim();//removes all white spaces (tabs & spaces) from the end of the string
	
	//Search methods
	int findFromStart( TCHAR char2find, ULONG skip = 0 ) const;//Finds the position of a character in the string
	int findFromEnd( TCHAR char2find, ULONG skip = 0 ) const;//Finds the position of a character in the string

	//Assignment operator
	void operator = ( const CString& value ) { setString( value ); }
	void operator = ( const TCHAR* value ) { setString( value ); }
	void operator = ( const int value ) { setString( value ); }
	void operator = ( const long value ) { setString( value ); }
	void operator = ( const UINT value ) { setString( value ); }
	void operator = ( const ULONG value ) { setString( value ); }
	void operator = ( const FLOAT value ) { setString( value ); }
	void operator = ( const LFLOAT value ) { setString( value ); }

	//Assignment functions
	void setString( const CString& str );//Sets the string to another string class	
	void setString( const TCHAR* str );//sets the string to that of a charatcer string
	void setString( const int in ) { TCHAR str[15]; itoa( in, str, 10 ); setString( str ); }
	void setString( const long in ) { TCHAR str[15]; ltoa( in, str, 10 ); setString( str ); }
	void setString( const UINT in ) { TCHAR str[15]; _ultoa( (ULONG)in, str, 10 ); setString( str ); }
	void setString( const ULONG in ) { TCHAR str[15]; _ultoa( in, str, 10 ); setString( str ); }
	void setString( const FLOAT in ) { TCHAR str[20]; gcvt( (double)in, 15, str ); setString( str ); }
	void setString( const LFLOAT in ) { TCHAR str[25]; gcvt( in, 20, str ); setString( str ); }

	//cast operators to scalar values
	//operator int () const { int tmp; to( tmp ); return tmp; }//Cast the string to a scalar value
	//operator long () const { long tmp; to( tmp ); return tmp; }//Cast the string to a scalar value
	//operator UINT () const { UINT tmp; to( tmp ); return tmp; }//Cast the string to a scalar value
	//operator ULONG () const { ULONG tmp; to( tmp ); return tmp; }//Cast the string to a scalar value
	//operator FLOAT () const { FLOAT tmp; to( tmp ); return tmp; }//Cast the string to a scalar value
	//operator LFLOAT () const { LFLOAT tmp; to( tmp ); return tmp; }//Cast the string to a scalar value

	//Conversion functions
	void to( CString& out ) const { out.setString( m_text ); }
	void to( int& out ) const { out = atoi( m_text ); }
	void to( long& out ) const { out = atol( m_text ); }
	void to( BYTE& out ) const { out = (BYTE)atoi( m_text ); }
	void to( UINT& out ) const { TCHAR* pEnd; out = (UINT)strtoul( m_text, &pEnd, 10 ); }
	void to( ULONG& out ) const { TCHAR* pEnd; out = (ULONG)strtoul( m_text, &pEnd, 10 ); }
	void to( FLOAT& out ) const { out = (FLOAT)atof( m_text ); }
	void to( LFLOAT& out ) const { out = (LFLOAT)atof( m_text );}
	void to( bool& out ) const { out = atoi( m_text ) != 0;}
	
	template< class T >
		void to( T& out ) const { out = *this; }

	//concatenate two strings together
	friend CString operator +(const CString& str1, const CString& str2);

	//adds a string onto the end of the current one (Concatenates)
	void operator +=( const CString& str );

	//Access function
	TCHAR& operator []( ULONG index ); //Get the character at a position in the string

	//comparison functions
	int compare( const CString& str, bool caseSensitive = false) const;	//Compare two string & return values like strcmp()
	int compare( const TCHAR* str, bool caseSensitive = false) const;
	bool right( const TCHAR* str ) const;//compares check the tight/end of the string to see if it matches the input string

	//boolean operators
	bool operator !() { return isNull(); }

	bool operator ==(const CString& str) const { return (bool)( compare( str ) == 0); }
	bool operator !=(const CString& str) const { return (bool)( compare( str ) != 0); }
	
	bool operator <(const CString& str) const { return (bool)( compare( str ) < 0); }
	bool operator >(const CString& str) const { return (bool)( compare( str ) > 0); }

	bool operator ==(const TCHAR* str) const { return (bool)( compare( str ) == 0); }
	bool operator !=(const TCHAR* str) const { return (bool)( compare( str ) != 0); }
	
	bool operator <(const TCHAR* str) const { return (bool)( compare( str ) < 0); }
	bool operator >(const TCHAR* str) const { return (bool)( compare( str ) > 0); }

	//split to arrays
	template <class T>
		COUNT toArray( T*& arrOut, COUNT numToGet, TCHAR splitCharacter = ' ' ) const; //to an array

	template <class T>
		COUNT toSTLVector( vector< T >& arrOut, COUNT numToGet = 0, TCHAR splitCharacter = ' ' ) const; //to an std::vector

	//###################
	// STATIC FUNCTIONS
	//###################

	//merge into arrays
	template <class T>
		static CString fromArray( const T* arrIn, COUNT numToCreate, TCHAR splitCharacter = ' '  );
	

protected:
	//increase the length of the string array
	void grow( ULONG increaseBy );


protected:
	//The text stored in the class
	TCHAR* m_text;

	//the length of the string
	ULONG m_length;
};

template <class T>
COUNT CString::toArray( T*& arrOut, COUNT numToGet, TCHAR splitCharacter ) const
{
	//we start with 1 word
	COUNT wordCount = 1;
	
	//count the number of items in the string
	for( COUNT i = 0; i < m_length; ++i)
	{
		if( m_text[ i ] == splitCharacter) 
		{
			wordCount++;
		}
	}

	//check that there are enough 'words' to meet the target asked for
	if (wordCount < numToGet)
	{
		numToGet = wordCount;        
	}

	//If an unspecified number was given then we create the space for all the values
	if (numToGet == 0 || arrOut == 0)
	{
        arrOut = new T[ numToGet ];
	}

	//(*count) = wordCount;
	
	//the start index for the current word
	COUNT wordStart = 0;

	//reset the word counter
	INDEX currentWord = 0;

	TCHAR buff[ KMaxStringLength ];

	//loop through the entire string
	for( COUNT i = 0; i <= m_length; ++i)
	{	
		//check if we hav met a split character
		if( m_text[ i ] == splitCharacter || m_length == i ) 
		{
			if (wordStart == i) //If 2splits next to each other set a zero sting
				arrOut[ currentWord ] = 0;
			else
			{
				buff[ i - wordStart ] = '\0';

				//convert the string into the output value
				CString( buff ).to( arrOut[ currentWord ] );
			}

			wordStart = i + 1;

			currentWord++;

			
			if ( m_length == i //if at the end of the string then break out
				|| currentWord > numToGet  )
				break;

		}
		else
		{
			//Store the charcter into the buffer
			buff[ i - wordStart ] = m_text[ i ];
		}
	}

	return numToGet;

}

template <class T>
COUNT CString::toSTLVector( vector< T >& arrOut, COUNT numToGet, TCHAR splitCharacter ) const
{
	//we start with 1 word
	COUNT wordCount = 1;
	
	//count the number of items in the string
	for( COUNT i = 0; i < m_length; ++i)
	{
		if( m_text[ i ] == splitCharacter) 
		{
			wordCount++;
		}
	}

	//Reserve space in the std::vector for all the values
	arrOut.resize( wordCount );
	
	//the start index for the current word
	INDEX wordStart = 0;
	INDEX currentWord = 0; 

	TCHAR buff[ KMaxStringLength ];

	//loop through the entire string
	for( COUNT i = 0; i <= m_length; ++i)
	{	
		//check if we hav met a split character
		if( m_text[ i ] == splitCharacter || m_length == i ) 
		{
			if (wordStart == i) //If 2splits next to each other set a zero sting
				arrOut[ currentWord ] = 0;
			else
			{
				buff[ i - wordStart ] = '\0';
				arrOut[ currentWord ] = (T)(atof( buff ));
			}

			wordStart = i + 1;

			currentWord++;

			//break if we have reached the required number
			if (numToGet > 0 && currentWord > numToGet)
				break;

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

	//return the size of the new vector array
	return (COUNT)arrOut.size();
}

template <class T>
CString CString::fromArray( const T* arrIn, COUNT numToCreate, TCHAR splitCharacter )
{
	CString out;
	CString tStr = " ";
	for ( COUNT i = 0; i < numToCreate; ++i)
	{
		tStr.setString( *(arrIn + i) );
		out += tStr + " ";
	}

	return out;
}


#endif

