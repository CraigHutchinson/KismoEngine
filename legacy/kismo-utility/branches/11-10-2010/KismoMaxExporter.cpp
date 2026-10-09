/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo Utility\branches\11-10-2010\KismoMaxExporter.cpp
 * Status: superseded by kismo-utility/splitversion/KismoMaxExporter.cpp (same name, content differs)
 */
//**************************************************************************/
// Copyright (c) 1998-2007 Autodesk, Inc.
// All rights reserved.
// 
// These coded instructions, statements, and computer programs contain
// unpublished proprietary information written by Autodesk, Inc., and are
// protected by Federal copyright law. They may not be disclosed to third
// parties or copied or duplicated in any form, in whole or in part, without
// the prior written consent of Autodesk, Inc.
//**************************************************************************/
// DESCRIPTION: Appwizard generated plugin
// AUTHOR: 
//***************************************************************************/

#include "KismoMaxExporter.h"
#include "KismoMaxExporterFH.h"
#include "KismoMaxExporterOpts.h"
#include <decomp.h>
#include <stdmat.h>
#include <fstream>

using namespace std;

#define KismoMaxExporter_CLASS_ID	Class_ID(0x7b5d837d, 0x85787add)

class KismoMaxExporter : public SceneExport {
	public:
		
		static HWND hParams;
		
		int				ExtCount();					// Number of extensions supported
		const TCHAR *	Ext(int n);					// Extension #n (i.e. "3DS")
		const TCHAR *	LongDesc();					// Long ASCII description (i.e. "Autodesk 3D Studio File")
		const TCHAR *	ShortDesc();				// Short ASCII description (i.e. "3D Studio")
		const TCHAR *	AuthorName();				// ASCII Author name
		const TCHAR *	CopyrightMessage();			// ASCII Copyright message
		const TCHAR *	OtherMessage1();			// Other message #1
		const TCHAR *	OtherMessage2();			// Other message #2
		unsigned int	Version();					// Version number * 100 (i.e. v3.01 = 301)
		void			ShowAbout(HWND hWnd);		// Show DLL's "About..." box

		BOOL SupportsOptions(int ext, DWORD options);
		int				DoExport(const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts=FALSE, DWORD options=0);

		//Constructor/Destructor
		KismoMaxExporter();
		~KismoMaxExporter();

		
	private:

		// Instance of the export file class
		KismoMaxExporterFH m_exportfile;
		// Instance of the options class
		KismoMaxExporterOpts m_Opts;

		//////////////////////////////////////////////////////////////////////////
		// Export max entities

		// Export a Max Node.  This recursive function will iterate
		// the max scene tree, and export the interesting information
		// from each node visited.
		void Export(INode* pNode, int iTreeDepth = 0);

		// Export the material off the passed node.  If the
		// node has no material, export the basic object color
		void ExportMaterial(INode* pNode, int iTreeDepth);

		// Export all the textures applied to the given material
		void ExportTextures(Mtl* pMtl, int iTreeDepth);

		// Export the given texture map.  If the texture is
		// a bitmap, export its path and UV parameters
		void ExportTexture(Texmap* pTex, const char* pSlotName, int iTreeDepth);

		// Try and export the object from this node.
		// If we cannot convert the object to something
		// we understand, just write out the object ID's
		void ExportObject(INode* pNode, int iTreeDepth);

		// Export the geometry information contained by TriObject.
		// For a start this will be just the vertices and triangle indices
		void ExportObject(TriObject* pTriObject, int iTreeDepth);

		// Export the local transform of the given node
		void ExportPRS(INode* pNode, int iTreeDepth);

		bool exportSelected;

};

class KismoMaxExporterClassDesc : public ClassDesc2 
{
public:
	virtual int IsPublic() 							{ return TRUE; }
	virtual void* Create(BOOL /*loading = FALSE*/) 		{ return new KismoMaxExporter(); }
	virtual const TCHAR *	ClassName() 			{ return GetString(IDS_CLASS_NAME); }
	virtual SClass_ID SuperClassID() 				{ return SCENE_EXPORT_CLASS_ID; }
	virtual Class_ID ClassID() 						{ return KismoMaxExporter_CLASS_ID; }
	virtual const TCHAR* Category() 				{ return GetString(IDS_CATEGORY); }

