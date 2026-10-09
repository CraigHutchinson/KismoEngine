/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\IO\DirectoryManager.h
 * Status: current (newest copy of this file name in the archive)
 */
#ifndef FRX_DIRECTORYMANAGER_H
#define FRX_DIRECTORYMANAGER_H

#include "Framework/Common.h"
#include "Framework/Rtti.h"
#include "Framework/Singleton.h"

#define _DirectoryManager io::DirectoryManager::getSingleton()

/** \brief IO namespace contains all file operation classes and directory manager etc
*/
namespace io
{
	/** \brief Directory manager stores paths where files are stored 
	**/
	class _DllExport DirectoryManager : public frx::Singleton< DirectoryManager >
	{
	public:		
		SINGLETON( DirectoryManager );

		Bool addDirectoryMap( const frx::Rtti& type, const String& directory );
 
		String getFullPath( const String& filename ) const; 
		String getFullPath( const String& filename, const frx::Rtti& type ) const;

	protected:
		typedef MultiMap< const frx::Rtti*, String > DirectoryMap;
		DirectoryMap m_directories; ///< Map of 
	};

} //END: io

#endif
