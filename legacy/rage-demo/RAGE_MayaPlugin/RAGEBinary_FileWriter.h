/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\RAGE_MayaPlugin\RAGEBinary_FileWriter.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef _KISMOBINARY_FILEWRITER_H
#define _KISMOBINARY_FILEWRITER_H

#include "FileWriter.h"

class RAGEBinary_FileWriter : public FileWriter
{
public:
	RAGEBinary_FileWriter() :
		FileWriter()
	{}
	virtual ~RAGEBinary_FileWriter() {}

	//Write to the file stream
	MStatus doWrite( const MString& filename )
	{
		//error status
		MStatus status;

		std::ofstream ofile( filename.asChar(), ios::binary );

		if ( !ofile.is_open() )
		{
  			status.perror( "Failed to open file for writing " + filename );
			return status;
		}

		for ( std::vector< DagNode* >::const_iterator i = m_nodes.begin(); i != m_nodes.end(); ++i )
		{
			ofile << ios::binary<< 3 << 2 << 1 << (*i)->path.fullPathName() << '\n';
			DEBUG_ONLY( cerr << (*i)->path.fullPathName() << '\n' );
		}

		ofile.close();
		
		MGlobal::displayInfo("Export to " + filename + " successful!"); 
		return status;
	}

	static FileWriter* create() { return new RAGEBinary_FileWriter; }
};

#endif