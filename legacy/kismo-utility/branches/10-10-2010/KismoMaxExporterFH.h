/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo Utility\branches\10-10-2010\KismoMaxExporterFH.h
 * Status: current (newest copy of this file name in the archive)
 */
#ifndef KISMO_MAX_EXPORTER_FH
#define KISMO_MAX_EXPORTER_FH

#include <fstream>
using namespace std;

class KismoMaxExporterFH 
{
public:
	
	bool KismoBeginWriting(const string& name);
	void KismoEndWriting();
	void KismoWrite(int indent, char* msg);
	
private:	
	ofstream KismoExportFile;
};

#endif