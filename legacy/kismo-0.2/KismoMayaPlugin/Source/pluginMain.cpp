/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\KismoMayaPlugin\Source\pluginMain.cpp
 * Status: superseded by kismo-v0.3/Kismo_MayaPlugin/pluginMain.cpp (same name, content differs)
 */
//Link maya libraries
#ifdef WIN32
	#pragma comment(lib,"Foundation.lib")
	#pragma comment(lib,"OpenMaya.lib")
	#pragma comment(lib,"OpenMayaFx.lib")
	#pragma comment(lib,"OpenMayaUI.lib")
	#pragma comment(lib,"Image.lib")
	#pragma comment(lib,"OpenMayaAnim.lib")
#endif

#include "FileTranslator.h"
#include "Options.h"
#include "framework/kismoString.h"

//---------------Include all translators here -----------------
#include "KismoAscii_FileWriter.h"
#include "KismoBinary_FileWriter.h"
//---------------Include all translators here -----------------
//----Add all translaters and their type names to the map -----
extern void initialiseFileTranslators()
{
	g_fileFormats[ "ascii" ].newWriter = KismoAscii_FileWriter::create;	
	g_fileFormats[ "binary" ].newWriter = KismoBinary_FileWriter::create;	
}
//---------------Include all translators here -----------------

#include <maya/MStatus.h>
#include <maya/MObject.h>
#include <maya/MFnPlugin.h>

using std::cerr;
using std::cout;

// specifies a script for the Export Options UI
const char* const melOptionScript = "KismoExport";



//register the plugin in the maya plugin list
MStatus initializePlugin( MObject obj )
{ 
	//error status
	MStatus   status;

	//create a new plugin instance
	MFnPlugin plugin( obj, "Kismo Engine File Translator", "8.5", "Any");

	//Register the plugin with Maya 
	status = plugin.registerFileTranslator( "KismoFileTranslator", 
		"none",
		FileTranslator::creator,
		(char*)melOptionScript,
		(char*)defaultArgument );

	if (status != MS::kSuccess) 
	{
  		status.perror( "Failed to register \"Kismo Engine File Translator\"\n" );
		return status;
	}
	else
	{
		cout << "Kismo Engine File Translator registered OK\n";
	}
	
	//Initialise the file writer/reader maps and any other information
	initialiseFileTranslators();

	return status;
}

//unregister the plugin from the maya plugin list
MStatus uninitializePlugin( MObject obj )
{
	MStatus   status;
	MFnPlugin plugin( obj );

	//De-register the plugin
	status = plugin.deregisterFileTranslator( "KismoFileTranslator" );

	if (status != MS::kSuccess) 
	{
		status.perror("KismoFileTranslator::deregisterFileTranslator did not execute successfully\n");
		return status;
	}
	else
	{
		cout << "Kismo Engine File Translator un-registered\n";
	}

	return status;
}
