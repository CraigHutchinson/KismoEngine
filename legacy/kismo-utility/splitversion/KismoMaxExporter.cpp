/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo Utility\splitversion\KismoMaxExporter.cpp
 * Status: current (newest copy of this file name in the archive)
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

		//////////////////////////////////////////////////////////////////////////
		// Output Utilities

		// Our output section.  Because we are a simple text writer,
		// we are simply going to pipe our messages to a text file.
		FILE* m_pFile;

		// Initialize our out stream.  This function must be
		// called before commencing the export process
		BOOL BeginWriting(const char* pPath);

		BOOL exportSelected;

		// New output method
		ofstream ExportFile;
		BOOL BeginWriting2(const std::string& name);
		void KismoMaxExporter::EndWriting2();

		// Finalize our out stream.  This function must be
		// called before finishing the export process
		void EndWriting();

		// A simple printf wrapper function to encapsulate
		// writing to the file.
		void Write(int indent, const char* pMsg, ...);

		// Write out the given transform in terms
		// of its position, rotation, and scale parts
		void Write(int indent, const Matrix3& tmVal);

		// write out the given value after the message
		void WriteValue(int indent, const char* pMsg, float val);
		void WriteValue(int indent, const char* pMsg, const Point3& val);
		void WriteValue(int indent, const char* pMsg, const Quat& val);
		void WriteValue(int indent, const char* pMsg, const AngAxis& val);
		void WriteValue(int indent, const char* pMsg, const ScaleValue& val);

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


INT_PTR CALLBACK KismoMaxExporterOptionsDlgProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam) {
	static KismoMaxExporter *imp = NULL;

	switch(message) {
		case WM_INITDIALOG:
			imp = (KismoMaxExporter *)lParam;
			CenterWindow(hWnd,GetParent(hWnd));
			return TRUE;

		case WM_CLOSE:
			EndDialog(hWnd, 0);
			return 1;
	}
	return 0;
}


