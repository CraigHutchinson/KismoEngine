/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\fileIO\KismoFilePlatform.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef _KISMO_FILE_PLATFORM_H
#define _KISMO_FILE_PLATFORM_H

#include "framework/KismoCommon.h"

namespace kismo
{
	namespace fio
	{
		namespace platform
		{

			const k_Char kc_Path_Seperator = _T('\\');
			const k_String kc_Path_InvalidSeperators = _T("/"); //Invalid directory seperator list
			const k_String kc_Path_RelativeUp = _T("..\\");	
			const k_String kc_Path_RelativeSame = _T(".\\");
			//const k_Char kc_ArchiveLabel = '@';	//Label used at the start of a folder name to signify that
													//	 an archive of that name should be used if one exists	

			//Takes a fullpath and generates the relative path from the root
			k_String toRelative( const k_String& path, const k_String& rootPath );
			
			//Takes a relative path and returns the full path to the file
			k_String toFullPath( const k_String& path, const k_String& rootPath );

			//Makes sure all slashes in a file path are all '\'(kc_Path_Seperator)
			// type - sets whether the path should be a slash terminated directory, or as a file path
			enum ke_PathType
			{
				kc_PathType_Unknown, //Unknown whether the path is to a file or directory (terminating slashes are not altered)
				kc_PathType_File,
				kc_PathType_Directory, //terminates path with slashes
			};	
			k_String slashify( const k_String& path, ke_PathType type = kc_PathType_Unknown );

			//Is the path relative? aka starts with '..\' , '.\' or '\'
			k_Bool isRelative( const k_String& path );

			//gets the current working directory
			const k_String& getWorkingDirectory();

			//Push a working directory onto the stack
			k_Bool pushWorkingDirectory( const k_String& dir );

			//Pop the topmost working directory off the stack
			k_Bool popWorkingDirectory();

			//Looks for a file in the current working directory
			// Returns success/fail and the fully adjusted filename if the file is found
			k_Bool findFile( k_String& outFilename, const k_String& path, const k_String& fileQuery, k_Bool recursive = false );

		} //END: platform	

	} //END: fileio

} //END: kismo

#endif