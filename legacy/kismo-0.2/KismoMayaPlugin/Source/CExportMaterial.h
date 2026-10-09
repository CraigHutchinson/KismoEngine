/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\KismoMayaPlugin\Source\CExportMaterial.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef CMATERIALEXPORT_H
#define	CMATERIALEXPORT_H

#include <fstream>
#include <iostream>

using std::cerr;
using std::cout;
using std::fstream;

#include <maya/MObject.h>
#include <maya/MFnDependencyNode.h>

#include "CString.h"

enum eImageFormat
{
	eAsOriginal,
	eTarga,
	eBitmap
};

enum eOverwite
{
	eYes = 1,
	eNo,
	ePrompt
};

class CExportMaterial
{
public:
	CExportMaterial(){};

	bool export( MObject& objMtrl, fstream& outFile );

	bool getTextureFile( MFnDependencyNode& fn, const MString& name, MString& outFilename );

protected:

	//Include the full image path?
	bool bFullPaths;

	//Sets whether to do converting/copying of images or not
	bool bCopyImages;

	//Image settings
	CString imageFolder;
	eImageFormat imageFormat;

	eOverwite overWriteImage;
};

#endif