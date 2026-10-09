/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\KismoMayaPlugin\Source\Options.cpp
 * Status: superseded by kismo-v0.3/Kismo_MayaPlugin/Options.cpp (same name, content differs)
 */
#include "Options.h"

#include <maya/MStringArray.h>
#include "FileWriter.h"
#include "FileReader.h"


k_Map< k_String, FileFormat > g_fileFormats;




Options::Options( const MString& optionsString, 
		k_Char propertyEndMarker,
		k_Char nameValueSeperator  ) :
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
	for ( k_Count i =0; i < optionList.length(); ++i )
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
	safeDelete( m_writer );
	safeDelete( m_reader );
}

//sets a paramaters value from the option string. Passing 0 for value sets the param to default.
void Options::setProperty( const MString& param,  const MString* value )
{
	for ( int i = 0; i < kc_Syntax_COUNT_; ++i )
	{
		if (	stricmp( syntax[ i ].paramsName[0], param.asChar() ) == 0 ||
				stricmp( syntax[ i ].paramsName[0], param.asChar() ) == 0  )
		{
			setProperty( (ke_Syntax)i, value );
		}
	}
}

void Options::setProperty( ke_Syntax param,  const MString* value )
{
	if ( value == 0 )
	{
		m_properties[ param ] = MString( syntax[ param ].defaultValue ).toLowerCase() ;
	}
	else
	{
		//Trim the white space
		MString tmp = *value;
		k_UInt32 left = 0;
		while ( left < tmp.numChars() && tmp.asChar()[ left ] == ' ' )
			++left;
		
		k_UInt32 right = tmp.numChars()-1;
		while ( right >= 0 && tmp.asChar()[ right ] == ' ' )
			--right;

		tmp = tmp.substring( left, right );

		m_properties[ param ] = tmp.toLowerCase();
	}

	switch ( param )
	{
	case kc_Syntax_FileType:
		setFileType( m_properties[ param ] );
	};

}

bool Options::setFileType( const MString& typeName )
{
	k_Map< k_String, FileFormat >::iterator i = g_fileFormats.find( typeName.asChar() );
	if ( i == g_fileFormats.end() )
		return false;
	
	safeDelete( m_writer ); //delete any exisating reader/writer
	safeDelete( m_reader );

	if ( isValidPointer( i->second.newWriter ) )
		m_writer = i->second.newWriter();
	if ( isValidPointer( i->second.newReader ) )
		m_reader = i->second.newReader();

	return true;
}

bool Options::isPropertySet( ke_Syntax param ) const
{
	return ( m_properties.find( param ) != m_properties.end() );
}


const MString& Options::getProperty( ke_Syntax param ) const
{
	k_Map< ke_Syntax, MString >::const_iterator i = m_properties.find( param );

	if ( i != m_properties.end() )
		return i->second;
	else
	{
		return m_notSetValue;
	}
}

const MString& Options::getProperty( ke_Syntax param, k_Int32& out ) const
{
	out = getProperty( param ).asInt();
	return getProperty( param );
}

const MString& Options::getProperty( ke_Syntax param, k_UInt32& out ) const
{
	out = getProperty( param ).asUnsigned();
	return getProperty( param );
}

const MString& Options::getProperty( ke_Syntax param, k_Float32& out ) const
{
	out = getProperty( param ).asFloat();
	return getProperty( param );
}

const MString& Options::getProperty( ke_Syntax param, k_Float64& out ) const
{
	out = getProperty( param ).asDouble();
	return getProperty( param );
}

const MString& Options::getProperty( ke_Syntax param, k_Bool& out ) const
{
	const MString& tmp = getProperty( param );
	out = ( tmp == "yes" || tmp == "true" || "tmp" == "1" );
	return getProperty( param );
}
