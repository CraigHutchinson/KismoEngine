/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\KismoMayaPlugin\Source\KismoAscii_FileWriter.cpp
 * Status: superseded by kismo-v0.3/Kismo_MayaPlugin/KismoAscii_FileWriter.cpp (same name, content differs)
 */
#include "KismoAscii_FileWriter.h"
#include "framework\KismoDateTime.h"
#include "maya/MFnPhongShader.h"
#include "maya/MFnIkJoint.h"
#include "maya/MFnSkinCluster.h"
#include "maya/MDagPathArray.h"
#include "maya/MItGeometry.h"
#include "maya/MBoundingBox.h"
#include "framework/KismoBoundingSphere.h"
#include "framework/KismoDateTime.h"
#include "framework/KismoString.h"

#define kc_AsciiFileHeader (k_Byte*)"Kismo Ascii Scene Object v1.00"
#define kc_ObjectHierarchyHeader "hierarchy"
#define kc_MeshesHeader "meshes"
#define kc_MaterialsHeader "materials"

#define kc_TransformHeader "transform"
#define kc_LODGroupHeader "lod_group"
#define kc_MeshHeader "mesh"
#	define kc_MeshUVSetHeader "uv_set"
#	define kc_MeshVerticesHeader "vertices"
#	define kc_MeshNormalsHeader "normals"
#	define kc_MeshFaceSetHeader "face_set"
#	define kc_MeshBoundingBoxHeader "bounding_box"
#	define kc_MeshBoundingSphereHeader "bounding_sphere"
#define kc_SkinClusterHeader "skin_cluster"
#define kc_JointHeader "joint"

#define kc_MaterialHeader "material"
#	define kc_MaterialAmbientHeader "ambient"
#	define kc_MaterialDiffuseHeader "diffuse"
#	define kc_MaterialSpecularHeader "specular"
#	define kc_MaterialTransparencyHeader "transparency"

#define kc_StartNodeParam "(\t"
#define kc_EndNodeParam "\t)"
#define kc_OpenLevel /*'\n' << m_indent << */"{" 
#define kc_CloseLevel '\n' << m_indent << '}'
#define kc_StartLevel "\n" << m_indent 

#define START_NODE_HEADER( _nodeName ) \
	m_ofile << kc_StartLevel << _nodeName kc_StartNodeParam

#define END_NODE_HEADER() \
	m_ofile << kc_EndNodeParam

#define WRITE_NODE_HEADER( _nodeName, _param ) \
	START_NODE_HEADER( _nodeName ) _param; END_NODE_HEADER()

#define OPEN_LEVEL() m_ofile << kc_OpenLevel; incrementDepth()

#define CLOSE_LEVEL() decrementDepth(); m_ofile << kc_CloseLevel


KismoAscii_FileWriter::KismoAscii_FileWriter() :
	FileWriter(),
	m_indent( "" ),
	m_currentMeshPriority( 0 ),
	m_priorityStep( 0 )
{
}

KismoAscii_FileWriter::~KismoAscii_FileWriter()
{
}

//Write to the file stream
MStatus KismoAscii_FileWriter::doWrite( const MString& filename )
{
	//error status
	MStatus status;

	m_ofile.open( filename.asChar() );

	if ( !m_ofile.is_open() )
	{
		status.perror( "Failed to open file for writing " + filename );
		return status;
	}
	m_currentMeshPriority = 0;

	//Write the file header 
	m_ofile << kc_AsciiFileHeader << '\n';

	//Write the creation date
	framework::Date date;
	m_ofile << date.day() << '/' <<  date.month() << '/' << date.year() << '\n';

	writeHierarchy();

	//Write the material data
	writeMaterials();

	//Write the mesh data in priority order for async loading purposes
	writeMeshes();

	//writeSkinClusters();

	assert( m_currentMeshPriority == 0 );

	m_ofile.close();
	
	MGlobal::displayInfo("Export to " + filename + " successful!"); 
	return status;
}

void KismoAscii_FileWriter::writeHierarchy()
{
	WRITE_NODE_HEADER( kc_ObjectHierarchyHeader, "" );
	OPEN_LEVEL();

	for ( std::vector< DagNode* >::const_iterator i = m_nodes.begin(); i != m_nodes.end(); ++i )
	{
		if ( !(*i)->parentCount() ) //Only write root nodes...at the root!
			writeNode( (*i ) );
	}

	CLOSE_LEVEL();
}

