/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\KismoMayaPlugin\Source\CCXMLFileExporter.h
 * Status: unique (no other copy of this file name in the archive)
 */
#include <maya/MFileIO.h>
#include <maya/MFileObject.h>
#include <maya/MObject.h>
#include <maya/MStatus.h>
#include <maya/MString.h>
#include <maya/MFnDagNode.h>

#include <maya/MFnCamera.h>

//Include the maya file transltor base class
#include <maya/MPxFileTranslator.h>

#include "CExportMesh.h"

#define VERSION "1.00"
const char* const defaultArgument =
	"expMeshes=1;"
	"expMaterials=1;"
	"expHeirachy=1;"
	"expCameras=0;"
	"expLights=0;"
	"externalMesh=0;"
	"meshNormals=1;"
	"meshTexCoords=1;"
	"useFullImagePath=0;"
	"copyImages=0;"
	"imageFolder=;"
	"imageFormat=0;"
	"overWriteImage=0";
			

#include <fstream>
#include <iostream>

using std::cerr;
using std::cout;
using std::fstream;

#include "CString.h"

#include "CExportMaterial.h"
#include "CExportMesh.h"
#include "CExportCamera.h"
#include "CExportLight.h"

//The plugin class derives from the file translator base class
class CCXMLFileExporter : public MPxFileTranslator
{
public:
  	CCXMLFileExporter() : 
		MPxFileTranslator()
	{};
	~CCXMLFileExporter() {};

	//Function which performs the exporting
	MStatus writer( const MFileObject& file, const MString& optionsString, FileAccessMode mode);
	
	//returns true to tell Maya that this plugin can export files
	bool haveWriteMethod() const { return true; }

	//Function which performs Importing
	MStatus reader( const MFileObject& file, const MString& optionsString, FileAccessMode mode);
	//returns true to tell Maya that this plugin can import files!
	bool haveReadMethod () const { return true; }

	//returns the default extension of the files which are supported
	MString defaultExtension() const { return "csf"; }

	// Used by Maya to create a new instance of our custom file translator
	static void* creator() { return new CCXMLFileExporter; }

private:
	//iterates through a node exporting required information & progressing down the tree to the branches
	void exportDagNode( MDagPath& dagPath, fstream& outFile );

	void exportCamera( MDagPath& dagPath, fstream& outFile );
	void exportMesh( MDagPath& dagPath, fstream& outFile );
	void exportLight( MDagPath& dagPath, fstream& outFile );
	void exportMaterials( fstream& outFile );

	//The na
	CString sceneName;

	//Exporters
	CExportMaterial materialExporter;
	CExportMesh meshExporter;
	CExportCamera cameraExporter;
	CExportLight lightExporter;


	//Options;
	bool bExpMeshes;
	bool bExpCameras;
	bool bExpLights;
	bool bExpHierarchy;
	bool bExpMaterials;

	//Mesh options
	bool bOptimiseMesh;
	bool bExternalMesh;
	bool bMeshNormals;
	bool bMeshTexCoords;



};