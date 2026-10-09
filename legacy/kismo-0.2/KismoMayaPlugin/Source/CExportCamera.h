/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\KismoMayaPlugin\Source\CExportCamera.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef CCAMERAEXPORT_H
#define	CCAMERAEXPORT_H

#include <maya/MFnCamera.h>

#include <fstream>
#include <iostream>

using std::cerr;
using std::cout;
using std::fstream;


class CExportCamera
{
public:
	CExportCamera(){};

	bool export( MFnCamera& fnCamera, fstream& outFile );
};

#endif