/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\IO\DirectoryManager.cpp
 * Status: superseded by kismo-v0.3/IO/DirectoryManager.cpp (same name, content differs)
 */
#include "DirectoryManager.h"
#include "Platform/Platform.h"
#include "Framework/Log.h"
#include <stdio.h>

IMPL_SINGLETON( io::DirectoryManager )

namespace io
{
	Bool DirectoryManager::addDirectoryMap( const frx::Rtti& type, const String& directory )
	{
		logMessage( "FirectoryMap added for '" << type.typeFullName() << "' to path '" << directory << "'" );
		m_directories.insert( DirectoryMap::value_type( &type, directory ) );
		return true;
	}

	String DirectoryManager::getFullPath( const String& filename ) const
	{
		for ( DirectoryMap::const_iterator i = m_directories.begin(); i != m_directories.end(); ++i )
		{
			String tempPath = i->second + filename;
			if ( platform::fileExists( tempPath.c_str() ) )
				return tempPath;						
		}
		return filename;
	}

	String DirectoryManager::getFullPath( const String& filename, const frx::Rtti& type ) const
	{		
		std::pair< DirectoryMap::const_iterator, DirectoryMap::const_iterator > range = m_directories.equal_range( &type );
		for ( DirectoryMap::const_iterator i = range.first; i != range.second; ++i )
		{
			String tempPath = i->second + filename;
			if ( platform::fileExists( tempPath.c_str() ) )
				return tempPath;						
		}
		return filename;
	}


} //END: io
