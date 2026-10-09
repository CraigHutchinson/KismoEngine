/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo Utility\branches\11-10-2010\KismoMaxExporterFH.cpp
 * Status: superseded by kismo-utility/branches/10-10-2010/KismoMaxExporterFH.cpp (same name, content differs)
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

void KismoMaxExporterFH::KismoWrite(int indent, string msg)
{
	// Loop through and indent the line by the correct amount
	for (int i = 0; i < indent; i++) KismoExportFile << "\t";

	// Do stuff here with data
	KismoExportFile << msg;

	// Write a new line in the file
	KismoExportFile << "\n";
}

void KismoMaxExporterFH::KismoWrite(int indent, string msg, int i)
{
	// Create a string stream and concatinate values
	stringstream output;
	output << msg << i;
	// Send the stringstream output to the main KismoWrite function
	KismoWrite(indent, output.str());
}

void KismoMaxExporterFH::KismoWrite(int indent, string msg, int i, float x, float y, float z)
{
	stringstream output;
	output << msg << "[" << i << "] : (" << x << ", " << y << ", " << z << ")";
	// Send the stringstream output to the main KismoWrite function
	KismoWrite(indent, output.str());
}