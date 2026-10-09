/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\Kismo_MayaPlugin\KismoAscii_FileWriter.cpp
 * Status: current (newest copy of this file name in the archive)
 */
#include "KismoAscii_FileWriter.h"
#include "Framework/DateTime.h"
#include "maya/MFnPhongShader.h"
#include "maya/MFnIkJoint.h"
#include "maya/MFnSkinCluster.h"
#include "maya/MDagPathArray.h"
#include "maya/MItGeometry.h"
#include "maya/MBoundingBox.h"
#include "maya/MFnSphereData.h"
#include "maya/MFnMessageAttribute.h"
#include "maya/MFnEnumAttribute.h"
//#include "maya/MFnBoxData.h"
#include "framework/BoundingSphere.h"
#include "framework/DateTime.h"
#include "framework/String.h"

//------------------------------------------
#define AsciiFileHeader _T("Kismo Ascii Scene Object v1.00")
#define AsciiComment _T("//")
#define AsciiCommentHeader AsciiComment _T("\t")
#define ObjectHierarchyHeader _T("scene")
#define MeshesHeader _T("meshes")
#define MaterialsHeader _T("materials")

#define TransformNodeHeader _T("node")
#define LODGroupNodeHeader _T("lod_group")
#define MeshNodeHeader _T("mesh")

#define RigidNodeHeader _T("phys:actor")
#define RigidActiveNodeHeader _T("phys:body")
#define PhysMeshNodeHeader _T("phys:mesh")
#define PhysSceneNodeHeader _T("phys:scene")
#define PhysConstraintNodeHeader _T("phys:constraint")

#define MeshHeader _T("mesh")
#	define MeshUVSetHeader _T("uv_set")
#	define MeshPositionsHeader _T("positions")
#	define MeshNormalsHeader _T("normals")
#	define MeshFaceSetHeader _T("face_set")
#	define MeshBoundingBoxHeader _T("bounding_box")
#	define MeshBoundingSphereHeader _T("bounding_sphere")
#define SkinClusterHeader _T("skin_cluster")
#define JointHeader _T("joint")

#define BoxShapeNodeHeader _T("box")
#define SphereShapeNodeHeader _T("sphere")
#define CylinderShapeNodeHeader _T("cylinder")
#define PlaneShapeNodeHeader _T("plane")

#define MaterialHeader _T("material")
#	define MaterialAmbientHeader _T("ambient")
#	define MaterialDiffuseHeader _T("diffuse")
#	define MaterialSpecularHeader _T("specular")
#	define MaterialTransparencyHeader _T("transparency")

#define OpenLevelHeader _T("{")
#define CloseLevelHeader _T("}")

#define ParamCount _T("count")
#define ParamID _T("id")

#define StartNodeParam _T("(")
#define EndNodeParam _T(")")


#define StartParamData _T("\"")
#define EndParamData _T("\"")
//------------------------------------------


#define StartNodeParamHeader StartNodeParam _T("\t")
#define EndNodeParamHeader _T("\t") EndNodeParam
#define OpenLevel /*_T("\n") << m_indent.c_str() << */ OpenLevelHeader 
#define CloseLevel _T("\n") << m_indent.c_str() << CloseLevelHeader
#define StartLevel _T("\n") << m_indent.c_str()

#define START_NODE_HEADER( _nodeName ) \
	m_ofile << StartLevel << _nodeName StartNodeParamHeader

#define END_NODE_HEADER() \
	m_ofile << EndNodeParamHeader

#define WRITE_NODE_HEADER( _nodeName, _param ) \
	START_NODE_HEADER( _nodeName ) _param; END_NODE_HEADER()

#define OPEN_LEVEL() m_ofile << OpenLevel; incrementDepth()