	virtual const TCHAR* InternalName() 			{ return _T("KismoMaxExporter"); }	// returns fixed parsable name (scripter-visible name)
	virtual HINSTANCE HInstance() 					{ return hInstance; }					// returns owning module handle
	

};

ClassDesc2* GetKismoMaxExporterDesc() { 
	static KismoMaxExporterClassDesc KismoMaxExporterDesc;
	return &KismoMaxExporterDesc; 
}


//--- KismoMaxExporter -------------------------------------------------------
KismoMaxExporter::KismoMaxExporter()
{
}

KismoMaxExporter::~KismoMaxExporter()
{

}

int KismoMaxExporter::ExtCount()
{
	//No. of file extensions supported by plugin.
	return 1;
}

const TCHAR *KismoMaxExporter::Ext(int n)
{		
	//Output file extension
	return _T("txt");
}

const TCHAR *KismoMaxExporter::LongDesc()
{
	return _T("KismoEngine Model File");
}
	
const TCHAR *KismoMaxExporter::ShortDesc() 
{			
	return _T("Kismo");
}

const TCHAR *KismoMaxExporter::AuthorName()
{			
	return _T("Steve Gordon");
}

const TCHAR *KismoMaxExporter::CopyrightMessage() 
{	
	return _T("Copyright 2010 LimboGroup - Working Title");
}

const TCHAR *KismoMaxExporter::OtherMessage1() 
{		
	//TODO: Return Other message #1 if any
	return _T("");
}

const TCHAR *KismoMaxExporter::OtherMessage2() 
{		
	//TODO: Return other message #2 in any
	return _T("");
}

unsigned int KismoMaxExporter::Version()
{				
	return 100;
}

void KismoMaxExporter::ShowAbout(HWND hWnd)
{			
	// Optional
}

BOOL KismoMaxExporter::SupportsOptions(int ext, DWORD options)
{
	#pragma message(TODO("Decide which options to support.  Simply return true for each option supported by each Extension the exporter supports."))
	return TRUE;
}

int	KismoMaxExporter::DoExport(const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts, DWORD options)
{
	// /Show our UI.  Allow user to cancel
	if (!suppressPrompts && !m_Opts.ShowDialog())
	{
		// User cancelled, return TRUE
		// Otherwise Max displays export failed dialog
		return TRUE;
	}

	// If the user selects to export the selected item this value will be true.
	exportSelected = (options & SCENE_EXPORT_SELECTED);

	// NOTE: Should consider initialising the file via the constructor / destructor?
	if (m_exportfile.KismoBeginWriting(name))
	{
		// Interface* can also be accessed by GetCOREInterface()
		INode* pRootNode = i->GetRootNode();

		//Export tree.
		Export(pRootNode);

		m_exportfile.KismoEndWriting();	
		return TRUE;
	}
	return FALSE;
}

//////////////////////////////////////////////////////////////////////////
// Max Entity Export

void KismoMaxExporter::Export(INode* pNode, int iTreeDepth /* = 0 */)
{

	// First step to export: What are we exporting?
	// Here we are going to export the basic scene tree,
	// identifying each node by name.

	MCHAR* pNodeName = pNode->GetName();
	int nChildren = pNode->NumberOfChildren();

	// Perform checks to see if we want to export the current node
	if (!exportSelected || pNode->Selected()) 
	{			

		// Use a stringstream to build the output string for the node information
		stringstream of;
		of << "[";
		of << pNodeName;
		of << "] : ";
		of << nChildren;
		of << " Children";

		// Write out this node as [NodeName] : XX Children
		m_exportfile.KismoWrite(iTreeDepth, of.str());

		// Write out this nodes local transform
		ExportPRS(pNode, iTreeDepth);

		// Write out the color information for this node.
		if (m_Opts.ExportMaterials()) ExportMaterial(pNode, iTreeDepth);
		
		// Write out this nodes object properties
		ExportObject(pNode, iTreeDepth);
	}
		
	// Recursively call the children
	iTreeDepth++;
	for (int i = 0; i < nChildren; i++)
	{
		Export(pNode->GetChildNode(i), iTreeDepth);
	}
}