void KismoAscii_FileWriter::writeMeshes()
{ 	
	WRITE_NODE_HEADER( kc_MeshesHeader, "count\t" << m_meshNamesByPriority.size() );
	OPEN_LEVEL();
	
	for ( MESH_PRIORITY_MAP::const_reverse_iterator i = m_meshNamesByPriority.rbegin(); i != m_meshNamesByPriority.rend(); ++i )
	{
		MESH_NAME_MAP::const_iterator b = m_meshsByName.find( i->second );
		assert( b != m_meshsByName.end() );
		if ( b != m_meshsByName.end() )
			writeMeshNode( b->second, false );
	}

	CLOSE_LEVEL();
}

void KismoAscii_FileWriter::writeMaterials()
{
	//Iterate through all the material nodes (All derive from the Lambert material)
	MItDependencyNodes iterDep( MFn::kLambert );

	unsigned int mtrlCount = 0;
	while ( !iterDep.isDone() )
	{
		iterDep.next();
		++mtrlCount;
	}

	WRITE_NODE_HEADER( kc_MaterialsHeader, "count\t" << mtrlCount );
	OPEN_LEVEL();
	
	iterDep = MItDependencyNodes( MFn::kLambert );
	//Iterate through all the materials in the iterator
	while ( !iterDep.isDone() )
	{
		MFnLambertShader mat( iterDep.item() );
		writeMaterial( mat );

		iterDep.next();
	}

	CLOSE_LEVEL();
}

void KismoAscii_FileWriter::writeNode( DagNode* node )
{
	if ( node->type & kc_NodeType_Joint )
		writeJointNode( node );
	else
	//if ( node->type & kc_NodeType_SkinCluster )
	//	writeSkinCluster( node );
	//else
	if ( node->type & kc_NodeType_LODGroup )
		writeLodGroupNode( node );
	else
	if ( node->type & kc_NodeType_Transform )
		writeTransformNode( node );
	else
	if ( node->type & kc_NodeType_Mesh )
		writeMeshNode( node, true );
	else
		return;

	//Write all the child nodes
	writeChildNodes( node );

	if ( node->type & kc_NodeType_LODGroup )
	{
		assert( m_currentMeshPriority == 0 );
		m_priorityStep = 0;
	}
}

void KismoAscii_FileWriter::writeChildNodes( DagNode* node )
{
	DagNode* child = node->firstChild();
	if ( child )
	{
		OPEN_LEVEL();
		do
		{
			writeNode( child );
		} 
		while( child = node->nextChild() );

		CLOSE_LEVEL();
		
	}
}

void KismoAscii_FileWriter::writeTransformNode( DagNode* node  )
{
	MFnTransform transform = node->path;
	
	WRITE_NODE_HEADER( kc_TransformHeader, 
		"id\t" << MFnDependencyNode( transform ).name();
		writeTransformationData( node );
		m_ofile );

	//for ( k_Count attrIndex = 0; attrIndex < transform.attributeCount(); ++attrIndex )
	//{
	//	MFnAttribute attr = transform.attribute( attrIndex );
	//	if ( !attr.internal() && !attr.type() )
	//	{
	//		m_ofile 
	//			<< attr.name() << '\n';
	//	}
	//}

}

void KismoAscii_FileWriter::writeTransformationData( DagNode* node )
{
	MFnTransform transform = node->path;
	MTransformationMatrix trans = transform.transformation();
		
	//MEulerRotation rot = trans.eulerRotation();	//get the objects rotation
	//MVector scale = trans.eulerRotation();	//get the objects rotation

	MVector pos = trans.translation( MSpace::kTransform );//Get the objects translation/position
	MVector pivot = trans.rotatePivot( MSpace::kTransform );//Get the objects translation/position
	double scale[3];
	trans.getScale( scale, MSpace::kTransform );//Get the objects translation/position

	double rx,ry,rz,rw; //quaternion rotation
	trans.getRotationQuaternion( rx, ry, rz, rw );

	m_ofile 
		<< "\tpos\t" << pos.x << '\t' << pos.y << '\t' << pos.z
		<< "\tquart\t" << rx << '\t' << ry << '\t' << rz << '\t' << rw
		<< "\tscale\t" << scale[0] << '\t' << scale[1] << '\t' << scale[2]
		<< "\tpivot\t" << pivot.x << '\t' << pivot.y << '\t' << pivot.z;
}

