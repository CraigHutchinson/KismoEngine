/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\KismoMayaPlugin\Source\CExportTransform.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef CTRANSFORMEXPORT_H
#define	CTRANSFORMEXPORT_H

#include <maya/MFnTransform.h>
#include <maya/MDagPath.h>

#include <fstream>
#include <iostream>

using std::cerr;
using std::cout;
using std::fstream;


class CExportTransform
{
public:
	CExportTransform(){};

	bool export( MFnTransform& fnTransform, fstream& outFile );

};

#endif