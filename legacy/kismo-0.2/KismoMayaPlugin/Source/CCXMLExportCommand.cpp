/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\KismoMayaPlugin\Source\CCXMLExportCommand.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
#include "CCXMLExportCommand.h"


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
#include <maya/MFileObject.h>
#include <maya/MSyntax.h>
#include <maya/MArgDatabase.h>

//-------------------
//command help string
//-------------------

const char* KhelpText = 
" Crog CXML Export Command :\n"
"// --------------------------\n"
"// -h | -help = Show this help.\n"
"// -f | -filename = the full path of the file to create & export to. e.g. c:\\export\\cxmlfile.csf <required>\n"
"// -t | -type = the type of files to create. Valid options are :\n"
"// \t 'scene' = full scene file allowing for lights, cameras, heirachy etc.. (CSF)\n"
"// \t 'mesh' = only mesh files will be created, meaning only meshes & materials are exported(CMF)\n"
"// \t 'sceneandmesh' = A full scene will be created but meshes are defined in external files name 'scenefile_objName.cmf' (CMF)\n"
"// -msh | -meshes = export meshes [default:true] :\n"
"// -mtrl | -materials = export materials [default:true] :\n"
"// -cam|-cameras = export cameras [default:true] :\n"
"// -l | -lights = export lights [default:true] :\n"
"// -h | -hierarchy = export hierarchy [default:true] :\n"
"\n// Mesh options:\n"
"// -mn | -normals = export mesh normals [default:true]\n"
"// -mt | -texcoords = export mesh texture coordinates [default:true]\n"
;

//-------------------
//Command syntax
//-------------------

const char* KshortHelp = "-h";				const char* KlongHelp = "-help";
const char* KshortExportSelection = "-sl";	const char* KlongExportSelection = "-selection";
const char* KshortFilename = "-f";			const char* KlongFilename = "-filename";
const char* KshortFileType = "-t";			const char* KlongFileType = "-type";
const char* KshortMeshes = "-msh";			const char* KlongMeshes = "-meshes";
const char* KshortMaterials = "-mtrl";		const char* KlongMaterials = "-materials";
const char* KshortCameras = "-cam";			const char* KlongCameras = "-cameras";
const char* KshortLights = "-l";			const char* KlongLights = "-lights";
const char* KshortHierarchy = "-h";			const char* KlongHierarchy = "-hierarchy";
const char* KshortNormals = "-mn";			const char* KlongNormals = "-normals";
const char* KshortTexCoords = "-mt";		const char* KlongTexCoords = "-texcoords";
const char* KshortFullPaths = "-fp";		const char* KlongFullPaths = "-fullpaths";
const char* KshortCopyImages  = "-ci";		const char* KlongCopyImages = "-copyimages";

//-------------------
//Gets a syntax instance defining the paramaters accepted by the function
//-------------------
MSyntax CCXMLExportCommand::getSyntax() 
{
	MSyntax syn;

	//Build the command syntax list
	syn.addFlag( KshortHelp, KlongHelp );
	syn.addFlag( KshortExportSelection, KlongExportSelection ); //Export just the selected?
	syn.addFlag( KshortFilename, KlongFilename, MSyntax::kString ); //The filename
	syn.addFlag( KshortFileType, KlongFileType, MSyntax::kString ); //CSF / CMF /sceneandmesh ?
	syn.addFlag( KshortMeshes, KlongMeshes, MSyntax::kBoolean ); //Export meshes?
	syn.addFlag( KshortMaterials, KlongMaterials, MSyntax::kBoolean ); //Export materials?
	syn.addFlag( KshortCameras, KlongCameras, MSyntax::kBoolean ); //Export Cameras?
	syn.addFlag( KshortLights, KlongLights, MSyntax::kBoolean ); //Export Lights
	syn.addFlag( KshortHierarchy, KlongHierarchy, MSyntax::kBoolean ); //Export the heirachy?	

	//mesh options
	syn.addFlag( KshortNormals, KlongNormals, MSyntax::kBoolean ); //Export the mesh normals?	
	syn.addFlag( KshortTexCoords, KlongTexCoords, MSyntax::kBoolean ); //Export the mesh texture coordinates?	

	//material options
	syn.addFlag( KshortFullPaths, KlongFullPaths, MSyntax::kBoolean ); //Include the full image path?
	syn.addFlag( KshortCopyImages, KlongCopyImages, MSyntax::kBoolean ); //Include the full image path?

	//return the syntax
	return syn;
}

