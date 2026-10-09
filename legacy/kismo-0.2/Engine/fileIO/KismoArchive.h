/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\fileIO\KismoArchive.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef _KISMO_ARCHIVE_H
#define _KISMO_ARCHIVE_H

#include "KismoDirectory.h"

namespace kismo
{
	namespace fileSystem
	{
		//archive derives directory to allow for a custom data structure invisibly acting like a directory
		class Archive : public Directory
		{
		public:
			//Search for files
			// recursive -  should sub-directories be searched
			// pattern - a wildcard string/filename
			virtual k_Count findFiles( vector< File* >& out_FileList, bool recursive = false, const k_String& pattern = "*.*" );
			virtual File* findFile( const k_String& filename, bool recursive = false );
			virtual Directory* findDirectory( const k_String& name, bool recursive = false );

		private:

		};
		
	} //END: fileSystem

} //END: kismo

#endif