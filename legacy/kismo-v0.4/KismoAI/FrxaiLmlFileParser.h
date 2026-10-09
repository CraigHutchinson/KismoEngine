/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoAI\FrxaiLmlFileParser.h
 * Status: unique (no other copy of this file name in the archive)
 */
/** \author Craig Hutchinson \file 
	\brief 
*/
#ifndef FRXAI_LMLFILEPARSER_H
#define FRXAI_LMLFILEPARSER_H

#include "FileParser.h"

namespace frxai
{
	class LmlFileParser : public io::FileParser
	{
	public:

		typedef InFStream Stream;
		typedef Stream::pos_type StreamPos;

	public:
		LmlFileParser( const String& filename );

		~LmlFileParser();

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
		io::ParserNode getRootNode();

	protected:

		Stream m_fs;

	};

} //END: frxai

#endif