#define CLOSE_LEVEL() decrementDepth(); m_ofile << CloseLevel


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
	m_ofile << AsciiCommentHeader << AsciiFileHeader << "\n";

	//Write the creation date
	frx::Date date;
	m_ofile << AsciiCommentHeader << date.day() << '/' <<  date.month() << '/' << date.year() << "\n";

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
	WRITE_NODE_HEADER( ObjectHierarchyHeader, "" );
	OPEN_LEVEL();

	//TODO: Detect physics and only add when needed
	writePhysSceneNode( 0 );

	for ( std::vector< DagNode* >::const_iterator i = m_nodes.begin(); i != m_nodes.end(); ++i )
	{
		if ( !(*i)->parentCount() ) //Only write root nodes...at the root!
			writeNode( (*i ) );
	}

	CLOSE_LEVEL();
}

void KismoAscii_FileWriter::writeMeshes()
{ 	
	WRITE_NODE_HEADER( MeshesHeader, "count\t" StartParamData << m_meshNamesByPriority.size() << EndParamData );
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

	WRITE_NODE_HEADER( MaterialsHeader, "count\t" StartParamData << mtrlCount << EndParamData );
	OPEN_LEVEL();
	
	iterDep.reset( MFn::kLambert );
	//Iterate through all the materials in the iterator
	while ( !iterDep.isDone() )
	{
		MFnLambertShader mat( iterDep.thisNode() );
		writeMaterial( mat );

		iterDep.next();
	}

	CLOSE_LEVEL();
}

void KismoAscii_FileWriter::writeNode( DagNode* node )
{
	if ( node->type & NodeType_Joint )
		writeJointNode( node );
	else
	if ( node->type & NodeType_DynamicsNode ) //Dynamics node allows better control of dynamic proiperties and shape
	{
		writeDynamicsNode( node );
		return;
	}
	else
	//if ( node->type & NodeType_SkinCluster )
	//	writeSkinCluster( node );
	//else
	if ( node->type & NodeType_LODGroup )
		writeLodGroupNode( node );
	else
	if ( node->type & NodeType_RigidConstraint )
		writeDynamicConstraintNode( node );
	else
	if ( node->type & NodeType_Transform )
		writeTransformNode( node );
	else
	if ( node->type & NodeType_Mesh )
		writeMeshNode( node, true );
	else
	if ( node->type & NodeType_RigidBody )
		writeRigidBodyActorNode( node );
	else
		return;

	//Write all the child nodes
	writeChildNodes( node );

	if ( node->type & NodeType_LODGroup )
	{
		assert( m_currentMeshPriority == 0 );
		m_priorityStep = 0;
	}
}

void KismoAscii_FileWriter::writeDynamicsNode( DagNode* node )
{	
	if ( node->childCount() )
	{
		bool actorFound = false;
		for ( DagNode::const_iterator it = node->childBegin(); it != node->childEnd(); ++it )
		{
			//Write out the rigid body paramaters
			//This will only output the body paramaters and no dynamics information
			if ( (*it)->type & NodeType_RigidBody )
			{
				writeRigidBodyActorNode( (*it), false );
				actorFound = true;
			}
		}

		if ( !actorFound )
			return; //TODO: report an error!

		OPEN_LEVEL();

		for ( DagNode::const_iterator it = node->childBegin(); it != node->childEnd(); ++it )
		{
			//Write out the dynamic rigid body paramaters
			//this adds dyhnamic information if the body is active
			if ( (*it)->type & NodeType_RigidBody )
				writeRigidBodyNode( (*it) );
			else
			//Write out the rigid body shape
			if ( (*it)->type & NodeType_Mesh )
			{
				writePhysMeshNode( (*it) );
			}
		}

		CLOSE_LEVEL();
	}

}

void KismoAscii_FileWriter::writeChildNodes( DagNode* node )
{
	if ( node->childCount() )
	{
		OPEN_LEVEL();
		for ( DagNode::const_iterator it = node->childBegin(); it != node->childEnd(); ++it )
		{
			assert( *it );
			writeNode( *it );
		}
		CLOSE_LEVEL();
	}
}

