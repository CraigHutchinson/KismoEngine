/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\fileIO\KismoFileIO.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
#include "KismoFileIO.h"

#include <windows.h>
#include <direct.h>
#include <io.h>

namespace kismo
{
	template<> fio::FileIO* Singleton< fio::FileIO >::ms_instance = 0;

	namespace fio
	{
		
		FileIO& FileIO::getSingleton( void ) { assert( isValidPointer( ms_instance ) ); return ( *ms_instance ); }
		FileIO* FileIO::getSingletonPtr( void ) { return ms_instance;  }

		FileIO::FileIO( const k_String& rootDirectory )
		{
			m_directories.resize( _kc_SystemFolder_count );
			const k_Size bufferSize = MAX_PATH + 1;
			k_Char buffer[ bufferSize ] = {0}; //String buffer

			//----------------------------------
			// Application/Asset paths
			//----------------------------------
			//Get the path to the root directory (Where system, texture folders should be
			GetFullPathName( rootDirectory.c_str(), bufferSize, buffer, 0 );
			m_directories[ kc_SystemFolder_Root ].insert( new Directory( buffer ) );

			m_directories[ kc_SystemFolder_Plugins ].insert(		new Directory( _T("Plugins\\"), *m_directories[ kc_SystemFolder_Root ].begin() ) );
			m_directories[ kc_SystemFolder_System ].insert(			new Directory( _T("System\\"), *m_directories[ kc_SystemFolder_Root ].begin() ) ); 
			m_directories[ kc_SystemFolder_StaticMeshes ].insert(	new Directory( _T("StaticMeshes\\"), *m_directories[ kc_SystemFolder_Root ].begin() ) );
			m_directories[ kc_SystemFolder_Sound ].insert(			new Directory( _T("Sound\\"), *m_directories[ kc_SystemFolder_Root ].begin() ) );
			m_directories[ kc_SystemFolder_Textures ].insert(		new Directory( _T("Textures\\"), *m_directories[ kc_SystemFolder_Root ].begin() ) );
			m_directories[ kc_SystemFolder_Saves ].insert(			new Directory( _T("Saves\\"), *m_directories[ kc_SystemFolder_Root ].begin() ) );
			m_directories[ kc_SystemFolder_Music ].insert(			new Directory( _T("Music\\"), *m_directories[ kc_SystemFolder_Root ].begin() ) ); 
			m_directories[ kc_SystemFolder_Maps ].insert(			new Directory( _T("Maps\\"), *m_directories[ kc_SystemFolder_Root ].begin() ) ); 
			m_directories[ kc_SystemFolder_Animation ].insert(		new Directory( _T("Animation\\"), *m_directories[ kc_SystemFolder_Root ].begin() ) ); 
			m_directories[ kc_SystemFolder_System ].insert(			new Directory( _T("System\\"), *m_directories[ kc_SystemFolder_Root ].begin() ) );

			//Get the application working directory
			GetCurrentDirectory( bufferSize, buffer );
			m_directories[ kc_SystemFolder_Application ].insert( new Directory( buffer ) ); 

			//----------------------------------
			// System/OS paths
			//----------------------------------	
			//Get the operating systems directory
			GetWindowsDirectory( buffer, bufferSize );
			m_directories[ kc_SystemFolder_OS ].insert( new Directory( k_String(buffer) + _T("\\") ) ); 
			
			//Get the Temp directory
			GetTempPath( bufferSize, buffer );
			m_directories[ kc_SystemFolder_Temp ].insert( new Directory( buffer ) );
			
			//Get the System directory
			GetSystemDirectory( buffer, bufferSize );
			m_directories[ kc_SystemFolder_OS_System ].insert( new Directory( k_String(buffer) + _T("\\") ) ); 
		}

		FileIO::~FileIO()
		{
			for ( SystemDirectoryListList::iterator searchPathIter = m_directories.begin(); searchPathIter != m_directories.end(); ++searchPathIter )
			{		
				for ( DirectoryList::iterator iter = searchPathIter->begin(); iter != searchPathIter->end(); ++iter )
				{
					safeDelete( (*iter) );
				}
			}
		}

		/*
		//gets a vector of files in a folder
		k_Count FileIO::findFiles( const k_String& directory, FileList& out_FileList, bool recursive, const k_String& pattern )
		{
			intptr_t lHandle, res;
			_finddata_t tagData;
			k_Count filesFound = 0;
			
			if( chdir( directory.c_str() ) == -1 )//Change directory to the search path
			{
				LOG_ERROR( "The directory \"" + directory + "\" was not found!" );
				return filesFound ; //Directory not found
			}
			else
			{
			   // GetCurrentDirectory(
			}

			lHandle = _findfirst( pattern.c_str(), &tagData);
			res = 0;
			while (lHandle != -1 && res != -1)
			{
				if( !(tagData.attrib & _A_SUBDIR) )
				{
					File file( directory + tagData.name );			
					out_FileList.push_back( file );
					++filesFound; //Increment the file count
				}
				else
				if ( recursive ) //If doing a recursive search down the tree..
				{
					if(( tagData.attrib & _A_SUBDIR )
						&& strcmp(tagData.name, ".") //Don't include parent file links
						&& strcmp(tagData.name, ".."))
					{
						k_String subDirectory = directory + tagData.name + "/" ); 
						filesFound += findFiles( subDirectory, out_FileList, recursive, pattern );

						if( chdir( directory.c_str()) == -1 )
						{
							//GetDirectory
						}

					}
				}

				res = _findnext( lHandle, &tagData );
			}

			// Close if we found any files
			if( lHandle != -1 )
				_findclose(lHandle);

			return filesFound;
		}*/
	}
}