/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\fileIO\KismoDirectory.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef _KISMO_DIRECTORY_H
#define _KISMO_DIRECTORY_H

#include "framework/KismoCommon.h"

#include "KismoFile.h"

namespace kismo
{
	namespace fio
	{
		//Folder constants
		enum ke_SystemFolder
		{
			kc_SystemFolder_Root,
			kc_SystemFolder_Plugins,
			kc_SystemFolder_System,
			kc_SystemFolder_Sound,
			kc_SystemFolder_StaticMeshes,
			kc_SystemFolder_Textures,
			kc_SystemFolder_Saves,
			kc_SystemFolder_Music,
			kc_SystemFolder_Maps,
			kc_SystemFolder_Animation,
			kc_SystemFolder_Log,
			kc_SystemFolder_Temp,
			kc_SystemFolder_OS,
			kc_SystemFolder_OS_System,
			kc_SystemFolder_Application,
			_kc_SystemFolder_count,
		};


		//Forward Declaration
		class File;

		//Directory
		class _KismoExport Directory : public PathNode
		{
		public:			
			typedef frx::SmartPtr< Directory > Ptr;
		public:
			
			//Constructor takes the full directory path
			// Priority is used to order system directory searches in FileIO (higher value = searched first)
			Directory( const k_String& path, k_UInt8 priority = 128 );
			Directory( const k_String& dirName, PathNode::Ptr parentNode, k_UInt8 priority = 128 );

			//Search for files
			// recursive -  should sub-directories be searched
			// pattern - a wildcard string/filename
			virtual k_Count findFiles( k_Vector< File::Ptr >& out_FileList, bool recursive = false, const k_String& pattern = _T("*.*") ){ return 0;}
			virtual File::Ptr findFile( const k_String& filename, bool recursive = false );

			virtual Directory::Ptr findDirectory( const k_String& name, bool recursive = false ){ return 0;}


						
			// buildTree - recursively create all sub-directories 
			//				NOTE: Will reuse existing directories if they are already created already
			virtual void buildTree()
			{
				

			}



			inline friend std::ostream& operator << ( std::ostream& outStream, const Directory& f )
			{
				//TODO:
				//FileList::const_iterator end = f.end();
				//for ( FileList::const_iterator iter = f.begin(); iter != end; ++iter )
				//	outStream << iter->path().fullPath() << std::endl;

				return outStream;
			}

			bool operator < ( const Directory& b ) { return m_priority < b.m_priority; }
			bool operator > ( const Directory& b ) { return m_priority > b.m_priority; }

		protected:

			// Priority is used to order system directory searches in FileIO (higher value = searched first)
			k_UInt8 m_priority;

			//PathNode::List m_children; ///< Child path nodes
		};


	} //END: fileio

} //END: kismo

#endif