void KismoMaxExporter::ExportMaterial(INode* pNode, int iTreeDepth)
{
	// The node stores the material for the object
	Mtl* pMtl = pNode->GetMtl();
	if (pMtl == NULL)
	{
		// This case is perfectly valid.  For simple scenes,
		// a node can specify a color for an object without
		// using the overhead of an entire material
		DWORD dwNodeColor = pNode->GetWireColor();
		//m_exportfile.KismoWrite(iTreeDepth, "Object Color: ", dwNodeColor);
	}
	else
	{
		// We have a material.  Lets export the basic parameters
		const MSTR& sMtlName = pMtl->GetName();
		//m_exportfile.KismoWrite(iTreeDepth, "Object Material: ", sMtlName.data());
		//WriteValue(iTreeDepth, "   Ambient: ", pMtl->GetAmbient());
		//WriteValue(iTreeDepth, "   Diffuse: ", pMtl->GetDiffuse());
		//WriteValue(iTreeDepth, "   Specular: ", pMtl->GetSpecular());
		//WriteValue(iTreeDepth, "   Shininess: ", pMtl->GetShininess());
		//WriteValue(iTreeDepth, "   Shininess Strength: ", pMtl->GetShinStr());

		if (pMtl->GetSelfIllumColorOn())
			//WriteValue(iTreeDepth, "   Self Illumination: ", pMtl->GetSelfIllumColor());
			;
		else ;
			//WriteValue(iTreeDepth, "   Self Illumination: ", pMtl->GetSelfIllum());

		// We have written out the very basic properties for a material.
		// However, there are some more properties missing from this implementation
		// that we could use.  Here we test to see if we have the standard material
		// and export some of its more 'advanced' properties
		if (pMtl->IsSubClassOf(Class_ID(DMTL_CLASS_ID, 0)))
		{
			StdMat* pStdMtl = (StdMat*)pMtl;
			//WriteValue(iTreeDepth, "   Opacity: ", pStdMtl->GetOpacity(0));
			//WriteValue(iTreeDepth, "   Opacity Falloff: ", pStdMtl->GetOpacFalloff(0));
		}

		// Export this materials textures
		ExportTextures(pMtl, iTreeDepth);
	}
}

void KismoMaxExporter::ExportTextures(Mtl* pMtl, int iTreeDepth)
{
	// Iterate all the textures this mtl could possibly
	// have assigned.  If a texture is assigned, export it
	for (int i = 0; i < pMtl->NumSubTexmaps(); i++)
	{
		Texmap* pTex = pMtl->GetSubTexmap(i);
		if (pTex != NULL)
		{
			MSTR sSlotName = pMtl->GetSubTexmapSlotName(i);
			ExportTexture(pTex, sSlotName.data(), iTreeDepth + 1);
		}
	}
}

void KismoMaxExporter::ExportTexture(Texmap* pTex, const char* pSlotName, int iTreeDepth)
{
	// There are many different types of textures in Max
	// Which one have we got here?
	MSTR sTexClass;
	pTex->GetClassName(sTexClass);
	//Write(iTreeDepth, "%s Tex: %s", pSlotName, sTexClass.data());

	// If our texture is an image file, write out its path
	if (pTex->ClassID() == Class_ID(BMTEX_CLASS_ID, 0))
	{
		BitmapTex* pBmpTex = (BitmapTex*) pTex;
		StdUVGen* pUVGen = pBmpTex->GetUVGen();

		// Note to reader: All these parameters could
		// be animated!  Here we only export at time
		// 0 for convenience, in production we must be careful
		// to take care of exporting values at the correct time
		float fUTile = pUVGen->GetUScl(0);
		float fVTile = pUVGen->GetVScl(0);

		//Write(iTreeDepth, "   path: \"%s\"", pBmpTex->GetMapName());
		//Write(iTreeDepth, "   U Tile = %.1f, V Tile = %.1f\n", fUTile, fVTile);
	}
}