void KismoAscii_FileWriter::writeTransformNode( DagNode* node  )
{
	MFnTransform transform = node->path;
	
	WRITE_NODE_HEADER( TransformNodeHeader, 
		"id\t" StartParamData << MFnDependencyNode( transform ).name() << EndParamData;
		writeTransformationData( node );
		m_ofile );

	//for ( Count attrIndex = 0; attrIndex < transform.attributeCount(); ++attrIndex )
	//{
	//	MFnAttribute attr = transform.attribute( attrIndex );
	//	if ( !attr.internal() && !attr.type() )
	//	{
	//		m_ofile 
	//			<< attr.name() << "\n";
	//	}
	//}

}

void KismoAscii_FileWriter::writeTransformationData( DagNode* node )
{
	MFnTransform transform = node->path;
	MTransformationMatrix trans = transform.transformation();
		
	//MVector scale = trans.eulerRotation();	//get the objects rotation

	MVector pos = trans.getTranslation( MSpace::kTransform );//Get the objects translation/position
	MVector pivot = trans.rotatePivot( MSpace::kTransform );//Get the objects translation/position
	double scale[3];
	trans.getScale( scale, MSpace::kTransform );//Get the objects translation/position

	double rx,ry,rz,rw; //quaternion rotation
	trans.getRotationQuaternion( rx, ry, rz, rw );

	MEulerRotation eulerRotation = trans.eulerRotation();	//get the objects rotation
	MString eulerOrder;
	switch( eulerRotation.order )
	{
		case MEulerRotation::kXYZ: eulerOrder = "XYZ"; break;
		case MEulerRotation::kYZX: eulerOrder = "YZX"; break;
		case MEulerRotation::kZXY: eulerOrder = "ZXY"; break;
		case MEulerRotation::kXZY: eulerOrder = "XZY"; break;
		case MEulerRotation::kYXZ: eulerOrder = "YXZ"; break;
		case MEulerRotation::kZYX: eulerOrder = "ZYX"; break;
		default: assert( false ); break;
	};

	m_ofile 
		<< "\tpos\t" StartParamData << pos.x << '\t' << pos.y << '\t' << pos.z << EndParamData 
		<< "\tquart\t" StartParamData << rx << '\t' << ry << '\t' << rz << '\t' << rw << EndParamData
		<< "\tscale\t" StartParamData << scale[0] << '\t' << scale[1] << '\t' << scale[2] << EndParamData
		<< "\tpivot\t" StartParamData << pivot.x << '\t' << pivot.y << '\t' << pivot.z << EndParamData
		<< "\teuler\t" StartParamData << eulerRotation.x * math::RadToDeg << '\t' << eulerRotation.y * math::RadToDeg << '\t' << eulerRotation.z  * math::RadToDeg << EndParamData
		<< "\teuler_order\t" StartParamData << eulerOrder << EndParamData;
}


void KismoAscii_FileWriter::writePhysSceneNode( DagNode* node )
{
	WRITE_NODE_HEADER( PhysSceneNodeHeader, 
			"id\t" StartParamData << "physicScene" << EndParamData );
}

void KismoAscii_FileWriter::writeDynamicConstraintNode( DagNode* node )
{
	MFnTransform transform = node->path;
	START_NODE_HEADER( PhysConstraintNodeHeader ); 

	//Get the name for usefullness
	m_ofile << "id\t" StartParamData << MFnDependencyNode( transform ).name() << EndParamData;

	//The joint needs a location!
	writeTransformationData( node );

	MPlug plug;

	//Get the constraint type
	plug = transform.findPlug( "constraintType" );
	if( !plug.isNull() )
	{
		//get the enumerator type for the constraint
		MFnEnumAttribute typeEnum( transform.attribute( "constraintType" ) );
		//get the type by looking up the index into the enumerator
		m_ofile << "\ttype\t" StartParamData << typeEnum.fieldName( plug.asInt() ).asChar() << EndParamData;
	}

	//Get the first body that is attached
	plug = transform.findPlug( "rigidBody1" );
	if( !plug.isNull() && plug.isConnected() )
	{
		// will hold the connections to the surfaceShader attribute
		MPlugArray plugs;

		// get the material connected to the surface shader
		plug.connectedTo( plugs, true, false );

		// if we found a material
		if( plugs.length() )
		{
			MFnDependencyNode fn( plugs[0].node() );
			m_ofile << "\tbody1\t" StartParamData << fn.name().asChar() << EndParamData;
		}
	}

	plug = transform.findPlug( "rigidBody2" );
	if( !plug.isNull() && plug.isConnected() )
	{
		// will hold the connections to the surfaceShader attribute
		MPlugArray plugs;

		// get the material connected to the surface shader
		plug.connectedTo( plugs, true, false );

		// if we found a material
		if( plugs.length() )
		{
			MFnDependencyNode fn( plugs[0].node() );
			m_ofile << "\tbody2\t" StartParamData << fn.name().asChar() << EndParamData;
		}
	}

	//relativeTo 
	//http://www.kxcad.net/autodesk/maya/Maya_Documentation/Nodes/rigidConstraint.html


	END_NODE_HEADER(); //Finish the header
}

