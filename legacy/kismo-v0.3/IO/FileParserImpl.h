/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\IO\FileParserImpl.h
 * Status: current (newest copy of this file name in the archive)
 */
#ifndef IO_FILEPARSER_IMPL_H
#define IO_FILEPARSER_IMPL_H

namespace io
{
	/** \brief implimentation class which defines the interface for file parsers
	\remarks
		This is implimented here as a user of FileParser does not need to know about this
	*/
	class ParserNode::Impl
	{
	public:
		virtual ~Impl(){}
		virtual const Char* getName() const = 0;
		virtual const Char* getValue() const = 0;
		virtual InStream* getValueStream() const = 0;	
		virtual Impl* firstChild() const = 0;
		virtual Impl* firstChild( const String& name ) const = 0;
		virtual Impl* firstAttribute() const = 0;	
		virtual Impl* firstAttribute( const String& name ) const = 0;		
		virtual Impl* next() const = 0;	
		virtual Impl* next( const String& name ) const = 0;
		virtual Impl* copy() const = 0;
	};

} //END: io

#endif
