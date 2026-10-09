/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\fileIO\KismoFileIO.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef _KISMO_FileIO_H
#define _KISMO_FileIO_H

#include "framework/KismoPlatform.h"
#include "framework/KismoSingleton.h"
#include "fileio\KismoDirectory.h"

namespace kismo
{
	using frx::Singleton;

	namespace fio
	{
		class _KismoExport FileIO : public Singleton< FileIO >
		{
		public:

			typedef std::set< Directory* > DirectoryList;
			typedef k_Vector< DirectoryList > SystemDirectoryListList;

		public:
			FileIO( const k_String& rootDirectory = platform::kc_Path_RelativeUp );
			~FileIO();

				
			static FileIO& getSingleton( void );
			static FileIO* getSingletonPtr( void );
			
			//const k_String& rootFolderPath() const { return m_folders[ kc_SystemFolder_Root ]->getPath(); }//Get the root folder
			//const k_String& appFolderPath() const { return m_folders[ kc_SystemFolder_Application ]->getPath(); }//Get the application folder (Where app is running from)
/*

			const k_String& getPath( ke_SystemFolder systemFolderID )
			{
				assert( systemFolderID >= 0 && systemFolderID < _kc_SystemFolder );
				return m_folders[ systemFolderID ]->getPath();
			}

			void setPath( ke_SystemFolder systemPathID, const k_String& path )
			{
				assert( systemFolderID >= 0 && systemFolderID < _kc_SystemFolder );
				m_folders[ systemFolderID ]->setPath( path );
			}

			*/

			//Finds the first file in the system folder search
			// recursive - specifies whether search should look in sub folders (default = true)
			File::Ptr findFile( ke_SystemFolder searchPath, const k_String& name, k_Bool recursive = true )
			{
				//Iterate through all search paths with the specified ID
				for ( DirectoryList::const_iterator iter = m_directories[ searchPath ].begin(); iter != m_directories[ searchPath ].end(); ++iter )
				{
					//Search this directory for the file
					File::Ptr file( (*iter)->findFile( name, recursive ) );
					
					//If the file is found return it now
					if ( file )
						return file;
				}

				return 0; //Failed
			}

			//Find a plugin file
			File::Ptr findPlugin( const k_String& name )
			{
				k_String tpluginFilename( name + COMPILE_SWITCH( _T("_D.kdp"), _T(".kdp") ) );
				return findFile( kc_SystemFolder_Plugins, tpluginFilename );
			}

		protected:

			SystemDirectoryListList m_directories; //System directories/search paths


		};
		

	} //END: fileio

} //END: kismo

#endif