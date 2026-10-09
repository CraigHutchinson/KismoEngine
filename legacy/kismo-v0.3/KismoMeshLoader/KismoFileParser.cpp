/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\KismoMeshLoader\KismoFileParser.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
#include "KismoFileParser.h"
#include "IO/FileParserImpl.h"
#include "Framework/Log.h"

const Char newLineChar = _T('\n');
const String commentLineTag = _T("//");
const Char commentBlockTag = _T('#');

const Char elementStartTag = _T('{');
const Char elementEndTag = _T('}');
const Char attributeStartTag = _T('(');
const Char attributeEndTag = _T(')');
const Char attributeValueStartTag = _T('\"');
const Char attributeValueEndTag = _T('\"');

namespace rage
{
	/**  \brief Implementation fo the RageFileParser whihc is hidden from the user
		\remarks Uses the pimpl design (Pointer to implementation)
	*/
	class KismoFileParserImpl : public io::ParserNode::Impl
	{
	public:

		KismoFileParserImpl(InStream* inStream, OutStream* outStream, const String& name, KismoFileParser::StreamPos pos  ) :
			m_inStream( inStream ),
			m_outStream( outStream ),
			m_pos( pos ), 
			m_name( name )
		{}

		virtual const Char* getName() const;
		virtual const Char* getValue() const;
		virtual Impl* firstChild() const;
		virtual Impl* firstChild( const String& name ) const;
		virtual Impl* firstAttribute() const;	
		virtual Impl* firstAttribute( const String& name ) const;	

		/** \brief Reads a file 'word' ignoring comment lines
		*/
		Bool readBlock( String& tval ) const;

		/** \brief an element and places the read cursor just past the current elements data
		*/
		Bool skipElement( String& tval ) const;

	protected:
		mutable InStream* m_inStream; ///< The stream being loaded from
		mutable OutStream* m_outStream; ///< The stream being written to
		KismoFileParser::StreamPos m_pos; ///< The position in the file just after this nodes name
		String m_name; ///< The name of the node
	};

	//---------------------------------------------------------------------------
	// Node implimentation definitions
	//---------------------------------------------------------------------------
	/** \brief File parser attributes
	*/
	class KismoFileParserAttribImpl : public KismoFileParserImpl
	{
	public:
		KismoFileParserAttribImpl( InStream* inStream, OutStream* outStream, const String& name, KismoFileParser::StreamPos postNamePos );
	
		virtual Impl* next() const;	
		virtual Impl* next( const String& name ) const;
		virtual Impl* copy() const;		
		virtual const Char* getValue() const;
		virtual InStream* getValueStream() const;	
	private:
		mutable String m_value; ///< The value - only used after a call to getValue()
	};

	/** \brief File parser attributes
	*/
	class KismoFileParserElementImpl : public KismoFileParserImpl
	{
	public:
		/** \brief Constructor for reading an element from a file after the name tag with the name supplied
		*/
		KismoFileParserElementImpl( InStream* inStream, OutStream* outStream, const String& name, KismoFileParser::StreamPos postNamePos );
	
		virtual Impl* next() const;	
		virtual Impl* next( const String& name ) const;
		virtual Impl* copy() const;
		virtual InStream* getValueStream() const;

	};

	//---------------------------------------------------------------------------
	// base node implimentation
	//---------------------------------------------------------------------------
	Bool KismoFileParserImpl::readBlock( String& tval ) const
	{
		bool comment;
		do
		{
			(*m_inStream) >> tval;
			if ( tval.left( commentLineTag.length() ) == commentLineTag )
			{
				(*m_inStream).ignore( Stream_SizeMax, newLineChar );
				comment = true;
			}	
			else
				comment = false;
		}
		while ( comment );

		//assert( (*m_inStream).good() );
		return (*m_inStream).good();
	}