void KismoAscii_FileWriter::writeRigidBodyNode( DagNode* node )
{
	MFnDagNode transform = node->path;

	MPlug plug;
	
	//FGet whether the body is active or not
	bool active = false;
	plug = transform.findPlug( "active" );
	if( !plug.isNull() )
		plug.getValue( active );

	if ( !active )
		return;

	START_NODE_HEADER( RigidActiveNodeHeader ); 


	plug = transform.findPlug( "mass" );
	assert(!plug.isNull() );
	if( !plug.isNull() )
	{
		double dmass; plug.getValue( dmass );
		m_ofile << "mass\t" StartParamData <<  dmass << EndParamData ;
	}

	plug = transform.findPlug( "damping" );
	if( !plug.isNull() )
	{
		double damping; plug.getValue( damping  );
		m_ofile << "\tdamping\t" StartParamData << damping << EndParamData ;
	}

	plug = transform.findPlug( "centerOfMass" );
	if( plug.isCompound()  )
	{
		m_ofile << "\tcentreOfMass\t" StartParamData;
		for ( unsigned int i = 0; i < 3; ++i )
		{
			if ( i != 0 ) m_ofile << '\t';
			double pos;// get the plugs to the child attributes			
			plug.child(i).getValue( pos );
			m_ofile << pos;
		}
		m_ofile << EndParamData;
	}

	END_NODE_HEADER(); //Finish the header
}

void KismoAscii_FileWriter::writeRigidBodyActorNode( DagNode* node, bool writeActiveBody )
{
	MFnDagNode transform = node->path;
	
	START_NODE_HEADER( RigidNodeHeader ); 

	m_ofile << "id\t" StartParamData << MFnDependencyNode( transform ).name() << EndParamData;

	//MFnNumericData threshold = transform.attribute( "threshold[0]" );

	// get the MPlug for the compound attribute
	MPlug plug;

	plug = transform.findPlug( "dynamicFriction" );
	if( !plug.isNull() )
	{
		double dynamicFriction; plug.getValue( dynamicFriction  );
		m_ofile << "\tdynamicFriction \t" StartParamData <<  dynamicFriction << EndParamData ;
	}

	plug = transform.findPlug( "staticFriction" );
	if( !plug.isNull() )
	{
		double staticFriction; plug.getValue( staticFriction   );
		m_ofile << "\tstaticFriction\t" StartParamData <<  staticFriction << EndParamData ;
	}

	plug = transform.findPlug( "collisionLayer" );
	if( !plug.isNull() )
	{
		double collisionLayer; plug.getValue( collisionLayer );
		m_ofile << "\tcollisionLayer\t" StartParamData <<  collisionLayer << EndParamData ;
	}

	plug = transform.findPlug( "bounciness" );
	if( !plug.isNull() )
	{
		double bounciness ; plug.getValue( bounciness  );
		m_ofile << "\tbounciness\t" StartParamData << bounciness << EndParamData ;
	}

	//FGet whether the body is active or not
	bool active = false;
	plug = transform.findPlug( "active" );
	if( !plug.isNull() )
		plug.getValue( active );

	END_NODE_HEADER(); //Finish the header

	//If the body is active we output a body node for the actor which defines if dynamic properties
	if ( writeActiveBody && active )
	{
		OPEN_LEVEL();
		writeRigidBodyNode( node );
		CLOSE_LEVEL();		
	}
	
}

