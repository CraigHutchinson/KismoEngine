/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\KismoMayaPlugin\Source\KismoAscii_FileWriter.h
 * Status: superseded by kismo-v0.3/Kismo_MayaPlugin/KismoAscii_FileWriter.h (same name, content differs)
 */
#ifndef _KISMOASCII_FILEWRITER_H
#define _KISMOASCII_FILEWRITER_H

#include "FileWriter.h"
#include <fstream>
#include <iomanip>
#include <maya/MGlobal.h>
#include <maya/MFnTransform.h>
#include <maya/MVector.h>
#include <maya/MEulerRotation.h>
#include <maya/MFnMesh.h>
#include <maya/MFloatArray.h>
#include <maya/MFnAttribute.h>
#include <maya/MFloatPointArray.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MObjectArray.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MItDependencyNodes.h>
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>
#include <maya/MFnDoubleArrayData.h>
#include <maya/MFnNumericData.h>
#include <maya/MFnLambertShader.h>

class KismoAscii_FileWriter : public FileWriter
{
public:
	KismoAscii_FileWriter();
	virtual ~KismoAscii_FileWriter();

	static FileWriter* create() { return new KismoAscii_FileWriter; }

protected:

	//Write to the file stream
	MStatus doWrite( const MString& filename );

	void incrementDepth() { m_indent += '\t'; }
	void decrementDepth() {m_indent = m_indent.substr( 0, m_indent.length() - 1 ); }
	//---------------------------
	// Hierarchy
	//---------------------------
	void writeHierarchy();
	void writeNode( DagNode* node );
	void writeChildNodes( DagNode* node );

	void writeTransformNode( DagNode* node  );
	void writeLodGroupNode( DagNode* node );
	void writeMeshNode( DagNode* node, bool firstPass );
		void writeUVSets( MFnMesh& mesh  );
		void writeVertices( MFnMesh& mesh  );
		void writeNormals( MFnMesh& mesh  );
		void writeFaces( MFnMesh& mesh  );
	//void writeSkinCluster( DagNode* node  );
	void writeJointNode( DagNode* node  );
	
	//---------------------------
	// Material data
	//---------------------------
	void writeMaterials(); ///< Write all the material data
	void writeMaterial( MFnLambertShader& mat ); ///< Write a material

	//---------------------------
	// Mesh data
	//---------------------------
	void writeMeshes(); ///< Write all the mesh data in priority order
	void writeSkinClusters(); ///< Write all the mesh skinning clusters

	//---------------------------
	// helper functions
	//---------------------------
	void writeTransformationData( DagNode* node ); //Writes the data of a transform

protected:

	bool getTextureFile( MFnDependencyNode& fn, const k_String& name, k_String& outFilename );

private:
	k_OFStream m_ofile; ///< The output file stream
	k_String m_indent; //The indentfor the current node

	k_Priority m_currentMeshPriority; ///< The current priority for the next msh that is added
	k_Int8 m_priorityStep;//the priority change per mesh
	typedef k_MultiMap< k_Priority, k_String > MESH_PRIORITY_MAP;
	MESH_PRIORITY_MAP m_meshNamesByPriority; ///< Map of meshes ordered by the priority

	typedef k_Map< k_String, DagNode* > MESH_NAME_MAP;
	MESH_NAME_MAP m_meshsByName; ///< Map of meshes ordered by the priority

	//typedef k_Map< k_String, DagNode* > MESH_NAME_MAP;
	//MESH_NAME_MAP m_meshsByName; ///< Map of meshes ordered by the priority


};

#endif