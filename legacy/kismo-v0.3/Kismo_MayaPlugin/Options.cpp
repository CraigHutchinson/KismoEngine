/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\Kismo_MayaPlugin\Options.cpp
 * Status: current (newest copy of this file name in the archive)
 */
#include "Options.h"

#include <maya/MStringArray.h>
#include "FileWriter.h"
#include "FileReader.h"


Map< String, FileFormat > g_fileFormats;




Options::Options( const MString& optionsString, 
		Char propertyEndMarker,
		Char nameValueSeperator  ) :
	m_writer( 0 ), m_reader( 0 ),
	m_nameValueSeperator( nameValueSeperator ),
	m_propertyEndMarker( propertyEndMarker )
{
	MStringArray optionList;

    //Split the option string into its parts
    optionsString.split( m_propertyEndMarker, optionList);

	if ( optionList.length() % 2 )
	{
		cerr << "Option string has an odd number of properties!, please check the option string format.\n";
	}

	//Construct the property map
	for ( Count i =0; i < optionList.length(); ++i )
	{   
		MStringArray option;
		optionList[ i ].split( m_nameValueSeperator, option );
		if ( option.length() == 2 )
		{
			setProperty( option[ 0 ], &option[ 1 ] );
			cout << option[ 0 ] << " = " << option[ 1 ] << "\n";	
		}
		else
		if ( option.length() > 2 )
		{
			setProperty( option[ 0 ], &option[ 1 ] );
			cerr << "Too many values given for " << option[0] << ". Using first value only\n";
			cout << option[ 0 ] << " = " << option[ 1 ] << "\n";
		}
		else
		if ( option.length() == 1 )
		{
			setProperty( option[ 0 ], 0 );
			cerr << "No value given for " << option[0] << ". Assigning default.\n";
		}
		else
		{
			cerr << "Empty paramater found!!\n"; //a freeky occurance!
			assert( false );
		}
	}
}

Options::~Options()
{
	frx::safeDelete( m_writer );
	frx::safeDelete( m_reader );
}

//sets a paramaters value from the option string. Passing 0 for value sets the param to default.
void Options::setProperty( const MString& param,  const MString* value )
{
	for ( int i = 0; i < Syntax_COUNT_; ++i )
	{
		if (	_stricmp( syntax[ i ].paramsName[0], param.asChar() ) == 0 ||
				_stricmp( syntax[ i ].paramsName[0], param.asChar() ) == 0  )
		{
			setProperty( (eSyntax)i, value );
		}
	}
}

void Options::setProperty( eSyntax param,  const MString* value )
{
	if ( value == 0 )
	{
		m_properties[ param ] = MString( syntax[ param ].defaultValue ).toLowerCase() ;
	}
	else
	{
		//Trim the white space
		MString tmp = *value;
		UInt32 left = 0;
		while ( left < tmp.numChars() && tmp.asChar()[ left ] == ' ' )
			++left;
		
		UInt32 right = tmp.numChars()-1;
		while ( right >= 0 && tmp.asChar()[ right ] == ' ' )
			--right;

		tmp = tmp.substring( left, right );

		m_properties[ param ] = tmp.toLowerCase();
	}

	switch ( param )
	{
	case Syntax_FileType:
		setFileType( m_properties[ param ] );
	};

}

bool Options::setFileType( const MString& typeName )
{
	Map< String, FileFormat >::iterator i = g_fileFormats.find( typeName.asChar() );
	if ( i == g_fileFormats.end() )
		return false;
	
	frx::safeDelete( m_writer ); //delete any exisating reader/writer
	frx::safeDelete( m_reader );

	if ( frx::isValidPointer( i->second.newWriter ) )
		m_writer = i->second.newWriter();
	if ( frx::isValidPointer( i->second.newReader ) )
		m_reader = i->second.newReader();

	return true;
}

bool Options::isPropertySet( eSyntax param ) const
{
	return ( m_properties.find( param ) != m_properties.end() );
}


const MString& Options::getProperty( eSyntax param ) const
{
	Map< eSyntax, MString >::const_iterator i = m_properties.find( param );

	if ( i != m_properties.end() )
		return i->second;
	else
	{
		return m_notSetValue;
	}
}

const MString& Options::getProperty( eSyntax param, Int32& out ) const
{
	out = getProperty( param ).asInt();
	return getProperty( param );
}

const MString& Options::getProperty( eSyntax param, UInt32& out ) const
{
	out = getProperty( param ).asUnsigned();
	return getProperty( param );
}

const MString& Options::getProperty( eSyntax param, Float32& out ) const
{
	out = getProperty( param ).asFloat();
	return getProperty( param );
}

const MString& Options::getProperty( eSyntax param, Float64& out ) const
{
	out = getProperty( param ).asDouble();
	return getProperty( param );
}

const MString& Options::getProperty( eSyntax param, Bool& out ) const
{
	const MString& tmp = getProperty( param );
	out = ( tmp == "yes" || tmp == "true" || "tmp" == "1" );
	return getProperty( param );
}
