/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\fileIO\KismoFilePlatform.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
#include "fileIO/KismoFilePlatform.h"
#include "framework/KismoLog.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stack>

namespace kismo
{
	//Maximum number of directories that can be placed on the directory stack (usefull to check if a directory is not poped )
	const k_UInt16 MAX_DIRECTORY_STACK = 16;

	//Stack of working directories
	std::stack< k_String > m_directoryStack;

	//Initialises the stack with the current working dirctory
	// NOTE: You should not need to call this from anywhere else
	void initialiseStack()
	{
		assert( m_directoryStack.size() == 0 );

		const k_Size bufferSize = MAX_PATH + 1;
		k_Char buffer[ bufferSize ];
		GetCurrentDirectory( bufferSize, buffer );

		m_directoryStack.push( buffer );
	}


	//Set the current working directory
	k_Bool setWorkingDirectory( const k_String& dir )
	{
		return ( SetCurrentDirectory( dir.c_str() ) != 0 );
	}


	namespace fio
	{
		namespace platform
		{
			//Takes a fullpath and generates the relative path from the specified root directory
			k_String toRelative( const k_String& path, const k_String& rootPath )
			{
				k_String relative = kc_Path_RelativeSame; //Start relative paths with the 'this' folder path of '.\'
				
				if ( rootPath == path )//First make sure the directories are not the same
					return _T("");

				k_String::Index_T index = 0;
				k_String::Index_T mismatchIndex = index;//Store the last slash at which the two paths match
				//Find the first different charater between the two paths
				while ( (rootPath.length() >= index) && (path[ index ] == rootPath[ index ]) )
				{
					if ( path[ index ] == kc_Path_Seperator )
						mismatchIndex = index;
					++index;
				}

				if ( mismatchIndex == 0 ) //Check if the string are entirely different! (e.g. different drives)
					return path; //Return the fullPath as a relative is not possible

				//Count the number of directories the root is 'up' from here
				while ( rootPath.length() >= index )
				{
					if ( rootPath[ index ] == kc_Path_Seperator )
						relative += kc_Path_RelativeUp; //Add to the relative path
					++index;
				}

				//finally attach on the rest of the full path
				relative += path.right( mismatchIndex + 1 );

				return relative; //Return the relative path

			}

			k_String toFullPath( const k_String& path, const k_String& rootPath )
			{
				//TODO: Generalise this function for any kc_Path_RelativeUp value
				k_Count dots = 0;
				k_String::Index_T index = 0;
				k_String::Index_T rootIndex = rootPath.length() - 1; //Ignore trailing slash
				while ( index < path.length() )
				{
					if ( path[ index ] == kc_Path_RelativeUp[0] )
					{
						++dots;
					}
					else
					if( path[ index ] == kc_Path_Seperator )
					{
						if ( dots >= 2 ) //If we have an up directory tag
						{
							//move up a directory in the root
							while( --rootIndex >= 0 && rootPath[ rootIndex ] != kc_Path_Seperator )
							{}

							if ( rootIndex == 0 )
							{
								assert( false ); //Problem case!!
								return path;
							}
						}
						dots = 0; //Reset the dot counter
					}
					else
					{
						break; //Break from the loop
					}
					++index;
				}

				k_String fullPath = rootPath.left( rootIndex + 1 );
				fullPath += path.right( index );

				return fullPath;

			}


			//Makes sure all slashes in a file path are all '\' (back-slashes)
			// - type sets whether the path should be treated as a directory and terminated with a slash, or as a file path
			k_String slashify( const k_String& path, ke_PathType type )
			{
				k_String newPath = path;

				k_String::Index_T index = 0;
				//Loop through the string looking for invalid slashes
				while ( index < path.length() )
				{
					if ( kc_Path_InvalidSeperators.findFromStart( newPath[ index ] ) != kc_Path_InvalidSeperators.npos ) //Replace all invalid slashes
						newPath[ index ] = kc_Path_Seperator;
					++index;
				}

				//Determine the type of slahing to do and make sure the terminating slash is on the path if needed
				switch(type)
				{
				case kc_PathType_Directory: //Add on the terminating slash if needed
					{
						if ( newPath[ index - 1 ] != kc_Path_Seperator )
							newPath += kc_Path_Seperator;
						break;
					}
				case kc_PathType_File: //Remove the terminating slash if needed
					{
						newPath.rtr
						if ( newPath[ index - 1 ] == kc_Path_Seperator ) //TODO: Is this valid if we want to clip the end
							newPath.reserve( newPath.length() - 1);
						break;
					}
				case kc_PathType_Unknown:
				default:
					{
						break;
					}
				}

				return newPath;

			}