void KismoAscii_FileWriter::writeBoxShape( DagNode* node  )
{	
	WRITE_NODE_HEADER( BoxShapeNodeHeader, 
			"size\t" StartParamData << "0\t0\t0" << EndParamData );
}

void KismoAscii_FileWriter::writeSphereShape( DagNode* node  )
{
	//MFnDependencyNode depNode( node->path );
	//MFnSphereData shape( depNode;

	WRITE_NODE_HEADER( SphereShapeNodeHeader, 
			"radius\t" StartParamData << "0" << EndParamData );

}

void KismoAscii_FileWriter::writeCylinderShape( DagNode* node  )
{
	WRITE_NODE_HEADER( CylinderShapeNodeHeader, 
			"radius\t" StartParamData << "0" << EndParamData
			"\theight\t" StartParamData << "0" << EndParamData );
}

void KismoAscii_FileWriter::writePlaneShape( DagNode* node  )
{
	WRITE_NODE_HEADER( PlaneShapeNodeHeader, 
			"size\t" StartParamData << "0\t0" << EndParamData );
}


void KismoAscii_FileWriter::writeLodGroupNode( DagNode* node )
{
	MFnTransform transform = node->path;
	WRITE_NODE_HEADER( LODGroupNodeHeader, 
		"id\t" StartParamData << MFnDependencyNode( transform ).name() << EndParamData;
	
		//MFnNumericData threshold = transform.attribute( "threshold[0]" );

		// get the MPlug for the compound attribute
		MPlug threshold = transform.findPlug( "threshold" );

		if( threshold.isArray() )
		{
			m_ofile << "\tthresholds\t" StartParamData <<  threshold.numElements();
			for ( unsigned int i = 0; i < threshold.numElements(); ++i )
			{
				// get the plugs to the child attributes
				MPlug threshN = threshold.elementByPhysicalIndex( i );
				double dist;
				threshN.getValue( dist );

				m_ofile << '\t' << dist;
			}
			m_ofile << EndParamData;
			
			//As the first mesh is the highest lod level it should be set to a lower priority
			m_currentMeshPriority -= threshold.numElements() + 1;
			m_priorityStep = 1;
		}

		writeTransformationData( node );
	
	m_ofile ); //Finish the header
}

void KismoAscii_FileWriter::writePhysMeshNode( DagNode* node )
{	
	MFnMesh mesh = node->path;
	String name = MFnDependencyNode( mesh ).name().asChar();

	WRITE_NODE_HEADER( PhysMeshNodeHeader, "id\t" StartParamData << name.c_str() << EndParamData );

	if ( m_meshsByName.find( name ) == m_meshsByName.end() )
	{
		m_meshNamesByPriority.insert( MESH_PRIORITY_MAP::value_type( m_currentMeshPriority, name ) );
		m_meshsByName.insert( MESH_NAME_MAP::value_type( name, node ) );
	}
	
}