void KismoAscii_FileWriter::writeLodGroupNode( DagNode* node )
{
	MFnTransform transform = node->path;
	WRITE_NODE_HEADER( kc_LODGroupHeader, 
		"id\t" << MFnDependencyNode( transform ).name();
	
		//MFnNumericData threshold = transform.attribute( "threshold[0]" );

		// get the MPlug for the compound attribute
		MPlug threshold = transform.findPlug( "threshold" );

		if( threshold.isArray() )
		{
			m_ofile << "\tthresholds\t" <<  threshold.numElements();
			for ( unsigned int i = 0; i < threshold.numElements(); ++i )
			{
				// get the plugs to the child attributes
				MPlug threshN = threshold.elementByPhysicalIndex( i );
				double dist;
				threshN.getValue( dist );

				m_ofile << '\t' << dist;
			}
			
			//As the first mesh is the highest lod level it should be set to a lower priority
			m_currentMeshPriority -= threshold.numElements() + 1;
			m_priorityStep = 1;
		}

		writeTransformationData( node );
	
	m_ofile ); //Finish the header
}

void KismoAscii_FileWriter::writeMeshNode( DagNode* node, bool firstPass )
{		
	MFnMesh mesh = node->path;
	k_String name = MFnDependencyNode( mesh ).name().asChar();

	if ( firstPass )
	{
		m_currentMeshPriority += m_priorityStep;

		WRITE_NODE_HEADER( kc_MeshHeader, "id\t" << name );

		if ( m_meshsByName.find( name ) == m_meshsByName.end() )
		{
			m_meshNamesByPriority.insert( MESH_PRIORITY_MAP::value_type( m_currentMeshPriority, name ) );
			m_meshsByName.insert( MESH_NAME_MAP::value_type( name, node ) );
		}
		
	}
	else
	{
		WRITE_NODE_HEADER( kc_MeshHeader, "id\t" << name );
		OPEN_LEVEL();
		

		//Bounding box
		START_NODE_HEADER( kc_MeshBoundingBoxHeader );

		MBoundingBox boundingBox = mesh.boundingBox();
		MPoint minPoint =  boundingBox.min().cartesianize();
		MPoint maxPoint =  boundingBox.max().cartesianize();
		m_ofile << "min\t" << minPoint.x << '\t' << minPoint.y << '\t' << minPoint.z 
			<< "\tmax\t" << maxPoint.x << '\t' << maxPoint.y << '\t' << maxPoint.z;

		END_NODE_HEADER();

		//Bounding box test####################################
		{
			MFloatPointArray vertexArray;
			mesh.getPoints( vertexArray );
			kismo::framework::Vector3::Array vecArray;
			vecArray.reserve( vertexArray.length() );
			for ( k_Count vertIndex = 0; vertIndex < vertexArray.length(); ++ vertIndex )
			{
				vertexArray[ vertIndex ].cartesianize();
				vecArray.push_back( kismo::framework::Vector3( vertexArray[ vertIndex ].x, vertexArray[ vertIndex ].y, vertexArray[ vertIndex ].z ) );
			}
			kismo::framework::BoundingSphere bs( vecArray );
			START_NODE_HEADER( kc_MeshBoundingSphereHeader );
			m_ofile << "center\t" <<  bs.center().x << '\t' <<  bs.center().y << '\t' <<  bs.center().z 
				<< "\tradius\t" << bs.radius();

			END_NODE_HEADER();
		}
		//Bounding box test####################################


		// UV sets		
		writeUVSets( mesh );	

		// Output vertices	
		writeVertices( mesh );	
		
		// Output normals	
		writeNormals( mesh );

		// Faces
		writeFaces( mesh );
		
		CLOSE_LEVEL();
	}
}

