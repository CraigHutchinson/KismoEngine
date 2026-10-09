/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\RAGEMeshLoader\RAGEFileParser.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef RAGE_FILEPARSER_H
#define RAGE_FILEPARSER_H

#include "IO/FileParser.h"

namespace rage
{
	/** \brief RAGE mesh/scene file parser implementation
	*/
	class RAGEFileParser : public io::FileParser
	{
	public:

		typedef Stream::pos_type StreamPos;

	public:
		RAGEFileParser( const String& filename );
		RAGEFileParser( InStream& inputStream );
		RAGEFileParser( OutStream& outputStream );
		RAGEFileParser( Stream& ioStream );

		~RAGEFileParser();

		/** \brief Open the file previously specified by open() or in the constructor
		*/
		bool open();

		/** \brief returns whether the file opened successfully
		*/
		bool is_open();

		/** \brief Closes the file
		*/
		void close();

		/** \brief get the root element
		*/
		virtual io::ParserNode getRootNode();

	protected:

		FStream* m_fs; //< File stream object to use when only have the filename

	};

} //END: rage

#endif