			k_Bool isRelative( const k_String& path ) 
			{ 
				if ( path[ 0 ] == kc_Path_RelativeSame[ 0 ] || path[ 0 ] == kc_Path_RelativeUp[ 0 ] )
				{
					//Return if the string starts with a '..\' , '.\'
					return ( path.left( kc_Path_RelativeUp.length() ) == kc_Path_RelativeUp ) || 
						( path.left( kc_Path_RelativeSame.length() ) == kc_Path_RelativeSame );
				}
				else
					return (path[ 0 ] == kc_Path_Seperator); //Starts with '\'
			}

			//gets the current working directory
			const k_String& getWorkingDirectory()
			{
				if ( m_directoryStack.size()  == 0 )
				{
					LOG_WARNING( _T("getWorkingDirectory:: Directory stack has no elements! Getting current working directory.")  );		
					initialiseStack();
				}

				//return the value on the top of the stack
				return m_directoryStack.top();
			}


			//Push a working directory onto the stack
			k_Bool pushWorkingDirectory( const k_String& dir )
			{
				if ( m_directoryStack.size() > MAX_DIRECTORY_STACK )
				{
					LOG_MESSAGE( _T("pushWorkingDirectory:: Maximum directory stack size reached! Directory will still be set...")  );
				}
				else
				if ( m_directoryStack.size() == 0 )
				{
					//initialise the directory stack with the current woring directory
					initialiseStack();
				}

				m_directoryStack.push( dir );
				return setWorkingDirectory( dir );
			}


			//Pop the topmost working directory off the stack
			k_Bool popWorkingDirectory()
			{
				if ( m_directoryStack.size() <= 1 )
				{
					LOG_WARNING( _T("popWorkingDirectory:: At the bottom of the directory stack. Cannot pop!")  );
					return false;
				}

				m_directoryStack.pop();
				return setWorkingDirectory( m_directoryStack.top() );
			}

			//Looks for a file in the current working directory
			// Returns success/fail and the fully adjusted filename if the file is found
			k_Bool findFile( k_String& outFilename, const k_String& path, const k_String& fileQuery, k_Bool recursive )
			{

				WIN32_FIND_DATA findFileData; 
				HANDLE hFind;

				k_String query = path + fileQuery;

				//Get the search handle 
				hFind = FindFirstFile( query.c_str(), &findFileData ); 

				if (hFind != INVALID_HANDLE_VALUE) 
				{ 
					//Loop through all the found files
					do 
					{ 
						//Make sure its not a directory
						if ( !(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)  )
						{
							outFilename = path + findFileData.cFileName;
							FindClose( hFind );
							return true;
						}
					} 
					while ( FindNextFile( hFind, &findFileData ) ); 
					FindClose( hFind );
				}

				if ( recursive )
				{
					hFind = FindFirstFile( _T("*"), &findFileData ); 

					if ( hFind != INVALID_HANDLE_VALUE) 
					{ 
						//Loop through all the found files
						do 
						{ 
							//Make sure its not a directory
							if ( findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY
								&& _tcscmp( findFileData.cFileName, _T(".") ) //Don't include parent file links
								&& _tcscmp( findFileData.cFileName, _T("..") ) )
							{						
								//Find the file within the sub-directory passing recursively down the tree
								if ( findFile( outFilename, path + findFileData.cFileName, fileQuery, recursive ) )
								{
									FindClose( hFind );
									return true;
								}
							}
						} 
						while ( FindNextFile( hFind, &findFileData ) ); 
						FindClose( hFind );
					}
					
				}
				return false;
			}



		} //END: platform

	} //END: fileio

} //END: kismo