void KismoAscii_FileWriter::writeUVSets( MFnMesh& mesh  )
{			
	m_ofile.precision( 4 ); //set precision
	MStringArray uvSetNames;
	mesh.getUVSetNames( uvSetNames);
	for ( k_Count setindex = 0; setindex < uvSetNames.length(); ++setindex )
	{
		MFloatArray uArray;
		MFloatArray vArray;
		mesh.getUVs( uArray, vArray, &uvSetNames[ setindex ] );
		
		if ( !uArray.length() ) //if there are no coordinates for this map then skip it
			continue;

		unsigned int uvCount = uArray.length();
		WRITE_NODE_HEADER( kc_MeshUVSetHeader, "id\t" << uvSetNames[setindex] 
			<< "\ttype\tfloat2\tcount\t" << uvCount );
		m_ofile << kc_OpenLevel kc_StartLevel;
		for ( k_Count uvIndex = 0; uvIndex < uvCount; ++ uvIndex )
		{
			m_ofile << '\t' << uArray[ uvIndex ] << '\t' << vArray[ uvIndex ];
			if ( uvIndex != uvCount-1 && !((uvIndex+1) % 8))
				m_ofile << '\n' << m_indent;

		}
		m_ofile << kc_CloseLevel;
	}
}
void KismoAscii_FileWriter::writeVertices( MFnMesh& mesh  )
{
	m_ofile.precision( 11 ); //set high precision
	MFloatPointArray vertexArray;
	mesh.getPoints( vertexArray );
	unsigned int vertexCount = vertexArray.length();
	WRITE_NODE_HEADER( kc_MeshVerticesHeader, "type\tfloat3\tcount\t" << vertexCount );
	m_ofile << kc_OpenLevel kc_StartLevel;

	for ( k_Count vertIndex = 0; vertIndex < vertexArray.length(); ++ vertIndex )
	{
		vertexArray[ vertIndex ].cartesianize();
		m_ofile << '\t' << vertexArray[ vertIndex ].x << '\t' << vertexArray[ vertIndex ].y << '\t' << vertexArray[ vertIndex ].z;
		if ( vertIndex != vertexCount - 1 && !((vertIndex+1) % 4))
			m_ofile << '\n' << m_indent;
	}		
	m_ofile << kc_CloseLevel; //Finish vertices set header
}

void KismoAscii_FileWriter::writeNormals( MFnMesh& mesh  )
{
	m_ofile.precision( 6 ); //Set adequate precision
	MFloatVectorArray normalArray;
	mesh.getNormals( normalArray );
	unsigned int normalCount = normalArray.length();

	WRITE_NODE_HEADER( kc_MeshNormalsHeader, "type\tfloat3\tcount\t" << normalCount );
	m_ofile << kc_OpenLevel kc_StartLevel;

	for ( k_Count normalIndex = 0; normalIndex < normalArray.length(); ++ normalIndex )
	{
		m_ofile << '\t' << normalArray[ normalIndex ].x << '\t' << normalArray[ normalIndex ].y << '\t' << normalArray[ normalIndex ].z;
		if ( normalIndex != normalCount-1 && !((normalIndex+1) % 4))
			m_ofile << "\n" << m_indent;
	}

	m_ofile << kc_CloseLevel; //Finish normals set header
}

void KismoAscii_FileWriter::writeFaces( MFnMesh& mesh  )
{
	// Here is the trick, get all the nodes connected to this mesh, whether they are on 
	//the DAG or not. This will include all the materials.
	k_UInt32 instanceNumber = mesh.dagPath().instanceNumber();
	MObjectArray polygonSets;
	MObjectArray polygonComponents;

	//Get the connected sets and members used to determine texturing of different faces
	mesh.getConnectedSetsAndMembers( instanceNumber, polygonSets, polygonComponents, true );

	k_UInt32 mtrlCount = polygonSets.length();
	
	//When materials are set the initialShadingGroup for the whole mesh still remains
	if (mtrlCount > 1 )
		mtrlCount--;

	k_UInt32 subSetOffset = 0;

	// iterate over all the connected sets and look for materials
	for ( k_UInt32 i = 0; i < mtrlCount; i++ )
	{
		MObject set = polygonSets[i];
		MObject component = polygonComponents[i];

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
		MItMeshPolygon polyIter( mesh.dagPath(), component );
		
		MString mtrlName = MFnDependencyNode( srcPlugArray[ 0 ].node() ).name();
		
		//Count the number of triangles
		int triangleCount = 0;
		while( !polyIter.isDone() )
		{	
			int count = 0;
			polyIter.numTriangles( count );
			triangleCount += count;
			polyIter.next();
		}
		polyIter.reset();

		unsigned int polyCount = polyIter.count();

		WRITE_NODE_HEADER( kc_MeshFaceSetHeader, "material\t" << mtrlName 
			<< "\ttype\tU_N_V\tcount\t" << triangleCount );
		m_ofile << kc_OpenLevel kc_StartLevel;

		k_Count polyIndex = 0;
		
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
					//Get the index to the vertex in the polygon
					unsigned int vert = tvert + tri;
					
					//triangles always begin with the 1st polygon vertex ( triangle fan )
					if ( tvert == 0 )
						vert = 0;

					//Get the UV index
					int uvIndex;
					polyIter.getUVIndex( vert, uvIndex ); 

					k_UInt32 normalIndex = polyIter.normalIndex( vert );

					k_UInt32 vertexIndex = polyIter.vertexIndex( vert );
					m_ofile << '\t' << uvIndex << '\t' << normalIndex << '\t' << vertexIndex;
				}
				
				if ( polyIndex != triangleCount-1 && !((polyIndex+1) % 4))
					m_ofile << "\n" << m_indent;				
				++polyIndex;
			}

			//Move the iterator to the next polygon
			polyIter.next();
		}
		
		m_ofile << kc_CloseLevel;
	}
}

