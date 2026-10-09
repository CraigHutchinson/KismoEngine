/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\IO\FileParser.h
 * Status: current (newest copy of this file name in the archive)
 */
#ifndef IO_FILEPARSER_H
#define IO_FILEPARSER_H

#include "Framework/Types.h"
#include "Math/Rect.h"
#include "Math/Vector.h"

namespace io
{
	/** \brief ParserNode interface class
	\remarks
		Could differentiate between nodes and elements but to keep this generic i ignore this as
		it has no major effect at all!
	*/
	class _DllExport ParserNode
	{	
	public:
		class Impl;///< implimentation type 

		/** \brief Constructor takes an implimentation class which is protected preventing construction
		without using the helper functions
		*/
		ParserNode( Impl* impl = 0 );

		/** \brief Copy constructor
		*/
		ParserNode( const ParserNode& r );

		/** \brief Destructor deleted the implimentation
		*/
		~ParserNode();
				
		/** \brief gets whether the node is valid - a implimentation exists
		*/
		Bool isValid() const 
		{ return m_impl != 0; }

		/** \brief gets whether the node is invalid - a implimentation does not exists
		*/
		Bool isNull() const 
		{ return m_impl == 0; }

		/** \brief Assignment operator creates copies the implimentation
		*/
		ParserNode& operator =(const ParserNode& r);

		/** \brief Uses the implimentation class to get the element name
		*/
		const Char* getName() const;

		/** \brief Uses the implimentation class to get the element value
		*/
		const Char* getValue() const;	
		
		/** \brief Uses the implimentation class to get the elements value as a stream if possible
		*/
		InStream* getValueStream() const;	

		/** \brief Gets the first child element
		*/
		ParserNode firstChild() const;

		/** \brief Gets the first child element of the specified name
		*/
		ParserNode firstChild( const String& name ) const;

		/** \brief Gets the first attribute node
		*/
		ParserNode firstAttribute() const;	

		/** \brief Gets the first attribute node of the specified name
		*/
		ParserNode firstAttribute( const String& name ) const;	

		/** \brief Gets the next sibling element
		*/
		ParserNode next() const;

		/** \brief Gets the next sibling element of the specified name
		*/
		ParserNode next( const String& name ) const;	

		/** \brief Convers the node value to an Int16
		*/
		operator Int16() const;

		/** \brief Convers the node value to an UInt16
		*/
		operator UInt16() const;

		/** \brief Convers the node value to an Real
		*/
		operator Float32() const;

		/** \brief Convers the node value to an char*
		*/
		operator const Char*() const
		{ return getValue(); }

		/** \brief Comparison operator compares the nodes name to a string
		*/
		Bool operator ==( const String& rhs ) const;	
		
		/** \brief Comparison operator compares the nodes name to a string
		*/
		Bool operator !=( const String& rhs ) const;

		/** \brief Comparison operator compares the nodes name to a c-string
		*/
		Bool operator ==( const Char* rhs ) const;

		/** \brief Comparison operator compares the nodes name to a c-string
		*/
		Bool operator !=( const Char* rhs ) const;

		/** \brief not operator
		*/
		Bool operator !() const
		{ return isNull(); }

		operator Bool() const
		{ return isValid(); }

	protected:
#ifdef _DEBUG
		const Char* m_debugName; ///< Name of the attribute node for debugging purposes
#endif
		Impl* m_impl; ///< Pointer to the specific implimentation ( PIMPL idiom )
	};

	/** \brief Base FileParser interface
	*/
	class _DllExport FileParser
	{
	public:
		
		typedef ParserNode Node;

	public:

		/** \brief Constructor
		\param[in] filename The file to parse (optional)
		*/
		FileParser( const String& filename = String() );
		FileParser( InStream& inputStream );
		FileParser( OutStream& outputStream );
		FileParser( Stream& ioStream );

		virtual ~FileParser();

		/** \brief Open the specified file
		*/
		bool open( const String& filename );

		/** \brief Open from the specified stream
		*/
		bool open( InStream& inputStream );

		/** \brief Open from the specified stream
		*/
		bool open( OutStream& outputStream );

		/** \brief Open from the specified stream
		*/
		bool open( Stream& ioStream );

		/** \brief Open the file previously specified by open() or in the constructor
		*/
		virtual bool open() = 0;

		/** \brief returns whether the file opened successfully
		*/
		virtual bool is_open() = 0;

		/** \brief Closes the file
		*/
		virtual void close() = 0;

		/** \brief get the root element of a specified name
		*/
		ParserNode getRootNodeByName( const String& name );

		/** \brief get the root element
		*/
		virtual ParserNode getRootNode() = 0;
	
	protected:
		String m_filename; ///< The filename of the file to parse
		InStream* m_inStream; ///< The input file stream if set by the constructor
		OutStream* m_outStream; ///< The output file stream if set by the constructor
	};

} //END: io

#endif
