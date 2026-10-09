/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\KismoMayaPlugin\Source\FileWriter.h
 * Status: superseded by kismo-v0.3/Kismo_MayaPlugin/FileWriter.h (same name, content differs)
 */
#ifndef _FILEWRITER_H
#define _FILEWRITER_H

#include "framework/KismoDagNode.h"
#include <maya/MDagPath.h>
using namespace kismo;

class FileWriter
{
protected:

	enum ke_NodeType
	{
		kc_NodeType_Transform = 1 << 0,
		kc_NodeType_Mesh = 1 << 1,
		kc_NodeType_Material = 1 << 2,
		kc_NodeType_Light = 1 << 3,
		kc_NodeType_Texture = 1 << 4,
		kc_NodeType_LODGroup = 1 << 5,
		kc_NodeType_Camera = 1 << 6,
		kc_NodeType_Joint = 1 << 7,
		//kc_NodeType_SkinCluster = 1 << 8,
	};

	class DagNode : public kismo::framework::DagNode< DagNode >
	{
	public:
		DagNode( MDagPath& path, k_UInt32 type ) :
			path( path ),
			type( type )
		{
		}

		const MDagPath path;
		const k_UInt32 type; //Type of node to write e.g. transform
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