//-------------------
// This	function does the exporting
//-------------------
MStatus	CCXMLExportCommand::doIt( const MArgList& args )
{
	cerr << "----------------------------------\nstart of CXMLExporter......\n";

	MStatus status; //error status
	
	//set the options
	MString	fullFilename;
	bool exportSelection = false;
	MString fileType = "csf";
	bExpMeshes = true;
	bExpCameras = true;
	bExpLights = true;
	bExpMaterials = true;
	bExphierarchy = true;
	bExternalMesh = false;
	bMeshNormals = true;
	bMeshTexCoords = true;
	bFullPaths = false;

	//------------------------------
	// parse the argument list
	//------------------------------

	//a parser for the arguments
	MArgDatabase parser( syntax(), args, &status);

	if( status != MS::kSuccess ) // arguments could not be parsed!
	{
		return status;
	}

	//Check for the help flag.
	if( parser.isFlagSet( KlongHelp ) )
	{
 		MGlobal::displayInfo( KhelpText );
		return MS::kSuccess;
	}

	if( parser.isFlagSet( KshortFilename ) )
	{
		parser.getFlagArgument( KshortFilename, 0, fullFilename );
	}
	else
	{
		//Bail out if the filename was not set
		MGlobal::displayError( "Filename must be specified. use '-help' for details." );
		return MS::kFailure;
	}

	//Get the file type
	if( parser.isFlagSet( KshortFileType ) ) 
		parser.getFlagArgument( KshortFileType, 0, fileType );

	if( parser.isFlagSet( KshortExportSelection ) ) 
		parser.getFlagArgument( KshortExportSelection, 0, exportSelection );

	if( parser.isFlagSet( KshortMeshes ) ) 
		parser.getFlagArgument( KshortMeshes, 0, bExpMeshes );

	if( parser.isFlagSet( KshortCameras ) ) 
		parser.getFlagArgument( KshortCameras, 0, bExpCameras );

	if( parser.isFlagSet( KshortLights ) ) 
		parser.getFlagArgument( KshortLights, 0, bExpLights );

	if( parser.isFlagSet( KshortMaterials ) ) 
		parser.getFlagArgument( KshortMaterials, 0, bExpMaterials );

	if( parser.isFlagSet( KshortHierarchy ) ) 
		parser.getFlagArgument( KshortHierarchy, 0, bExphierarchy );

	if( parser.isFlagSet( KshortNormals ) ) 
		parser.getFlagArgument( KshortNormals, 0, bMeshNormals );

	if( parser.isFlagSet( KshortTexCoords ) ) 
		parser.getFlagArgument( KshortTexCoords, 0, bMeshTexCoords );

	if( parser.isFlagSet( KshortFullPaths ) ) 
		parser.getFlagArgument( KshortFullPaths, 0, bFullPaths );  

	cerr<< "fullFilename - " << fullFilename
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

	if( exportSelection )	
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

	MFileObject file;	
	file.setFullName( fullFilename );
	
	// Get the output filename
	CString	filename = file.name().asChar() ;

	filename.getSubString( 0, filename.findFromEnd( '.' ), sceneName ) ;

	//Setup the exporters.
	meshExporter = CExportMesh( bMeshNormals, bMeshTexCoords, bExternalMesh, bExpMaterials, sceneName );
	cameraExporter = CExportCamera();
	materialExporter = CExportMaterial();

	fstream outFile( fullFilename.asChar(), ios::out );

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

	//output the DTD
	outFile << "<!DOCTYPE scene SYSTEM \"crogxml.dtd\">\n";

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

	//If we are exporting everything set the selection back to what it was
	if( !exportSelection ) 
	{
		MGlobal::setActiveSelectionList( selectedList );
	}

	// writing of file was sucessful
	return MS::kSuccess;

	
}

void CCXMLExportCommand::exportDagNode( MDagPath& dagPath, fstream& outFile )
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

void CCXMLExportCommand::exportCamera(  MDagPath& dagPath, fstream& outFile )
{
	MFnCamera fnCamera( dagPath );

	cerr << "camera node : " << fnCamera.fullPathName() << "\n";

	if (fnCamera.isIntermediateObject())
		return;
	
	cameraExporter.export( fnCamera, outFile );
}

void CCXMLExportCommand::exportLight(  MDagPath& dagPath, fstream& outFile )
{
	MFnLight fnLight( dagPath );

	if (fnLight.isIntermediateObject())
		return;

	cerr << "light node : " << fnLight.fullPathName() << "\n";

	
	lightExporter.export( fnLight, outFile );
}


void CCXMLExportCommand::exportMesh( MDagPath& dagPath, fstream& outFile )
{
	MFnMesh fnMesh( dagPath );

	if (fnMesh.isIntermediateObject())
		return;

	
	cerr << "mesh node : " << fnMesh.fullPathName() << "\n";
	
	meshExporter.export( fnMesh, outFile );
}

void CCXMLExportCommand::exportMaterials( fstream& outFile )
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
