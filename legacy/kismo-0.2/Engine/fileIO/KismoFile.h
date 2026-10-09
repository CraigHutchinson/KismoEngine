/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\fileIO\KismoFile.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef _KISMO_FILE_H
#define _KISMO_FILE_H

#include "fileIO\KismoPathNode.h"

namespace kismo
{
	namespace fio
	{
		//Forward Declaration
		class Directory;

		//File
		class File : public PathNode
		{
		public:			
			typedef frx::SmartPtr< File > Ptr;
		public:

			File( const k_String& fullpath );
			File( const k_String& fileName, PathNode::Ptr parentNode );
			~File();

			//------------------
			// Filename sections
			//------------------
			//Get just the extension part of the fileName e.g. name.extension = extension
			k_String extension() const { return m_name.right( m_lastDot ); }

			//Has the file been opened?
			k_Bool isOpen() { return m_isOpen; }

			/** Open a new data stream for the file */
			DataStream::Ptr open( ke_streamMode mode );

			//Close the open streams.
			// closeLockedStreams - Forces any streams in use to be closed when avialable/unlocked
            k_Bool close( k_Bool closeLockedStreams = true )
			{}

		private:
			k_UInt16 m_lastDot; //Index of the last dot in the filename

			k_Bool m_isOpen; //Is the fileopen

			k_Vector< DataStream::Ptr > m_streams; //Vector of open streams for this file

		};


	} //END: fileio

} //END: kismo

#endif