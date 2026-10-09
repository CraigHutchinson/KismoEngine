/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo Utility\branches\10-10-2010\KismoMaxExporterFH.cpp
 * Status: current (newest copy of this file name in the archive)
 */
#include "KismoMaxExporterFH.h"
using namespace std;

bool KismoMaxExporterFH::KismoBeginWriting(const string& name)
{
	// Open the file
	// NOTE: Check why I need and if indeed I should use .c_str
	KismoExportFile.open (name.c_str());
	
	// Check the file is opened and if so return true otherwise return false
	if (KismoExportFile.is_open()){	
		return true;
	}
	else {return false;}
}

void KismoMaxExporterFH::KismoEndWriting()
{
	// Close the export file
	KismoExportFile.close();
}

void KismoMaxExporterFH::KismoWrite(int indent, char* msg)
{
	// Loop through and indent the line by the correct amount
	for (int i = 0; i < indent; i++) KismoExportFile<<"\t";

	// Do stuff here with data
	KismoExportFile<<msg;

	// Write a new line in the file
	KismoExportFile<<"\n";
}