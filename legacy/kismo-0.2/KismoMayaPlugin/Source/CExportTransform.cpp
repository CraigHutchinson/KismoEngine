/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\KismoMayaPlugin\Source\CExportTransform.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
#include "CExportTransform.h"

#include <maya/MTransformationMatrix.h>
#include <maya/MEulerRotation.h>
#include <maya/MVector.h>

bool CExportTransform::export( MFnTransform& fnTransform, fstream& outFile )
{
	MStatus status;

	MObject lightObject = fnTransform.object();

	MTransformationMatrix trans = fnTransform.transformation();

	//Get the objects translation/position
	MVector pos = trans.translation( MSpace::kWorld );
	outFile << "\t<position>" << pos.x << " " << pos.y << " " << pos.z << "</position>\n";

	//get the objects rotation
	MEulerRotation rot = trans.eulerRotation();	
	outFile << "\t<rotation>" << rot.x << " " << rot.y << " " << rot.z << "</rotation>\n";

	cerr<< "Transform exported\n";

	return true;
}
