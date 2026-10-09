/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\KismoMayaPlugin\Source\CCXMLFileExporter.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
#include "CCXMLFileExporter.h"


#include <maya/MGlobal.h>
#include <maya/MArgList.h>
#include <maya/MFileObject.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MItDependencyNodes.h>
#include <maya/MItSelectionList.h>
#include <maya/MItDag.h>
#include <maya/MDagPath.h>
#include <maya/MObject.h>
#include <maya/MObjectArray.h>
#include <maya/MStatus.h>
#include <maya/MString.h>
#include <maya/MStringArray.h>
#include <maya/MFnMesh.h>
#include <maya/MFnLight.h>
#include <maya/MFloatPointArray.h>
#include <maya/MFnTransform.h>



// This	function is	called when	we need	to export any data.
//
// file	- info about the file we should	be writing
// options - the option	string passed through from mel
// mode	- are we exporting all or just the selected	items?
//
MStatus	CCXMLFileExporter::writer(	const MFileObject& file,
								  const MString& options,
								  FileAccessMode mode)
{

	cerr << "----------------------------------\nstart of CXMLExporter......\n";

	//error status
	MStatus status;

	//set the default options
	bExpMeshes = true;
	bExpCameras = false;
	bExpLights = false;
	bExpMaterials = true;
	bExpHierarchy = true;
	bOptimiseMesh = true;
	bExternalMesh = false;
	bMeshNormals = true;
	bMeshTexCoords = true;

	//****parse the option list

	//if there are options...
	if (options.length() > 0) {

        MStringArray optionList;
        MStringArray theOption;

	    //Split the option string into its parts
        options.split(';', optionList);

		//loop through all the options
		int length = optionList.length();
		for( int i = 0; i < length; ++i )
		{
			//Clear the curren toption
			theOption.clear();

			//split the option into its name & the value ( "name=value" )
			optionList[i].split( '=', theOption );

			//'meshes' sets whether to export meshes
			if( theOption[0] == "meshes" && theOption.length() > 1 ) 
				bExpMeshes = (theOption[1].asInt() > 0);

			//'cameras' sets whether to export cameras or not
			else if( theOption[0] == "cameras" && theOption.length() > 1 ) 
				bExpCameras = (theOption[1].asInt() > 0);

			//'lights' sets whether to export light or not
			else if( theOption[0] == "lights" && theOption.length() > 1 ) 
				bExpLights = (theOption[1].asInt() > 0);	

			//'materials' sets whether to export materials or not
			else if( theOption[0] == "materials" && theOption.length() > 1 ) 
				bExpMaterials = (theOption[1].asInt() > 0);	

			//'optimisemesh' sets whether to optimise the mesh for faster load times
			// but slower export & possibly larger files
			else if( theOption[0] == "optimisemesh" && theOption.length() > 1 ) 
				bOptimiseMesh = (theOption[1].asInt() > 0);

			else if( theOption[0] == "externalmesh" && theOption.length() > 1 ) 
				bExternalMesh = (theOption[1].asInt() > 0);

			else if( theOption[0] == "normals" && theOption.length() > 1 ) 
				bMeshNormals = (theOption[1].asInt() > 0);	

			else if( theOption[0] == "texcoords" && theOption.length() > 1 ) 
				bMeshTexCoords = (theOption[1].asInt() > 0);
		}
	}

	cerr << "**Options = " << options
		<< "\nMeshes - " << bExpMeshes 
		<< "\nCamera - " << bExpCameras 
		<< "\nLights - " << bExpLights 
		<< "\nMaterials - " << bExpMaterials
		<< "\n";

	//The selection list before export
	MSelectionList selectedList;
    
	//The selection list of objects to export
	MSelectionList exportList;

	//Get a list of the currently selected objects
	MGlobal::getActiveSelectionList( selectedList );

	if(mode	== kExportActiveAccessMode)	
	{	
		//export only the selected objects
		exportList = selectedList;
	}
	else
	{
		MGlobal::executeCommand( "select -all -hierarchy;" );

		//Get a list of all the root objects in the sceen
		MGlobal::getActiveSelectionList( exportList );
		
	}

	// Get the output filename
	CString	fullFilename = file.fullName().asChar() ;
	CString	filename = file.name().asChar() ;

	filename.getSubString( 0, filename.findFromEnd( '.' ), sceneName ) ;

	//Setup the exporters.
	meshExporter = CExportMesh( bMeshNormals, bMeshTexCoords, bExternalMesh, bExpMaterials, sceneName );
	cameraExporter = CExportCamera();
	materialExporter = CExportMaterial();

	fstream outFile( fullFilename, ios::out );

	if ( !outFile.is_open() )
	{
		status.perror( "Error opening output file!!" );
		cerr << "Cannot open output file!\n";
		return MS::kFailure;
	}
	else
	{
		cerr << "Output file created = " << fullFilename << "\n";
	}

	//write the xml version tot he file
	outFile << "<?xml version=\"1.0\"?>\n";

	//Write the xml route node to the file
	outFile << "<scene version=\"" << VERSION 
		<< "\" format=\"CSF\" up=\"Y\" name=\"" << sceneName << "\" >\n\n";

	if (bExpMaterials)
	{
		exportMaterials( outFile );
	}

	MItSelectionList listIterator( exportList );

	while ( !listIterator.isDone() )
	{
		MDagPath dagPath;
		listIterator.getDagPath( dagPath );

		exportDagNode( dagPath, outFile );

		listIterator.next();
	}

	outFile << "</scene>\n";
	cerr << "Output complete.";
	outFile.close();

	//If we are exporting everything set the selction back to what it was
	if(mode	!= kExportActiveAccessMode) 
	{
		MGlobal::setActiveSelectionList( selectedList );
	}

	// writing of file was sucessful
	return MS::kSuccess;

	
}

