/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\KismoMayaPlugin\Source\FileWriter.cpp
 * Status: superseded by kismo-v0.3/Kismo_MayaPlugin/FileWriter.cpp (same name, content differs)
 */
#include "FileWriter.h"
#include "maya/MFnDagNode.h"

FileWriter::FileWriter() {}
FileWriter::~FileWriter() 
{
	for ( std::vector< DagNode* >::const_iterator i = m_nodes.begin(); i != m_nodes.end(); ++i )
		safeDelete( *i );
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

	//If we got here then the node  has not already been added
	k_UInt32 typeMask = 0;
	if ( path.hasFn( MFn::kMesh ) ) typeMask |= kc_NodeType_Mesh;
	if ( path.hasFn( MFn::kTransform ) ) typeMask |= kc_NodeType_Transform;
	if ( path.hasFn( MFn::kMaterial ) ) typeMask |= kc_NodeType_Material;
	if ( path.hasFn( MFn::kLight ) ) typeMask |= kc_NodeType_Light;
	if ( path.hasFn( MFn::kCamera ) ) typeMask |= kc_NodeType_Camera;
	if ( path.hasFn( MFn::kLodGroup ) ) typeMask |= kc_NodeType_LODGroup;
	if ( path.hasFn( MFn::kJoint ) ) typeMask |= kc_NodeType_Joint;
	//if ( path.hasFn( MFn::kSkinClusterFilter  ) ) typeMask |= kc_NodeType_SkinCluster;

	DagNode* newNode = new DagNode( path, typeMask );
	
	m_nodes.push_back( newNode );

	return true;
}

MStatus FileWriter::write( const MString& filename )
{
	//Construc the node tree
	for ( std::vector< DagNode* >::const_iterator iter = m_nodes.begin(); iter != m_nodes.end(); ++iter )
	{ 
		for ( k_UInt32 i = 0; i < (*iter)->path.childCount(); ++i )
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