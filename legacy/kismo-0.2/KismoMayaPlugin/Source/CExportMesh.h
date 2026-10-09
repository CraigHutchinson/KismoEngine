/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\KismoMayaPlugin\Source\CExportMesh.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef CMESHEXPORT_H
#define	CMESHEXPORT_H

#include <maya/MFnMesh.h>
#include <maya/MFloatPointArray.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MFloatArray.h>
#include <maya/MStringArray.h>
#include <maya/MDagPath.h>

#include "CString.h"

#include <fstream>
#include <iostream>

using std::cerr;
using std::cout;
using std::fstream;

#define VERSION "1.00"
#define MESH_TAG "mesh"
#define VERTEXARRAY_TAG "vertexarray"
#define INDICES_TAG "indicearray"

//For displaying a progress window
#include <maya/MProgressWindow.h>

class CExportMesh
{
public:
	CExportMesh( bool normals = true, bool texCoords = true, bool externalFile = false, bool materials = true, const CString& fileTitle = CString() ) :
		bNormals( normals ),
		bTexCoords( texCoords ),
		bExternalFile( externalFile ),
		bMaterials( materials ),
		parentFilename( fileTitle )
	{};
	
	//public export call
	bool export( MFnMesh& fnMesh, fstream& file );

private:
	//Calls the export functions required to export the mesh
	bool doExport( MFnMesh& fnMesh, fstream& outFile  );

	//Extracts & stores mesh vertice, normals & texture UVs
	bool extractMeshData( MFnMesh& fnMesh );

	//From object indices this function obtains indices within the current polygon
	MIntArray getLocalIndex( MIntArray& getVertices, MIntArray& getTriangle );

	MProgressWindow progressBar;

	//Filename for the export exluding file extension
	CString parentFilename;

	//error status
	MStatus status;

	//the dag path for the mesh
	MDagPath dagForMesh;

	//Array to store all the vertex positions
	MFloatPointArray positions;
	
	//Array to store all the vertex normals
	MFloatVectorArray normals;

	//Texture sets for multi texturing (not suported as of yet)
	MStringArray  UVSets;

	//The texture coordinates
	MFloatArray   u, v;

	//Options;
	bool bExternalFile;
	bool bNormals;
	bool bTexCoords;
	bool bMaterials;


};

#endif