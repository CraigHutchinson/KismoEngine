/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\RAGE_MayaPlugin\FileTranslator.h
 * Status: superseded by kismo-v0.4/KismoIO/FileTranslator.h (same name, content differs)
 */
#ifndef _FILETRANSLATOR_H
#define _FILETRANSLATOR_H

//Include the maya file transltor base class
#include <maya/MPxFileTranslator.h>

#define VERSION "1.00"
const char* const defaultArgument =
	"FileType=Ascii;";
			
//The plugin class derives from the file translator base class
class FileTranslator : public MPxFileTranslator
{
public:
  	FileTranslator();
	virtual ~FileTranslator();

	//Function which performs the exporting
	virtual MStatus writer( const MFileObject& file, const MString& optionsString, FileAccessMode mode);

	//Function which performs Importing
	virtual MStatus reader( const MFileObject& file, const MString& optionsString, FileAccessMode mode);

	//returns true to tell Maya that this plugin can export files
	virtual bool haveWriteMethod() const;

	//tells Maya whether this plugin can import files
	virtual bool haveReadMethod () const;

	//returns the default extension of the files which are supported
	virtual MString defaultExtension() const { return "rmf"; }

	virtual MString filter() const { return "*.rmf"; }

	//identify files that are RAGE formatted
	MPxFileTranslator::MFileKind identifyFile ( const MFileObject &, const char*, short) const;

	// Used by Maya to create a new instance of our custom file translator
	static void* creator() { return new FileTranslator; }

private:
	
};

#endif