void KismoAscii_FileWriter::writeMeshNode( DagNode* node, bool firstPass )
{		
	MFnMesh mesh = node->path;
	String name = MFnDependencyNode( mesh ).name().asChar();

	if ( firstPass )
	{
		m_currentMeshPriority += m_priorityStep;

		WRITE_NODE_HEADER( MeshNodeHeader, "id\t" StartParamData << name.c_str() << EndParamData );

		if ( m_meshsByName.find( name ) == m_meshsByName.end() )
		{
			m_meshNamesByPriority.insert( MESH_PRIORITY_MAP::value_type( m_currentMeshPriority, name ) );
			m_meshsByName.insert( MESH_NAME_MAP::value_type( name, node ) );
		}
		
	}
	else
	{
		WRITE_NODE_HEADER( MeshHeader, "id\t" StartParamData << name.c_str() << EndParamData );
		OPEN_LEVEL();
		

		//Bounding box
		START_NODE_HEADER( MeshBoundingBoxHeader );

		MBoundingBox boundingBox = mesh.boundingBox();
		MPoint minPoint =  boundingBox.min().cartesianize();
		MPoint maxPoint =  boundingBox.max().cartesianize();
		m_ofile << "min\t" StartParamData << minPoint.x << '\t' << minPoint.y << '\t' << minPoint.z << EndParamData
			<< "\tmax\t" StartParamData << maxPoint.x << '\t' << maxPoint.y << '\t' << maxPoint.z << EndParamData;

		END_NODE_HEADER();

		//Bounding box test####################################
		{
			MFloatPointArray vertexArray;
			mesh.getPoints( vertexArray );
			Vector3::Array vecArray;
			vecArray.reserve( vertexArray.length() );
			for ( Count vertIndex = 0; vertIndex < vertexArray.length(); ++ vertIndex )
			{
				vertexArray[ vertIndex ].cartesianize();
				vecArray.push_back( Vector3( vertexArray[ vertIndex ].x, vertexArray[ vertIndex ].y, vertexArray[ vertIndex ].z ) );
			}
			BoundingSphere bs( vecArray );
			START_NODE_HEADER( MeshBoundingSphereHeader );
			m_ofile << "center\t" StartParamData <<  bs.center().x << '\t' <<  bs.center().y << '\t' <<  bs.center().z << EndParamData 
				<< "\tradius\t" StartParamData << bs.radius() << EndParamData;

			END_NODE_HEADER();
		}
		//Bounding box test####################################


		// UV sets		
		writeUVSets( mesh );	

		// Output positions	
		writePositions( mesh );	
		
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
	for ( Count setindex = 0; setindex < uvSetNames.length(); ++setindex )
	{
		MFloatArray uArray;
		MFloatArray vArray;
		mesh.getUVs( uArray, vArray, &uvSetNames[ setindex ] );
		
		if ( !uArray.length() ) //if there are no coordinates for this map then skip it
			continue;

		unsigned int uvCount = uArray.length();
		WRITE_NODE_HEADER( MeshUVSetHeader, "id\t" StartParamData << uvSetNames[setindex] 
			<< EndParamData "\ttype\t" StartParamData "float2" EndParamData "\tcount\t" StartParamData 
			<< uvCount << EndParamData );
		m_ofile << OpenLevel StartLevel;
		for ( Count uvIndex = 0; uvIndex < uvCount; ++ uvIndex )
		{
			m_ofile << '\t' << uArray[ uvIndex ] << '\t' << vArray[ uvIndex ];
			if ( uvIndex != uvCount-1 && !((uvIndex+1) % 8))
				m_ofile << "\n" << m_indent.c_str();

		}
		m_ofile << CloseLevel;
	}
}
void KismoAscii_FileWriter::writePositions( MFnMesh& mesh  )
{
	m_ofile.precision( 11 ); //set high precision

	MFloatPointArray vertexArray;
	mesh.getPoints( vertexArray, MSpace::kTransform );
	unsigned int vertexCount = vertexArray.length();

	WRITE_NODE_HEADER( MeshPositionsHeader, "type\t" StartParamData "float3" EndParamData "\tcount\t" StartParamData << vertexCount << EndParamData );
	m_ofile << OpenLevel StartLevel;

	for ( Count vertIndex = 0; vertIndex < vertexArray.length(); ++ vertIndex )
	{
		vertexArray[ vertIndex ].cartesianize();
		m_ofile << '\t' << vertexArray[ vertIndex ].x << '\t' << vertexArray[ vertIndex ].y << '\t' << vertexArray[ vertIndex ].z;
		if ( vertIndex != vertexCount - 1 && !((vertIndex+1) % 4))
			m_ofile << "\n" << m_indent.c_str();
	}		
	m_ofile << CloseLevel; //Finish positions set header
}

void KismoAscii_FileWriter::writeNormals( MFnMesh& mesh  )
{
	m_ofile.precision( 6 ); //Set adequate precision
	MFloatVectorArray normalArray;
	mesh.getNormals( normalArray );
	unsigned int normalCount = normalArray.length();

	WRITE_NODE_HEADER( MeshNormalsHeader, "type\t" StartParamData "float3" EndParamData "\tcount\t" StartParamData << normalCount << EndParamData );
	m_ofile << OpenLevel StartLevel;

	for ( Count normalIndex = 0; normalIndex < normalArray.length(); ++ normalIndex )
	{
		m_ofile << '\t' << normalArray[ normalIndex ].x << '\t' << normalArray[ normalIndex ].y << '\t' << normalArray[ normalIndex ].z;
		if ( normalIndex != normalCount-1 && !((normalIndex+1) % 4))
			m_ofile << "\n" << m_indent.c_str();
	}

	m_ofile << CloseLevel; //Finish normals set header
}

void KismoAscii_FileWriter::writeFaces( MFnMesh& mesh  )
{
	// Here is the trick, get all the nodes connected to this mesh, whether they are on 
	//the DAG or not. This will include all the materials.
	UInt32 instanceNumber = mesh.dagPath().instanceNumber();
	MObjectArray polygonSets;
	MObjectArray polygonComponents;

	//Get the connected sets and members used to determine texturing of different faces
	mesh.getConnectedSetsAndMembers( instanceNumber, polygonSets, polygonComponents, true );

	UInt32 mtrlCount = polygonSets.length();

	UInt32 subSetOffset = 0;

	// iterate over all the connected sets and look for materials
	for ( UInt32 i = 0; i < mtrlCount; i++ )
	{
		MObject set = polygonSets[i];
		MObject component = polygonComponents[i];

		MFnDependencyNode dnSet( set );

		//When materials are set the initialShadingGroup for the whole mesh sometimes remains	
		//If there is only one material we obviousl need to keep it
		if ( mtrlCount > 1
			&& dnSet.name() == "initialShadingGroup" )
			continue;

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

		WRITE_NODE_HEADER( MeshFaceSetHeader, "material\t" StartParamData << mtrlName
			<< EndParamData "\tsides\t" StartParamData "3" EndParamData "\ttype\t" 
			StartParamData "U_N_V" EndParamData "\tcount\t" StartParamData << triangleCount << EndParamData );
		m_ofile << OpenLevel StartLevel;

		Count polyIndex = 0;
		
		//loop through all the polygons
		while( !polyIter.isDone() )
		{	
			//get the vertex count for this polygon
			int vertexCount = polyIter.polygonVertexCount();

			//Loop for the number of tiangles that make up this polygon
			for ( int tri = 0; tri <= vertexCount - 3; tri++ )
			{
				//Loop 3 times - for the 3 positions of a triangle
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

					UInt32 normalIndex = polyIter.normalIndex( vert );

					UInt32 vertexIndex = polyIter.vertexIndex( vert );
					m_ofile << '\t' << uvIndex << '\t' << normalIndex << '\t' << vertexIndex;
				}
				
				if ( polyIndex != triangleCount-1 && !((polyIndex+1) % 4))
					m_ofile << "\n" << m_indent.c_str();				
				++polyIndex;
			}

			//Move the iterator to the next polygon
			polyIter.next();
		}
		
		m_ofile << CloseLevel;
	}
}

