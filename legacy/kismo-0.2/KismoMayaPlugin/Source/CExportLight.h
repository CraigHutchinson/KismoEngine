/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\KismoMayaPlugin\Source\CExportLight.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef CLIGHTEXPORT_H
#define	CLIGHTEXPORT_H

#include <maya/MFnLight.h>

#include <fstream>
#include <iostream>

using std::cerr;
using std::cout;
using std::fstream;


class CExportLight
{
public:
	CExportLight(){};

	bool export( MFnLight& fnLight, fstream& outFile );

};

#endif