//void KismoAscii_FileWriter::writeSkinCluster( DagNode* node  )
//{	
//	MFnDependencyNode depNode( node->path );
//
//	MFnSkinCluster cluster( depNode. );
//	START_NODE_HEADER( kc_SkinClusterHeader );
//
//	m_ofile << "id\t" << MFnDependencyNode( cluster ).name();
//
//	END_NODE_HEADER();
//
//}

void KismoAscii_FileWriter::writeJointNode( DagNode* node  )
{	
	MFnIkJoint joint = node->path;
	START_NODE_HEADER( kc_JointHeader );

	m_ofile << "id\t" << MFnDependencyNode( joint ).name();
	writeTransformationData( node );

	END_NODE_HEADER();

}

void KismoAscii_FileWriter::writeMaterial( MFnLambertShader& mtrl )
{

	START_NODE_HEADER( kc_MaterialHeader );
		
	m_ofile << "id\t" << mtrl.name();
	
	END_NODE_HEADER(); //End material header

	OPEN_LEVEL();

	//-------------------- AMBIENT --------------------
	START_NODE_HEADER( kc_MaterialAmbientHeader );

	//get the ambient colour
	MColor ambient = mtrl.ambientColor();		
	//Output the ambient colour
	m_ofile << "colour\t" << ambient.r << '\t' << ambient.g << '\t' << ambient.b << '\t' << ambient.a;	
	//Output the ambient texture filename
	k_String ambientFile;
	if ( getTextureFile( mtrl, "ambientColor", ambientFile ) )
		m_ofile << "\ttexture\t" << ambientFile;

	END_NODE_HEADER();
	//-------------------------------------------------

	
	//-------------------- DIFFUSE --------------------
	START_NODE_HEADER( kc_MaterialDiffuseHeader );

	//get the diffuse colour
	MColor diffuse = mtrl.color();
	//////Scale the colour by the diffuse coefficient which should give the actual diffuse
	///diffuse *= fn.diffuseCoeff();
	//Output the diffuse colour
	m_ofile << "colour\t" << diffuse.r << '\t' << diffuse.g << '\t' << diffuse.b << '\t' << diffuse.a;
	//Output the diffuse texture filename
	k_String diffuseFile;
	if ( getTextureFile( mtrl, "color", diffuseFile ) )
		m_ofile << "\ttexture\t" << diffuseFile << "\tcoeff\t" << mtrl.diffuseCoeff();	

	END_NODE_HEADER();
	//-------------------------------------------------
		
	//-------------------- TRANSPARENCY --------------------
	START_NODE_HEADER( kc_MaterialTransparencyHeader );

	//get the transparency colour
	MColor transparency = mtrl.transparency();
	//Output the diffuse colour
	m_ofile << "colour\t" << transparency.r << '\t' << transparency.g << '\t' << transparency.b << '\t' << transparency.a;
	//Output the diffuse texture filename
	k_String transparencyFile;
	if ( getTextureFile( mtrl, "transparency", transparencyFile ) )
		m_ofile << "\ttexture\t" << transparencyFile;

	END_NODE_HEADER();
	//-------------------------------------------------

	//-------------------- PHONG --------------------
	if( mtrl.hasObj( MFn::kPhong ) ) 
	{
		MFnPhongShader phongMtrl( mtrl.object() );
		//-------------------- SPECULAR --------------------
		START_NODE_HEADER( kc_MaterialSpecularHeader );

		//get the specular colour
		MColor specular = phongMtrl.specularColor();		
		//Output the specular colour & shininess
		m_ofile << "colour\t" << specular.r << '\t' << specular.g << '\t' << specular.b << '\t' << specular.a << "\tcos_power\t" << phongMtrl.cosPower();	
		//Output the specular texture filename
		k_String specularFile;
		if ( getTextureFile( phongMtrl, "specularColor", diffuseFile ) )
			m_ofile << "\ttexture\t" << specularFile;

		END_NODE_HEADER();
		//-------------------------------------------------
	}

	CLOSE_LEVEL();

	return;
}

