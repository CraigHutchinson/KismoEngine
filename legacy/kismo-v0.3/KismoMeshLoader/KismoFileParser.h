/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\KismoMeshLoader\KismoFileParser.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef Kismo_FILEPARSER_H
#define Kismo_FILEPARSER_H

#include "IO/FileParser.h"

namespace rage
{
	/** \brief Kismo mesh/scene file parser implementation
	*/
	class KismoFileParser : public io::FileParser
	{
	public:

		typedef Stream::pos_type StreamPos;

	public:
		KismoFileParser( const String& filename );
		KismoFileParser( InStream& inputStream );
		KismoFileParser( OutStream& outputStream );
		KismoFileParser( Stream& ioStream );

		~KismoFileParser();

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
