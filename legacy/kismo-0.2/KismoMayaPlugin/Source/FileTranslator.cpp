/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\KismoMayaPlugin\Source\FileTranslator.cpp
 * Status: superseded by kismo-v0.3/Kismo_MayaPlugin/FileTranslator.cpp (same name, content differs)
 */
#include "FileTranslator.h"

#include <maya/MDagPath.h>
#include <maya/MGlobal.h>
#include <maya/MSelectionList.h>
#include <maya/MItSelectionList.h>
#include "Options.h"
#include "FileWriter.h"

FileTranslator::FileTranslator() : 
	MPxFileTranslator()
{

}

FileTranslator::~FileTranslator()
{

}

//returns true to tell Maya that this plugin can export files
bool FileTranslator::haveWriteMethod() const { return true; }
//Function which performs the exporting
MStatus FileTranslator::writer( const MFileObject& file, const MString& optionsString, FileAccessMode mode )
{
	MStatus status;

	const MString filename = file.fullName();
	//ofstream newFile(fileName.asChar(), ios::out);
	//ofstream os("output.flt", ios::out | ios::binary); 
	//ifstream is("output.flt", ios::in | ios::binary | ios::nocreate );
	
	Options options( optionsString );

	//set the default options
	if ( !options.isPropertySet( kc_Syntax_FileType ) )
		options.setProperty( kc_Syntax_FileType );

	MGlobal::displayInfo("Export formated as '" + options.getProperty( kc_Syntax_FileType ) + "'\n" );



	//The selection list before export
	MSelectionList selectedList;
    
	//The selection list of objects to export
	MSelectionList exportList;

	//Get a list of the currently selected objects
	MGlobal::getActiveSelectionList( selectedList );

	bool exportSelection;
	options.getProperty( kc_Syntax_Selection, exportSelection );
	if( mode == kExportActiveAccessMode || exportSelection )//export only the selected objects
	{	
		exportList = selectedList;
	}
	else
	{
		MGlobal::executeCommand( "select -all -hierarchy;" );

		//Get a list of all the root objects in the sceen
		MGlobal::getActiveSelectionList( exportList );
		
	}

	MItSelectionList listIterator( exportList );

	FileWriter* writer = options.writer();
	if ( !isValidPointer( writer ) )
	{
  		status.perror( "No writer was found for the specified file format!" );
		return status;
	}

	while ( !listIterator.isDone() )
	{
		MDagPath dagPath;
		listIterator.getDagPath( dagPath );
		writer->addNode( dagPath );
		listIterator.next();
	}
	writer->write( filename );

	return MS::kSuccess;
}

//tells Maya whether this plugin can import files
bool FileTranslator::haveReadMethod () const { return false; }
//Function which performs Importing
MStatus FileTranslator::reader( const MFileObject& file, const MString& optionsString, FileAccessMode mode)
{
	const MString fileName = file.fullName();

	MGlobal::displayInfo("Import from " + fileName + " successful!"); 
	return MS::kSuccess;
}

//identify files that are kismo formatted
FileTranslator::MFileKind FileTranslator::identifyFile ( const MFileObject& file, const char*, short) const
{
	const char *name = file.name().asChar(); 
	int   nameLength = (int)strlen(name); 
	if ((nameLength > 4) && (
			!strcasecmp(name+nameLength-4, ".kaf") ||
			!strcasecmp(name+nameLength-4, ".kbf") ||
			!strcasecmp(name+nameLength-4, ".kml") ||
			!strcasecmp(name+nameLength-4, ".ksf") ) ) 
	{ 
		return (kIsMyFileType); 
	} 
	return (kNotMyFileType);
}