	Bool KismoFileParserImpl::skipElement( String& tval ) const
	{
		// Read up to the element attribute end tag ')'
		(*m_inStream).ignore( Stream_SizeMax, attributeEndTag );
		assert( (*m_inStream).good() );

		//String tval;
		//Read the next tag which could be an element, an end tag, or start tag
		//KismoFileParser::StreamPos pos = (*m_inStream).tellg();
		if ( !readBlock( tval ) )
			return false;

		if ( tval[0] == elementStartTag ) ///< if we have child tags
		{
			//pos = (*m_inStream).tellg();
			if ( !readBlock( tval ) ) 
				return false;

			if ( tval[ tval.length() - 1 ] == attributeStartTag )
			{
				//Check if the first data is an element, if not we can jsut skip the data
				while ( (*m_inStream).good() && tval[ tval.length() - 1 ] == attributeStartTag ) 
				{
					skipElement( tval ); //If there is a child element we need to skip it!
				}

				assert( (*m_inStream).good() );
				if ( !readBlock( tval ) ) 
					return false;
			}
			else
			if ( tval[0] != elementEndTag )
			{					
				// Read up to the element end tag '}'
				(*m_inStream).ignore( Stream_SizeMax, elementEndTag );
				assert( (*m_inStream).good() );
				if ( !readBlock( tval ) ) 
					return false;
				return true;
			}
			assert( (*m_inStream).good() );
			
			return true;
		}
		else
		if ( tval[0] == elementEndTag ) ///< if we have child tags
		{
			//(*m_inStream).seekg( pos ); //< Move back to before the end tag
			assert( (*m_inStream).good() );
			return true; ///< We managed to skip the element!
		}
		else
		{
			assert( tval[ tval.length() - 1 ] == attributeStartTag ); //< Should be the next element 
			//(*m_inStream).seekg( pos ); //< Move back to before the new tag
			assert( (*m_inStream).good() );
			return true;
		}
	}

	//---------------------------------------------------------------------------
	// Element node implimentation
	//---------------------------------------------------------------------------
	KismoFileParserElementImpl::KismoFileParserElementImpl( InStream* inStream, OutStream* outStream, const String& name, KismoFileParser::StreamPos posNamePos  ) :
		KismoFileParserImpl( inStream, outStream, name, posNamePos ) 
	{
	}

	const Char* KismoFileParserImpl::getName() const
	{
		return (const Char*)m_name.c_str();
	}

	const Char* KismoFileParserImpl::getValue() const
	{
		assert( false );
		return 0;
	}

	io::ParserNode::Impl* KismoFileParserImpl::firstChild() const
	{		
		if ( (*m_inStream).tellg() != m_pos )
			(*m_inStream).seekg( m_pos ); //Make sure we are at the position of this node

		// Read up to the attribute end tab ')'
		(*m_inStream).ignore( Stream_SizeMax, attributeEndTag );

		String tval;
		readBlock( tval ); ///< Read what should be after the atribute close tag

		if ( tval[0] != elementStartTag ) //< Should be '{' if child data is supplied
			return 0; ///< No children!

		//Find a data element which has an attribute start tag definiing it not as data!
		bool found = false;
		while ( (*m_inStream).good() && tval[0] != elementEndTag )
		{
			if ( tval[ tval.length() - 1 ] == attributeStartTag )
			{
				tval.trimRight( attributeStartTag );
				found = true;
				break; //< We found a child
			}
			else
			if ( !readBlock( tval ) ) break;
		}

		//check if we found an element
		if ( found )
			return new KismoFileParserElementImpl( m_inStream, m_outStream, tval, (*m_inStream).tellg() );
		else
			return 0;
	}

	io::ParserNode::Impl* KismoFileParserImpl::firstChild( const String& name ) const
	{
		if ( (*m_inStream).tellg() != m_pos )
			(*m_inStream).seekg( m_pos ); //Make sure we are at the position of this node

		// Read up to the attribute end tab ')'
		(*m_inStream).ignore( Stream_SizeMax, attributeEndTag );

		String tval;
		readBlock( tval ); ///< Read what should be after the atribute close tag

		if ( tval[0] != elementStartTag ) //< Should be '{' if child data is supplied
			return 0; ///< No children!

		//Create the tag identifyer ( Name + '(' )
		String tname = name + attributeStartTag;

		//Find a data element which has an attribute start tag definiing it not as data!
		bool found = false;
		while ( (*m_inStream).good() && tval[0] != elementEndTag )
		{
			if ( tval == tname ) //If the tag matches
			{
				found = true;
				break; //< We found a child
			}
			else
			if ( !readBlock( tval ) ) break;
		}

		//check if we found an element
		if ( found )
			return new KismoFileParserElementImpl( m_inStream, m_outStream, name, (*m_inStream).tellg() );
		else
			return 0;
	}	

