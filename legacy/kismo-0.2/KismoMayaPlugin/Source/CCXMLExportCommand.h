/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\KismoMayaPlugin\Source\CCXMLExportCommand.h
 * Status: unique (no other copy of this file name in the archive)
 */
#include <maya/MFileIO.h>
#include <maya/MFileObject.h>
#include <maya/MObject.h>
#include <maya/MStatus.h>
#include <maya/MString.h>
#include <maya/MFnDagNode.h>

#include <maya/MFnCamera.h>

//Include the maya command base class
#include <maya/MPxCommand.h>

#include "CExportMesh.h"

#define VERSION "1.00"

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
class CCXMLExportCommand : public MPxCommand
{
public:
  	CCXMLExportCommand() : 
		MPxCommand()
	{};
	~CCXMLExportCommand() {};

	//actually does the exporting
	virtual MStatus doIt( const MArgList& args );

	//gets a new instance of the command
	static void* creator() 
	{
		return new CCXMLExportCommand;
	}

	//Returns the syntax for the command
	static MSyntax getSyntax();

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
	bool bExphierarchy;
	bool bExpMaterials;

	//Mesh options
	bool bExternalMesh;
	bool bMeshNormals;
	bool bMeshTexCoords;

	//material options
	bool bFullPaths;



};