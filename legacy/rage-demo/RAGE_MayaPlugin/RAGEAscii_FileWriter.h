/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\RAGE_MayaPlugin\RAGEAscii_FileWriter.h
 * Status: unique (no other copy of this file name in the archive)
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

class RAGEAscii_FileWriter : public FileWriter
{
public:
	RAGEAscii_FileWriter();
	virtual ~RAGEAscii_FileWriter();

	static FileWriter* create() { return new RAGEAscii_FileWriter; }

protected:

	//Write to the file stream
	MStatus doWrite( const MString& filename );

	void incrementDepth() 
	{ m_indent += '\t'; }

	void decrementDepth() 
	{ m_indent = m_indent.substr( 0, m_indent.length() - 1 ); }

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
		void writePositions( MFnMesh& mesh  );
		void writeNormals( MFnMesh& mesh  );
		void writeFaces( MFnMesh& mesh  );
	//void writeSkinCluster( DagNode* node  );
	void writeJointNode( DagNode* node  );
	
	//---------------------------
	// Dynamics data
	//---------------------------
	void writePhysSceneNode( DagNode* node );
	void writeDynamicsNode( DagNode* node );
	void writeRigidBodyActorNode( DagNode* node, bool writeActiveBody = true );
	void writeRigidBodyNode( DagNode* node );
	void writePhysMeshNode( DagNode* node );
	void writeDynamicConstraintNode( DagNode* node );

	//---------------------------
	// Shape data
	//---------------------------	
	void writeBoxShape( DagNode* node  );
	void writeSphereShape( DagNode* node  );
	void writeCylinderShape( DagNode* node  );
	void writePlaneShape( DagNode* node  );

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

	bool getTextureFile( MFnDependencyNode& fn, const String& name, String& outFilename );

private:
	OutFStream m_ofile; ///< The output file stream
	String m_indent; //The indentfor the current node

	Priority m_currentMeshPriority; ///< The current priority for the next msh that is added
	Int8 m_priorityStep;//the priority change per mesh
	typedef MultiMap< Priority, String > MESH_PRIORITY_MAP;
	MESH_PRIORITY_MAP m_meshNamesByPriority; ///< Map of meshes ordered by the priority

	typedef Map< String, DagNode* > MESH_NAME_MAP;
	MESH_NAME_MAP m_meshsByName; ///< Map of meshes ordered by the priority

	//typedef Map< String, DagNode* > MESH_NAME_MAP;
	//MESH_NAME_MAP m_meshsByName; ///< Map of meshes ordered by the priority


};

#endif