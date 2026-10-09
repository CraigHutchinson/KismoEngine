/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\fileIO\KismoFile.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
#include "KismoFile.h"

#include "KismoDirectory.h"

namespace kismo
{
	namespace fio
	{

		File::File( const k_String& path ) : 
			PathNode( path, kc_PathNodeType_File ),
			m_isOpen( kc_FALSE )
		{
		}

		File::File( const k_String& fileName, PathNode::Ptr parentNode ) : 
			PathNode( fileName, parentNode, kc_PathNodeType_File ),
			m_isOpen( kc_FALSE ) 
		{
		}

		File::~File()
		{
			//NOTE: No need to remove the streams as these will be destroid automatically when needed via smart pointer
		}

	}
}