	io::ParserNode::Impl* KismoFileParserImpl::firstAttribute() const 
	{		
		if ( (*m_inStream).tellg() != m_pos )
			(*m_inStream).seekg( m_pos ); //Make sure we are at the position of this node

		String tval;
		if ( !readBlock( tval ) ) //Read in the first tag which should be the first attribute
		{
			return false;
		}

		if ( tval[0] == attributeValueStartTag ) //Skip attribute value
		{
			if ( tval[ tval.length() - 1 ] != attributeValueEndTag ) ///< If the entire value isn't read
				(*m_inStream).ignore( Stream_SizeMax, attributeValueEndTag ); //Skip it

			if ( !readBlock( tval ) )
			{
				assert( false );
				return 0;
			}
		}

		//check if we found an element
		if ( tval[0] == attributeEndTag )
			return 0;
		else
			return new KismoFileParserAttribImpl( m_inStream, m_outStream, tval, (*m_inStream).tellg() ); 
	}
	
	io::ParserNode::Impl* KismoFileParserImpl::firstAttribute( const String& name ) const 
	{			
		if ( (*m_inStream).tellg() != m_pos )
			(*m_inStream).seekg( m_pos ); //Make sure we are at the position of this node

		String tval;
		if ( !readBlock( tval ) ) return 0;
		
		bool found = false;
		//Find the first attribute of the value
		while ( (*m_inStream).good() && tval[0] != attributeEndTag )
		{
			if ( tval == name ) //If the attribute matches
			{
				found = true;
				break; //< We found a child
			}
			else
			if ( tval[0] == attributeValueStartTag ) //Skip attribute values
			{				
				if ( tval[ tval.length() - 1 ] != attributeValueEndTag ) ///< If the entire value isn't read			
					(*m_inStream).ignore( Stream_SizeMax, attributeValueEndTag ); //Skip it
			}

			if ( !readBlock( tval ) ) return 0;
		}

		//check if we found an attribute
		if ( tval[0] == attributeEndTag )
			return 0;
		else
			return new KismoFileParserAttribImpl( m_inStream, m_outStream, tval, (*m_inStream).tellg() ); 
	}	
	

	io::ParserNode::Impl* KismoFileParserElementImpl::next() const
	{		
		if ( (*m_inStream).tellg() != m_pos )
			(*m_inStream).seekg( m_pos ); //Make sure we are at the position of this node
		
		String tval;
		if ( !skipElement( tval ) ) //Skip the contents of this element
			return 0;

		if ( /*!readBlock( tval ) ///Read the next block which could be the next element if their is one
			||*/ tval[0] == elementEndTag )
		{
			return 0;
		}
		else
		{
			tval.trimRight( attributeStartTag );
			return new KismoFileParserElementImpl( m_inStream, m_outStream, tval, (*m_inStream).tellg() ); 			
		}
	}

	io::ParserNode::Impl* KismoFileParserElementImpl::next( const String& name ) const
	{	
		if ( (*m_inStream).tellg() != m_pos )
			(*m_inStream).seekg( m_pos ); //Make sure we are at the position of this node
		
		//Create the tag identifyer ( Name + '(' )
		String tname = name + attributeStartTag;

		String tval;
		bool found = false;
		while ( skipElement( tval ) ) //Skip the contents of this element
		{
			if ( /*!readBlock( tval ) ///Read the next block which could be the next element if there is one
				|| */tval[0] == elementEndTag )
				return 0;

			if ( tval == tname ) //If the tag matches
			{
				found = true;
				break; //< We found a child
			}
		}

		//check if we found an element
		if ( found )
			return new KismoFileParserElementImpl( m_inStream, m_outStream, name, (*m_inStream).tellg() );
		else
			return 0;
	}	
	
	io::ParserNode::Impl* KismoFileParserElementImpl::copy() const
	{
		return new KismoFileParserElementImpl( m_inStream, m_outStream, m_name, m_pos );
	}
	
	InStream* KismoFileParserElementImpl::getValueStream() const
	{
		if ( (*m_inStream).tellg() != m_pos )
			(*m_inStream).seekg( m_pos ); //Make sure we are at the position of this node

		//Read upto the end of the attributes
		(*m_inStream).ignore( Stream_SizeMax, attributeEndTag ); 

		String tval;
		if ( !readBlock( tval ) )
			return 0; //< Failed!
		
		if ( tval[0] == elementStartTag )
			return m_inStream;
		else
			return 0;
	}

