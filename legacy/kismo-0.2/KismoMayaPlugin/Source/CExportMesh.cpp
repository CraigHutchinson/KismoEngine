/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\KismoMayaPlugin\Source\CExportMesh.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
#include "CExportMesh.h"

//Dag path
#include <maya/MDagPath.h>

//Polygon iterator to iterate polygons of the mesh
#include <maya/MItMeshPolygon.h>

//Integer array used for indices
#include <maya/MIntArray.h>

//vector array
#include <maya/MPointArray.h>


#include <maya/MFnSet.h>
#include <maya/MPlug.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MPlugArray.h>

#include "CExportTransform.h"
#include <cmath>

bool CExportMesh::export( MFnMesh& fnMesh, fstream& outFile ) 
{	
	//get the dag path of the mesh
	dagForMesh = fnMesh.dagPath();


	//setup the progress bar
	progressBar.reserve();
	progressBar.setInterruptable( true );
	progressBar.setProgressMin( 0 );
	progressBar.setTitle("Exporting meshes");

	progressBar.startProgress();

	if ( !doExport( fnMesh, outFile  ) )
	{
		status.perror("Error occured during CXMLexporter export!");
	}

	//close the progress bar
	progressBar.endProgress();
	
	cerr<< "mesh exported\n";

	return true;
}

bool CExportMesh::doExport( MFnMesh& fnMesh, fstream& toutFile  )
{
	//First extract the mesh vertices etc..
	if ( !extractMeshData( fnMesh ) )
		return false;;

	//file output stream
	fstream* outFile;

	fstream newOutFile;

	MString meshName = MFnDependencyNode( fnMesh.parent(0) ).name();


	if ( bExternalFile )
	{
		CString newFilename( parentFilename + "_" + meshName.asChar() + ".cmf" );

		toutFile << "<" << MESH_TAG << " type=\"" << "external" 
			<< "\" src=\"" << newFilename << "\" version=\"" << VERSION << "\" />\n";
				
		newOutFile.open( newFilename, ios::out ); 
		
		outFile = &newOutFile;
	}
	else
	{	
		outFile = &toutFile;
	}

	(*outFile) << "<" << MESH_TAG << " type=\"" << "static" 
		<< "\" name=\"" << meshName << "\" version=\"" << VERSION << "\" >\n";


	//SETUP VERTEX FORMAT//
	bNormals = (fnMesh.numNormals() > 0) && bNormals;
	bTexCoords = (fnMesh.numUVs() > 0) && bTexCoords;

	//get the number of floats per vertex
	unsigned int vertexSize = 3;

	//Build the vertex type string
	MString vertexType = "XYZ";	
	if (bTexCoords){
		vertexType += "|TEX1"; vertexSize += 2;
	}
	if (bNormals) {
		vertexType += "|NORMAL"; vertexSize += 3;
	}

	MFloatArray vertArray; //float array stores all the vertices
	float* tVertex = new float[ vertexSize ];//Temporary vertex
	int meshVertexCount = 0;

	// Here is the trick, get all the nodes connected to this
    // mesh, whether they are on the DAG or not. This will
    // include all the materials.
    unsigned instanceNumber = dagForMesh.instanceNumber();
    MObjectArray polygonSets;
    MObjectArray polygonComponents;

	//Get the connected sets and members used to determine texturing of different faces
    fnMesh.getConnectedSetsAndMembers( instanceNumber, polygonSets, polygonComponents, true );

	unsigned int mtrlCount = polygonSets.length();
	
	//When materials are set the initialShadingGroup for the whole mesh still remains
	if (mtrlCount > 1 )
		mtrlCount--;

	long subSetOffset = 0;

	// iterate over all the connected sets and look for materials
    for ( unsigned int i = 0; i < mtrlCount; i++ )
    {
		MObject set = polygonSets[i];
		MObject component = polygonComponents[i];

		//MfnSet fnSet( set );
		MFnDependencyNode dnSet( set );

		//get an object for the 'surfaceShader' attribute for the set
		MObject surfaceShaderObj = dnSet.attribute( "surfaceShader" );

		//get the shader plug
		MPlug surfaceShaderPlug( set, surfaceShaderObj );

		MPlugArray srcPlugArray;
		surfaceShaderPlug.connectedTo( srcPlugArray, true, false );

		//if this surface shader is not connected to anything check the next one...
		if ( srcPlugArray.length() == 0 ) 
			continue;

		//Iterate over all the polygons within this material
		MItMeshPolygon polyIter( dagForMesh, component );
		
		MString mtrlName = MFnDependencyNode( srcPlugArray[ 0 ].node() ).name();

		cerr  << mtrlName << " - " << polyIter.count() << " faces\n";

		(*outFile) << "\t<" << INDICES_TAG << " sides=\"" << 3 << "\" base=\"" << 0
			<< "\" material=\"" << mtrlName << "\" >";

		//Setup the progress bar
		int progressMax = polyIter.count() / 2;
		float currentProgress = 0;
		progressBar.setProgressMax( progressMax );

		//loop through all the polygons
		while( !polyIter.isDone() )
		{	
			//get the vertex count for this polygon
			int vertexCount = polyIter.polygonVertexCount();

			//Loop for the number of tiangles that make up this polygon
			for ( int tri = 0; tri <= vertexCount - 3; tri++ )
			{
				//Loop 3 times - for the 3 vertices of a triangle
				for ( int tvert = 0; tvert < 3; tvert++ ) 
				{
					//prosess the progress bar ( start off slower as later vertices require more processing )
					currentProgress += ( 1.0f / ((float)vertexCount * 3.0f));
					float sqrtProgress = sqrt(currentProgress);
					progressBar.setProgress( (int)( sqrtProgress * sqrtProgress ) );

					if ( progressBar.isCancelled() )
					{
						status.perror( "Export cancelled by user" );
						return false;
					}


					//Get a pointer to the temporary vertex
					float* ptr = tVertex;

					//Get the index to the vertex in the polygon
					unsigned int vert = tvert + tri;
					
					//triangles always begin with the 1st polygon vertex ( triangle fan )
					if ( tvert == 0 )
						vert = 0;

					//output the texture uv's
					if ( bTexCoords ) 
					{
    					int uvIndex;
    					if ( polyIter.getUVIndex( vert, uvIndex ) ) 
						{
    						*(ptr++) = u[ uvIndex ];
							*(ptr++) = v[ uvIndex ];
						}
					}

					//output the normals
					if ( bNormals ) 
					{
						MFloatVector p = normals[ polyIter.normalIndex( vert ) ];
						*(ptr++) = p.x;
						*(ptr++) = p.y;
						*(ptr++) = p.z;
					}

					//Output the position index to the vertex
					MFloatPoint p = positions[ polyIter.vertexIndex( vert ) ].cartesianize();
					*(ptr++) = p.x;
					*(ptr++) = p.y;
					*(ptr++) = p.z; 

					int vIndex = meshVertexCount;
					int xToProgressRefresh = 1000;

					//check if this vertex doesn't already exist
					while ( vIndex-- > subSetOffset )
					{
						//compare the two vertices
						if ( memcmp( tVertex, &vertArray[ vIndex * vertexSize ], vertexSize * sizeof( float ) ) ==0 )
							break;

						if ( !xToProgressRefresh-- )
						{
							xToProgressRefresh = 1000;
							progressBar.setProgressMax( progressMax );
						}
					}

					//if we went through all the vertices without a match then add the new vertex to the verticesay
					if( vIndex == (subSetOffset-1))
					{
						//store the index to this vertex into the indices for the current face
						//indices.append( meshVertexCount++ );  
						(*outFile) << meshVertexCount++ << " ";
						
						//add the vertex to the array
						for ( unsigned int i =0; i < vertexSize; ++i)
							vertArray.append( tVertex[ i ] );
					}
					else
						//store the index to this vertex into the indices for the current face
						//indices.append( vIndex );
						(*outFile) << vIndex << " ";

				}
			}

			//Increment the subset offset so that we don't try and find matches for vertices
			// out of the material group
			subSetOffset += (meshVertexCount - subSetOffset);

			//Move the iterator to the next polygon
			polyIter.next();
		}

		//End the indices set
		(*outFile) << "\n\t</" << INDICES_TAG << ">\n"; 

	}


	//delete the temporary vertex
	delete [] tVertex;

	(*outFile) << "\t<" << VERTEXARRAY_TAG << " type=\"" << vertexType 
		<< "\" count=\"" << meshVertexCount << "\" >";

	for ( unsigned int i = 0; i < vertArray.length(); ++i)
		(*outFile) << vertArray[ i ] << " ";

	(*outFile) << "\n\t</" << VERTEXARRAY_TAG << ">\n"; 


	//Out put the transform information
	CExportTransform trans;
	trans.export( MFnTransform( fnMesh.parent(0) ), *outFile );

	(*outFile) << "</" << MESH_TAG << ">\n\n";

	//Check we are outputting to an external file
	if (outFile == &newOutFile)
	{
		newOutFile.close();

		toutFile << "</" << MESH_TAG << ">\n\n";

	}

	return true;

}