bool KismoAscii_FileWriter::getTextureFile( MFnDependencyNode& fn, const k_String& name, k_String& outFilename )
{
	MPlug plug = fn.findPlug( name.c_str() );


	//Get the plugs connected to the attribute
	MPlugArray plugs;
	plug.connectedTo( plugs, true, false );

	//Loop through all the plugs
	for( int i = 0; i != plugs.length(); ++i )
	{
  		//Check if the node is a file texture
		if( plugs[i].node().apiType() == MFn::kFileTexture )
		{
			// attach a dependency node to the file node
			MFnDependencyNode fn( plugs[i].node());

			MPlug ftn = fn.findPlug("ftn");

			// get the filename from the attribute
			
			MString texname;
			ftn.getValue( texname );
			outFilename = texname.substring( texname.rindex( '/' )+1, texname.length() ).asChar();
						
			return true;
		}
	}

	return false;
}


void KismoAscii_FileWriter::writeSkinClusters() ///< Write all the mesh skinning clusters
{
	MItDependencyNodes iter( MFn::kSkinClusterFilter );
	for ( ; !iter.isDone(); iter.next() ) 
	{  
		MObject object = iter.item();
		// access the data
		MFnSkinCluster fn(object);
		MDagPathArray infs;

		unsigned int nInfs = fn.influenceObjects( infs, 0 );

		// loop through the geometries affected by this cluster
		unsigned int nGeoms = fn.numOutputConnections();
		for (unsigned int ii = 0; ii < nGeoms; ++ii) 
		{

			unsigned int index;
			index = fn.indexForOutputConnection(ii); 

			// get the dag path of the ii'th geometry
			MDagPath skinPath;
			fn.getPathAtIndex(index,skinPath);

			// iterate through the components of this geometry
			MItGeometry gIter(skinPath);

			// print out the name of the skin cluster, 
			// the vertexCount and the influenceCount
			m_ofile << "Skin " <<endl;
			m_ofile << skinPath.partialPathName().asChar() <<endl;
			m_ofile << "pointcount " << gIter.count() <<endl; 
			m_ofile << "numInfluences " << nInfs <<endl; 

			// print out the influence object names 
			for (unsigned int kk = 0; kk < nInfs; ++kk) {

				m_ofile << infs[kk].partialPathName().asChar() <<" "; 

			} 
			m_ofile << endl;

			for ( ; !gIter.isDone(); gIter.next() ) {


				MObject comp = gIter.component();

				// Get the weights for this vertex (one per influence object)
				//
				MFloatArray wts;
				unsigned int infCount;
				fn.getWeights(skinPath,comp,wts,infCount);

				if ( 0 != infCount && 
					//gIter.index() == i && 
					!gIter.isDone()) 

				{

					unsigned int numWeights=0;
					float outWts[40]={1.0f,0};
					unsigned int outInfs[40]={0}; 

					// Output the weight data for this vertex
					//
					for(unsigned int jj=0;jj!=infCount;++jj)
					{

						// ignore weights of little effect 
						if(wts[jj] > 0.001f)
						{   
							if(numWeights!=0)
							{   outWts[0] -= wts[jj];
							outWts[numWeights]=wts[jj]; 

							}
							outInfs[numWeights] = jj;
							++numWeights; 

						} 

					}
					m_ofile<< "\t" << numWeights ;
					for(int k=0;k!=numWeights;++k)
					{

						m_ofile << " " << outInfs[k] 
						<< " " << outWts[k]; 

					}

					m_ofile << endl;
					gIter.next();


				}


			}


		}

	}
}