//--- KismoMaxExporter -------------------------------------------------------
KismoMaxExporter::KismoMaxExporter() : m_pFile(0)
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
	/* #pragma message(TODO("Implement the actual file Export here and"))

	if(!suppressPrompts)
		DialogBoxParam(hInstance, 
				MAKEINTRESOURCE(IDD_PANEL), 
				GetActiveWindow(), 
				KismoMaxExporterOptionsDlgProc, (LPARAM)this); */


	//NOTE: Need to check this is the correct code but hopefully it should determine if the user selected export selected object in which case we
	//perform different export code

	exportSelected = (options & SCENE_EXPORT_SELECTED);

	/*if (BeginWriting(name))
	{

		// Interface* can also be accessed by GetCOREInterface()
		INode* pRootNode = i->GetRootNode();

		//Export tree.
		Export(pRootNode);

		EndWriting();

		return TRUE; // We have successfully exported to our file!
	}*/

	if (BeginWriting2(name))
	{

		//Export Stuff

		ExportFile << "This is output!" << endl;

		EndWriting2();

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

	if (!exportSelected || pNode->Selected()) 
	{			

		// Write out this node as [NodeName] : XX Children
		Write(iTreeDepth, "[%s] : %i children", pNodeName, nChildren);

		// Write out this nodes local transform
		ExportPRS(pNode, iTreeDepth);

		// Write out the color information for this node.
		ExportMaterial(pNode, iTreeDepth);

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
		Write(iTreeDepth, "Object Color: 0x%X", dwNodeColor);
	}
	else
	{
		// We have a material.  Lets export the basic parameters
		const MSTR& sMtlName = pMtl->GetName();
		Write(iTreeDepth, "Object Material: %s", sMtlName.data());
		WriteValue(iTreeDepth, "   Ambient: ", pMtl->GetAmbient());
		WriteValue(iTreeDepth, "   Diffuse: ", pMtl->GetDiffuse());
		WriteValue(iTreeDepth, "   Specular: ", pMtl->GetSpecular());
		WriteValue(iTreeDepth, "   Shininess: ", pMtl->GetShininess());
		WriteValue(iTreeDepth, "   Shininess Strength: ", pMtl->GetShinStr());

		if (pMtl->GetSelfIllumColorOn())
			WriteValue(iTreeDepth, "   Self Illumination: ", pMtl->GetSelfIllumColor());
		else 
			WriteValue(iTreeDepth, "   Self Illumination: ", pMtl->GetSelfIllum());

		// We have written out the very basic properties for a material.
		// However, there are some more properties missing from this implementation
		// that we could use.  Here we test to see if we have the standard material
		// and export some of its more 'advanced' properties
		if (pMtl->IsSubClassOf(Class_ID(DMTL_CLASS_ID, 0)))
		{
			StdMat* pStdMtl = (StdMat*)pMtl;
			WriteValue(iTreeDepth, "   Opacity: ", pStdMtl->GetOpacity(0));
			WriteValue(iTreeDepth, "   Opacity Falloff: ", pStdMtl->GetOpacFalloff(0));
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
	Write(iTreeDepth, "%s Tex: %s", pSlotName, sTexClass.data());

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

		Write(iTreeDepth, "   path: \"%s\"", pBmpTex->GetMapName());
		Write(iTreeDepth, "   U Tile = %.1f, V Tile = %.1f\n", fUTile, fVTile);
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
		Write(iTreeDepth, "Obj SuperclassID : %X", lSuperID);
		Write(iTreeDepth, "Obj ClassID : (%X, %X)", lClassID.PartA(), lClassID.PartB());

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
	Write(iTreeDepth, "Number of Vertices: %i", nVerts);

	// Write the vertices positions
	for (int i = 0; i < nVerts; i++)
	{
		Point3& vtx = lMesh.getVert(i);
		Write(iTreeDepth, "Vertex[%02i] : (%f,\t%f,\t%f)", i, vtx.x, vtx.y, vtx.z);
	}

	// Write out our triangle indices
	int nTris = lMesh.getNumFaces();
	Write(iTreeDepth, "Number of Triangles : %i", nTris);

	for (int i = 0; i < nTris; i++)
	{
		// Mesh has a public property, lFace,
		// that we use to access face data
		Face& lFace = lMesh.faces[i];
		// Retrieves a pointer to the index array
		DWORD* pIndices = lFace.getAllVerts();

		Write(iTreeDepth, "Face[%02i], Vertex IDs : (%u, %u, %u)",
					i, pIndices[0], pIndices[1], pIndices[2]);
	}
}

void KismoMaxExporter::ExportPRS(INode* pNode, int iTreeDepth)
{
	// Export the initial state, that is, at time 0
	Matrix3 tmWorld = pNode->GetNodeTM(0);
	Matrix3 tmParent = pNode->GetParentTM(0);
	Matrix3 tmLocal = tmWorld * Inverse(tmParent);

	Write(iTreeDepth, tmLocal);
}

//////////////////////////////////////////////////////////////////////////
// New Writing Utility Functions

BOOL KismoMaxExporter::BeginWriting2(const std::string& name)
{
	ExportFile.open (name.c_str());
	
	if (ExportFile.is_open()){	
		return TRUE;
	}
	else {return FALSE;}
}

void KismoMaxExporter::EndWriting2()
{
	ExportFile.close();
}

//////////////////////////////////////////////////////////////////////////
// Writing utility functions

BOOL KismoMaxExporter::BeginWriting(const char *pPath)
{
	// Check we are not currently writing
	DbgAssert(m_pFile == NULL);

	errno_t lErr = fopen_s(&m_pFile, pPath, "w");
	return lErr == 0;
}

void KismoMaxExporter::EndWriting()
{
	DbgAssert(m_pFile != NULL);

	fclose(m_pFile);
	m_pFile = NULL;
}

void KismoMaxExporter::Write(int indent, const char* pMsg, ...)
{
	DbgAssert(m_pFile != NULL);

	// first, write in the indent
	for (int i = 0; i < indent; i++) fprintf_s(m_pFile, "\t");

	// Write the message, passing in our variable
	// parameters to a function that wraps printf
	va_list vargs;
	va_start(vargs, pMsg);
	vfprintf_s(m_pFile, pMsg, vargs);
	va_end(vargs);

	// Finish the line
	fprintf_s(m_pFile, "\n");
}

void KismoMaxExporter::Write(int indent, const Matrix3 &tmVal)
{
	AffineParts apVal;
	float dRotEuler[3];

	decomp_affine(tmVal, &apVal);

	// Convert the rotation quaternion in apVal to rotation euler
	apVal.q.GetEuler(&dRotEuler[0], &dRotEuler[1], &dRotEuler[2]);

	// Write out the position
	Write(indent, "PRS: P (%f, %f, %f)", apVal.t.x, apVal.t.y, apVal.t.z);
	// The Euler rotation
	Write(indent, "     R (%f, %f, %f)", dRotEuler[0], dRotEuler[1], dRotEuler[2]);
	// And finally the scale
	Write(indent, "     S (%f, %f, %f)", apVal.k.x, apVal.k.y, apVal.k.z);
}

void KismoMaxExporter::WriteValue(int indent, const char* pMsg, float val)
{
	Write(indent, "%s%f", pMsg, val);
}

void KismoMaxExporter::WriteValue(int indent, const char* pMsg, const Point3& val)
{
	Write(indent, "%sPoint3(%f, %f, %f)", pMsg, val.x, val.y, val.z);
}

void KismoMaxExporter::WriteValue(int indent, const char *pMsg, const Quat &val)
{
	// Convert the rotation quaternion into a rotation Euler
	// this is simply because it is easier for us to read in ASCII
	float dRotEuler[3];
	val.GetEuler(&dRotEuler[0], &dRotEuler[1], &dRotEuler[2]);

	Write(indent, "%sEuler(%f, %f, %f)", pMsg, dRotEuler[0], dRotEuler[1], dRotEuler[2]);
}

void KismoMaxExporter::WriteValue(int indent, const char* pMsg, const AngAxis& val)
{
	// Keep our rotations consistent, convert to Quat and write that
	Quat q(val);
	WriteValue(indent, pMsg, q);
}

void KismoMaxExporter::WriteValue(int indent, const char *pMsg, const ScaleValue &val)
{
	Write(indent, "%sScale(%f, %f, %f)", pMsg, val.s.x, val.s.y, val.s.z);
}