/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\Kismo_MayaPlugin\FileWriter.h
 * Status: current (newest copy of this file name in the archive)
 */
#ifndef _FILEWRITER_H
#define _FILEWRITER_H

#include <maya/MDagPath.h>

#include "Framework/GraphNode.h"
#include "Framework/Vector.h"
#include "Framework/BoundingSphere.h"

typedef frx::Vector3< Real > Vector3;
typedef frx::BoundingSphere< Real > BoundingSphere;

class FileWriter
{
protected:

	enum eNodeType
	{
		NodeType_Transform = 1 << 0,
		NodeType_Mesh = 1 << 1,
		NodeType_Material = 1 << 2,
		NodeType_Light = 1 << 3,
		NodeType_Texture = 1 << 4,
		NodeType_LODGroup = 1 << 5,
		NodeType_Camera = 1 << 6,
		NodeType_Joint = 1 << 7,
		NodeType_DynamicsNode = 1 << 8, ///< for nodes named 'collision'
		NodeType_RigidBody = 1 << 9,
		NodeType_RigidConstraint = 1 << 10,
		NodeType_Field = 1 << 11,
		//NodeType_SkinCluster = 1 << 8,
	};

	class DagNode : public frx::GraphNode< DagNode >
	{
	public:
		DagNode( MDagPath& path, UInt32 type ) :
			path( path ),
			type( type )
		{
		}

		const MDagPath path;
		const UInt32 type; //Type of node to write e.g. transform
	};

public:
	FileWriter();
	virtual ~FileWriter();

	bool addNode( MDagPath path );

	MStatus write( const MString& filename );
	DagNode* findNode( const MDagPath& path );

protected:

	//Write to the file stream
	virtual MStatus doWrite( const MString& filename ) = 0;

	std::vector< DagNode* > m_nodes; //pointers to all the nodes
	DagNode* m_root;


};

#endif