//void KismoAscii_FileWriter::writeSkinCluster( DagNode* node  )
//{	
//	MFnDependencyNode depNode( node->path );
//
//	MFnSkinCluster cluster( depNode. );
//	START_NODE_HEADER( SkinClusterHeader );
//
//	m_ofile << "id\t" << MFnDependencyNode( cluster ).name();
//
//	END_NODE_HEADER();
//
//}

void KismoAscii_FileWriter::writeJointNode( DagNode* node  )
{	
	MFnIkJoint joint = node->path;
	START_NODE_HEADER( JointHeader );

	m_ofile << "id\t" StartParamData << MFnDependencyNode( joint ).name() << EndParamData;
	writeTransformationData( node );

	END_NODE_HEADER();

}

void KismoAscii_FileWriter::writeMaterial( MFnLambertShader& mtrl )
{

	START_NODE_HEADER( MaterialHeader );
		
	m_ofile << "id\t" StartParamData << mtrl.name() << EndParamData;
	
	END_NODE_HEADER(); //End material header

	OPEN_LEVEL();

	//-------------------- AMBIENT --------------------
	START_NODE_HEADER( MaterialAmbientHeader );

	//get the ambient colour
	MColor ambient = mtrl.ambientColor();		
	//Output the ambient colour
	m_ofile << "colour\t" StartParamData 
		<< ambient.r << '\t' << ambient.g << '\t' << ambient.b << '\t' << ambient.a << EndParamData;	
	//Output the ambient texture filename
	String ambientFile;
	if ( getTextureFile( mtrl, "ambientColor", ambientFile ) )
		m_ofile << "\ttexture\t" StartParamData << ambientFile.c_str() << EndParamData;

	END_NODE_HEADER();
	//-------------------------------------------------

	
	//-------------------- DIFFUSE --------------------
	START_NODE_HEADER( MaterialDiffuseHeader );

	//get the diffuse colour
	MColor diffuse = mtrl.color();
	//////Scale the colour by the diffuse coefficient which should give the actual diffuse
	///diffuse *= fn.diffuseCoeff();
	//Output the diffuse colour
	m_ofile << "colour\t" StartParamData 
		<< diffuse.r << '\t' << diffuse.g << '\t' << diffuse.b << '\t' << diffuse.a << EndParamData;
	//Output the diffuse texture filename
	String diffuseFile;
	if ( getTextureFile( mtrl, "color", diffuseFile ) )
		m_ofile << "\ttexture\t" StartParamData 
		<< diffuseFile.c_str() << "\tcoeff\t" << mtrl.diffuseCoeff() << EndParamData;	

	END_NODE_HEADER();
	//-------------------------------------------------
		
	//-------------------- TRANSPARENCY --------------------
	START_NODE_HEADER( MaterialTransparencyHeader );

	//get the transparency colour
	MColor transparency = mtrl.transparency();
	//Output the diffuse colour
	m_ofile << "colour\t" StartParamData 
		<< transparency.r << '\t' << transparency.g << '\t' << transparency.b << '\t' << transparency.a << EndParamData;
	//Output the diffuse texture filename
	String transparencyFile;
	if ( getTextureFile( mtrl, "transparency", transparencyFile ) )
		m_ofile << "\ttexture\t" StartParamData << transparencyFile.c_str() << EndParamData;

	END_NODE_HEADER();
	//-------------------------------------------------

	//-------------------- PHONG --------------------
	if( mtrl.hasObj( MFn::kPhong ) ) 
	{
		MFnPhongShader phongMtrl( mtrl.object() );
		//-------------------- SPECULAR --------------------
		START_NODE_HEADER( MaterialSpecularHeader );

		//get the specular colour
		MColor specular = phongMtrl.specularColor();		
		//Output the specular colour & shininess
		m_ofile << "colour\t" StartParamData
			<< specular.r << '\t' << specular.g << '\t' << specular.b << '\t' << specular.a 
			<< EndParamData "\tcos_power\t" StartParamData << phongMtrl.cosPower() << EndParamData;	
		//Output the specular texture filename
		String specularFile;
		if ( getTextureFile( phongMtrl, "specularColor", diffuseFile ) )
			m_ofile << "\ttexture\t" StartParamData << specularFile.c_str() << EndParamData;

		END_NODE_HEADER();
		//-------------------------------------------------
	}

	CLOSE_LEVEL();

	return;
}

bool KismoAscii_FileWriter::getTextureFile( MFnDependencyNode& fn, const String& name, String& outFilename )
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
	return; ///< Not implemented!

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
			m_ofile << skinPath.partialPathName().asChar() << endl;
			m_ofile << "pointcount\t" StartParamData << gIter.count() << endl; 
			m_ofile << "numInfluences\t" StartParamData << nInfs << endl; 

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