bool CExportMesh::extractMeshData( MFnMesh& fnMesh )
{
	
	//Get the vertex positions
	if ( (status = fnMesh.getPoints( positions, MSpace::kObject )) != MS::kSuccess )
	{
		status.perror("CXMLexporter::CExportMesh::extractMeshData()::fnMesh.getPoints() Failed! " + status);
		return false;
	}

	//Get the vertex normals
	if ( (status = fnMesh.getNormals( normals, MSpace::kObject )) != MS::kSuccess )
	{
		status.perror("CXMLexporter::CExportMesh::extractMeshData()::fnMesh.getNormals() Failed! " + status);
		return false;
	}
	
	//Get UV sets for this mesh
	if ( (status = fnMesh.getUVSetNames( UVSets )) != MS::kSuccess )
	{
		status.perror("CXMLexporter::CExportMesh::extractMeshData()::fnMesh.getUVSetNames() Failed! " + status);
		return false;
	}

	//Get the texture UVs for the first set only
	if ( (status = fnMesh.getUVs( u, v, &UVSets[0] )) != MS::kSuccess )
	{
		status.perror("CXMLexporter::CExportMesh::extractMeshData()::fnMesh.getUVs() Failed! " + status);
		return false;
	}

	return true;
}

MIntArray CExportMesh::getLocalIndex( MIntArray& getVertices, MIntArray& getTriangle )
{
	MIntArray localIndex;
	unsigned int vertLength = getVertices.length();
	unsigned int triLength = getTriangle.length();

	//assert ( getTriangle.length() == 3 );    // Should always deal with a triangle

	for ( unsigned int tri = 0; tri < triLength; tri++ )
	{
		for ( unsigned int vert = 0; vert < vertLength; vert++ )
		{
			if ( getTriangle[ tri ] == getVertices[ vert ] )
			{
				localIndex.append( vert );
				break;
			}
		}

		// if nothing was added, add default "no match"
		if ( localIndex.length() == tri )
			localIndex.append( -1 );
	}

	return localIndex;
}