//Function performs import of a file
MStatus	CCXMLFileExporter::reader( const MFileObject& file, const MString& options, FileAccessMode mode)
{
	cerr << "Read method as yet unimplimented\n" << std::endl;
	return MS::kFailure;
}

void CCXMLFileExporter::exportDagNode( MDagPath& dagPath, fstream& outFile )
{	
	MStatus status;

	//only checks children of transform nodes
	if ( dagPath.hasFn( MFn::Type::kTransform ) )
	{
		MFnTransform fnTransform( dagPath, &status );

		//get the child count
		unsigned int childCount = dagPath.childCount();

		//loop through all the children
		for ( unsigned int i = 0; i < childCount; ++i)
		{
			//Get the child node
			MFnDagNode node( dagPath.child( i, &status ) );

			MDagPath TdagPath;
			node.getPath( TdagPath );

			//Check if child transforms are to be iterated
			if (TdagPath.hasFn( MFn::Type::kTransform ))
				continue;
			else
				//check if the path is a mesh to export
			if ( TdagPath.hasFn( MFn::Type::kMesh) && bExpMeshes )
				exportMesh( TdagPath, outFile );
			else
				//check if the path is a camera to export
			if ( TdagPath.hasFn( MFn::Type::kCamera ) && bExpCameras )
				exportCamera( TdagPath, outFile );
			else
				//check if the path is a light to export
			if ( TdagPath.hasFn( MFn::Type::kLight ) && bExpLights )
				exportLight( TdagPath, outFile );	
			
		}

		 cerr << "\n";
		
	}
}

void CCXMLFileExporter::exportCamera(  MDagPath& dagPath, fstream& outFile )
{
	MFnCamera fnCamera( dagPath );

	cerr << "camera node : " << fnCamera.fullPathName() << "\n";

	
	cameraExporter.export( fnCamera, outFile );
}

void CCXMLFileExporter::exportLight(  MDagPath& dagPath, fstream& outFile )
{
	MFnLight fnLight( dagPath );

	cerr << "light node : " << fnLight.fullPathName() << "\n";

	
	lightExporter.export( fnLight, outFile );
}


void CCXMLFileExporter::exportMesh( MDagPath& dagPath, fstream& outFile )
{
	MFnMesh fnMesh( dagPath );

	if (fnMesh.isIntermediateObject())
		return;

	cerr << "mesh node : " << fnMesh.fullPathName() << "\n";
	
	meshExporter.export( fnMesh, outFile );
}

void CCXMLFileExporter::exportMaterials( fstream& outFile )
{
	

	//Iterate through all the material nodes (All derive from the Lambert material)
	MItDependencyNodes iterDep( MFn::kLambert );

	//Iterate through all the materials in the iterator
	while ( !iterDep.isDone() )
	{
		materialExporter.export( iterDep.item(), outFile );
		
		iterDep.next();
	}

}
