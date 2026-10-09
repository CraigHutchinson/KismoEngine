/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo Utility\trunk\3Ds Max Exporter\KismoMaxExporterFH.h
 * Status: superseded by kismo-utility/branches/10-10-2010/KismoMaxExporterFH.h (same name, content differs)
 */
#ifndef KISMO_MAX_EXPORTER_FH
#define KISMO_MAX_EXPORTER_FH

#include <fstream>
#include <string>
#include <sstream>

using namespace std;

class KismoMaxExporterFH 
{
public:
	
	// NOTE: Use constructor and destructor instead?
	bool KismoBeginWriting(const string& name);
	void KismoEndWriting();
	// The main writing function
	void KismoWrite(int indent, string msg);
	// This version of the write function accepts an additional int value which is appended to the msg string
	void KismoWrite(int indent, string msg, int i);
	// This version of the write function will accept three float values for co-ordinates
	void KismoWrite(int indent, string msg, int i, float x, float y, float z);
	
private:	
	ofstream KismoExportFile;
};

#endif