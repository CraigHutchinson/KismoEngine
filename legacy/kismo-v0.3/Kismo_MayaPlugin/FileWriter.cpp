/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\Kismo_MayaPlugin\FileWriter.cpp
 * Status: current (newest copy of this file name in the archive)
 */
#include "FileWriter.h"
#include "Framework/Common.h"
#include "maya/MFnDagNode.h"

FileWriter::FileWriter() {}
FileWriter::~FileWriter() 
{
	for ( std::vector< DagNode* >::iterator i = m_nodes.begin(); i != m_nodes.end(); ++i )
		frx::safeDelete( *i );
}

bool FileWriter::addNode( MDagPath path )
{
	if ( path.isInstanced() && path.instanceNumber() > 0 ) 
		return false;

	//First check that the path does not already exist
	for ( std::vector< DagNode* >::const_iterator i = m_nodes.begin(); i != m_nodes.end(); ++i )
	{
		if ( (*i)->path == path )
			return false;
	}

	MFnDagNode node = path;
	String name = MFnDependencyNode( node ).name().asChar();

	//If we got here then the node  has not already been added
	UInt32 typeMask = 0;
	if ( path.hasFn( MFn::kMesh ) ) typeMask |= NodeType_Mesh;
	if ( path.hasFn( MFn::kTransform ) ) typeMask |= NodeType_Transform;
	if ( path.hasFn( MFn::kMaterial ) ) typeMask |= NodeType_Material;
	if ( path.hasFn( MFn::kLight ) ) typeMask |= NodeType_Light;
	if ( path.hasFn( MFn::kCamera ) ) typeMask |= NodeType_Camera;
	if ( path.hasFn( MFn::kLodGroup ) ) typeMask |= NodeType_LODGroup;
	if ( path.hasFn( MFn::kJoint ) ) typeMask |= NodeType_Joint;
	if ( path.hasFn( MFn::kRigid ) ) typeMask |= NodeType_RigidBody;
	if ( path.hasFn( MFn::kRigidConstraint ) ) typeMask |= NodeType_RigidConstraint;
	if ( path.hasFn( MFn::kField ) ) typeMask |= NodeType_Field;
	if ( name == "collision" )
		typeMask |= NodeType_DynamicsNode;
	//if ( path.hasFn( MFn::kSkinClusterFilter  ) ) typeMask |= NodeType_SkinCluster;

	DagNode* newNode = new DagNode( path, typeMask );
	
	m_nodes.push_back( newNode );

	return true;
}

MStatus FileWriter::write( const MString& filename )
{
	//Construc the node tree
	for ( std::vector< DagNode* >::const_iterator iter = m_nodes.begin(); iter != m_nodes.end(); ++iter )
	{ 
		for ( UInt32 i = 0; i < (*iter)->path.childCount(); ++i )
		{
			MFnDagNode dagNode( (*iter)->path.child( i ) );
			MDagPath path;
			dagNode.getPath( path );
						
			if ( DagNode* node = findNode( path ) )
				(*iter)->addChild( node );
		}
	}

	
	return doWrite( filename );
}

FileWriter::DagNode* FileWriter::findNode( const MDagPath& path )
{
	for ( std::vector< DagNode* >::const_iterator i = m_nodes.begin(); i != m_nodes.end(); ++i )
	{
		if ( path == (*i)->path && 
			(!path.isInstanced() || path.instanceNumber() == (*i)->path.instanceNumber()) )
		{
				return (*i);
		}
	}
	return 0;
}