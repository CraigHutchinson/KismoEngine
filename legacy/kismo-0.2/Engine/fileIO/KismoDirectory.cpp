/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\fileIO\KismoDirectory.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
#include "fileIO/KismoDirectory.h"
#include "fileIO/KismoFile.h"

#include "framework/KismoLog.h"

namespace kismo
{
	namespace fio
	{	
		Directory::Directory( const k_String& fullpath, k_UInt8 priority ) : 
			PathNode( fullpath, kc_PathNodeType_Directory ),
			m_priority( priority )
		{}
		Directory::Directory( const k_String& dirName, PathNode::Ptr parentNode, k_UInt8 priority ) : 
			PathNode( dirName, parentNode, kc_PathNodeType_Directory ),
			m_priority( priority )
		{}


		File::Ptr Directory::findFile( const k_String& filename, bool recursive )
		{ 
			k_String fullFilename; //

			//Search for the file
			if ( platform::findFile( fullFilename, fullPath(), filename, recursive ) )
			{
				return new File( fullFilename ); //TODO: The file should be stored somewhere internal for re-use?
			}
			else
			{
				LOG_MESSAGE( _T("Failed to find the file '") << filename << _T('\'') );
				return 0;
			}

		}

	}
}