	////---------------------------------------------------------------------------
	//// Attribute node implimentation
	////---------------------------------------------------------------------------
	KismoFileParserAttribImpl::KismoFileParserAttribImpl( InStream* inStream, OutStream* outStream, const String& name, KismoFileParser::StreamPos posNamePos  ) :
		KismoFileParserImpl( inStream, outStream, name, posNamePos ) 
	{
	}

	io::ParserNode::Impl* KismoFileParserAttribImpl::next() const
	{ return firstAttribute(); }

	io::ParserNode::Impl* KismoFileParserAttribImpl::next( const String& name ) const
	{ return firstAttribute( name ); }	

	io::ParserNode::Impl* KismoFileParserAttribImpl::copy() const
	{
		return new KismoFileParserAttribImpl( m_inStream, m_outStream, m_name, m_pos );
	}

	const Char* KismoFileParserAttribImpl::getValue() const
	{
		if ( m_value.isNull() )
		{
			if ( (*m_inStream).tellg() != m_pos )
				(*m_inStream).seekg( m_pos ); //Make sure we are at the position of this node

			//Read upto the start tag
			(*m_inStream).ignore( Stream_SizeMax, attributeValueStartTag ); 
			//Reserve space for the value
			m_value.reserve( 64 );
			//Read the 
			(*m_inStream).getline( (Char*)m_value.c_str(), 64, attributeValueEndTag );
		}

		return m_value.c_str();
	}
	
	InStream* KismoFileParserAttribImpl::getValueStream() const
	{
		if ( (*m_inStream).tellg() != m_pos )
			(*m_inStream).seekg( m_pos ); //Make sure we are at the position of this node

		//Read upto the start tag
		(*m_inStream).ignore( Stream_SizeMax, attributeValueStartTag );  

		if ( (*m_inStream).good() )
			return m_inStream;
		else
			return 0;
	}

	//---------------------------------------------------------------------------
	// KismoFileParser file parser implimentations
	//---------------------------------------------------------------------------	
	KismoFileParser::KismoFileParser( const String& filename ) :
		FileParser( filename ),
		m_fs( 0 )
	{
	}
	
	KismoFileParser::KismoFileParser( InStream& inputStream ) :
		FileParser( inputStream ),
		m_fs( 0 )
	{
	}
		
	KismoFileParser::KismoFileParser( OutStream& outputStream ) :
		FileParser( outputStream ),
		m_fs( 0 )
	{
	}

	KismoFileParser::KismoFileParser( Stream& ioStream ) :
		FileParser( ioStream ),
		m_fs( 0 )
	{
	}

	KismoFileParser::~KismoFileParser()
	{
		close();
	}

	bool KismoFileParser::open()
	{
		if ( is_open() )
			return true; //already open!
		
		m_fs = new FStream();
		m_inStream = m_fs;
		m_outStream = m_fs;

		m_fs->open( m_filename.c_str() );
		if ( !is_open() ) 
		{
			logError( "Failed to open KismoFileParser file '" << m_filename << "'" );	
			return false;
		}
		else
		{
			return true;
		}
	}

	bool KismoFileParser::is_open()
	{
		if ( m_fs && m_fs->is_open() )
			return m_fs->good() && !m_fs->eof();
		if ( m_inStream )
			return true;
		if ( m_outStream )
			return true;
		return false;
	}

	void KismoFileParser::close()
	{	
		if ( m_fs )
		{
			m_fs->close();
			frx::safeDelete( m_fs );
		}
	}

	io::ParserNode KismoFileParser::getRootNode()
	{
		if ( is_open() )
		{			
			assert( m_inStream );
			if ( !m_inStream->good() || m_inStream->eof() )
				m_inStream->clear();

			m_inStream->seekg( (StreamPos)0 );
			KismoFileParserElementImpl tnode( m_inStream, m_outStream, String(), (StreamPos)0 );

			String tval;
			if ( !tnode.readBlock( tval ) )
				return io::ParserNode( 0 );

			assert( tval[ tval.length() - 1 ] == attributeStartTag );
			if ( tval[ tval.length() - 1 ] != attributeStartTag )
				return io::ParserNode( 0 );
			
			tval.trimRight( attributeStartTag ); ///Read the root name
			return io::ParserNode( new KismoFileParserElementImpl( m_inStream, m_outStream, tval, m_inStream->tellg() ) );
		}
		else
			return io::ParserNode( 0 );
	}



} //END: rage