void KismoMaxExporter::ExportObject(INode* pNode, int iTreeDepth)
{
	// Get a pointer to this nodes object reference
	Object* pObject = pNode->GetObjectRef();

	// the root node for example, doesnt have an object
	if (pObject != NULL)
	{
		SClass_ID lSuperID = pObject->SuperClassID();
		Class_ID lClassID = pObject->ClassID();

		// Write out our class types
		// See also SDK documentation for "list of Super Class IDs"

		m_exportfile.KismoWrite(iTreeDepth, "Obj SuperClassID : ", lSuperID);

		stringstream superclass;
		superclass << "Obj ClassID : (" << lClassID.PartA() << ", " << lClassID.PartB() << ")";
		m_exportfile.KismoWrite(iTreeDepth, superclass.str());

		// Is it possible that we can convert this object to
		// something we understand?  Here we try converting
		// to a TriObject, using its Class_ID
		if (pObject->CanConvertToType(triObjectClassID))
		{
			TriObject* pTriObject = (TriObject*)pObject->ConvertToType(0, triObjectClassID);
			if (pTriObject != NULL)
			{
				// Write out the mesh indented
				ExportObject(pTriObject, iTreeDepth + 1);

				// Note that if the TriObject pointer does not
				// point to the the original object, it is a new
				// instance that we are now responsible for
				if (pTriObject != pObject)
				{
					// Never call delete directly on any max class
					pTriObject->DeleteMe();
				}
			}
		}
	}
}

void KismoMaxExporter::ExportObject(TriObject* pTriObj, int iTreeDepth)
{
	// See the SDK Documentation for "PG: Working With Meshes"

	// Get the data container
	Mesh& lMesh = pTriObj->GetMesh();

	// How many vertices?
	int nVerts = lMesh.getNumVerts();
	m_exportfile.KismoWrite(iTreeDepth, "Number of Vertices: ", nVerts);

	// Write the vertices positions
	for (int i = 0; i < nVerts; i++)
	{
		Point3& vtx = lMesh.getVert(i);
		m_exportfile.KismoWrite(iTreeDepth, "Vertex", i, vtx.x, vtx.y, vtx.z);
	}

	// Write out our triangle indices
	int nTris = lMesh.getNumFaces();
	m_exportfile.KismoWrite(iTreeDepth, "Number of Triangles : ", nTris);

	for (int i = 0; i < nTris; i++)
	{
		// Mesh has a public property, lFace,
		// that we use to access face data
		Face& lFace = lMesh.faces[i];
		// Retrieves a pointer to the index array
		DWORD* pIndices = lFace.getAllVerts();

		m_exportfile.KismoWrite(iTreeDepth, "Face", i,  pIndices[0], pIndices[1],  pIndices[2]);
	}
}

void KismoMaxExporter::ExportPRS(INode* pNode, int iTreeDepth)
{
	// Export the initial state, that is, at time 0
	Matrix3 tmWorld = pNode->GetNodeTM(0);
	Matrix3 tmParent = pNode->GetParentTM(0);
	Matrix3 tmLocal = tmWorld * Inverse(tmParent);

	AffineParts apVal;
	float dRotEuler[3];

	decomp_affine(tmLocal, &apVal);

	// Convert the rotation quaternion in apVal to rotation euler
	apVal.q.GetEuler(&dRotEuler[0], &dRotEuler[1], &dRotEuler[2]);

	// Build the stringstreams
	stringstream prsp, prsr, prss;
	prsp << "PRS: P (" << apVal.t.x << ", " << apVal.t.y << ", " << apVal.t.z << ")";
	prsr << "     R (" << dRotEuler[0] << ", " << dRotEuler[1] << ", " << dRotEuler[2] << ")";
	prss << "     S (" << apVal.k.x << ", " << apVal.k.y << ", " << apVal.k.z << ")";

	// Write out the PRS Data
	m_exportfile.KismoWrite(iTreeDepth, prsp.str());
	m_exportfile.KismoWrite(iTreeDepth, prsr.str());
	m_exportfile.KismoWrite(iTreeDepth, prss.str());
}