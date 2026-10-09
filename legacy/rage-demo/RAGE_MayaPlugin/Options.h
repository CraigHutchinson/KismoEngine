/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\RAGE_MayaPlugin\Options.h
 * Status: duplicate of kismo-v0.3/Kismo_MayaPlugin/Options.h
 */
#ifndef _OPTIONS_H
#define _OPTIONS_H

#include <maya/MString.h>
#include "Framework/Common.h"

//forward declarations
class FileWriter;
class FileReader;
typedef FileWriter* (*fn_CreateWriter) (void);
typedef FileReader* (*fn_CreateReader) (void);

//Map of file format names and functions to create instances of their readers/writers
struct FileFormat { fn_CreateWriter newWriter; fn_CreateReader newReader; };
extern Map< String, FileFormat > g_fileFormats;

//-------------------
//Command syntax
//-------------------

enum eSyntax
{
	Syntax_Help,
	Syntax_FileType, 
	Syntax_Filename, 
	Syntax_Selection, 
	Syntax_CopyImages,
	Syntax_Collision,
	Syntax_COUNT_,
};
struct SSyntax { char* paramsName[2]; char* defaultValue; };

//Syntax table
const SSyntax syntax[ Syntax_COUNT_ ] = {
	{ {"help", "h" }, 0 },
	{ {"filetype","ft" }, "ascii" }, //Default filetype = ascii
	{ {"filename","fn" }, 0 },
	{ {"selection","sl" }, "yes" },
	{ {"copyimages","ci" }, "no" },
	{ {"collision","c"  }, "yes" },
};


//Parses the option string
class Options
{
public:

	//Destructor using seperators like param=value;param2=value2; etc...
	Options( const MString& optionsString, 
		Char propertyEndMarker = ';',
		Char nameValueSeperator = '='  );
	virtual ~Options();

	bool isPropertySet( eSyntax param ) const;

	//get the value of a property
	const MString& getProperty( eSyntax param ) const;
	const MString& getProperty( eSyntax param, Int32& out ) const;
	const MString& getProperty( eSyntax param, UInt32& out ) const;
	const MString& getProperty( eSyntax param, Float32& out ) const;
	const MString& getProperty( eSyntax param, Float64& out ) const;
	const MString& getProperty( eSyntax param, Bool& out ) const;

	FileWriter* writer() const { return m_writer; }
	FileReader* reader() const { return m_reader; }

	//sets a paramaters value from the option string. Passing 0 for value sets the param to default.
	void setProperty( const MString& param,  const MString* value = 0 ); 
	void setProperty( eSyntax param,  const MString* value = 0 ); 

protected:

	//Set the file type property
	bool setFileType( const MString& typeName );

private:
	Char m_nameValueSeperator; //the seperator which devides the value from the name
	Char m_propertyEndMarker; //Seperator which defines each section

	Map< eSyntax, MString > m_properties;
	
	FileWriter* m_writer;
	FileReader* m_reader;

	MString m_notSetValue; //Value returned if a paramater is not set
};

#endif