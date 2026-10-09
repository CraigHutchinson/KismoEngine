/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoAI\FrxaiLmlFileParser.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
/** \author Craig Hutchinson \file 
	\brief 
*/
#include "FrxaiLmlFileParser.h"
#include "FileParserImpl.h"
#include "Log.h"

const Char newLineChar = '\n';
const Char commentLineTag = ';';
const Char commentBlockTag = '#';

const Char elementStartTag = '(';
const Char elementEndTag = ')';

namespace frxai
{
	class LmlImpl : public io::ParserNode::Impl
	{
	public:
		LmlImpl( LmlFileParser::Stream& fs, LmlFileParser::StreamPos pos  ) :
		  m_fs( fs ), m_pos( pos )
		{}

		LmlImpl( LmlFileParser::Stream& fs, const String& name, LmlFileParser::StreamPos pos  ) :
		  m_fs( fs ), m_pos( pos ), m_name( name )
		{}

		virtual const Char* getName() const;
		virtual const Char* getValue() const;
		virtual Impl* firstChild() const;
		virtual Impl* firstChild( const String& name ) const;
		virtual Impl* firstAttribute() const;	
		virtual Impl* firstAttribute( const String& name ) const;	

	protected:
		
		Bool readBlock( String& tval ) const;

	protected:
		mutable LmlFileParser::Stream& m_fs; ////< The fie stream being loaded from
		LmlFileParser::StreamPos m_pos; ////< The position in the file just after this nodes name
		String m_name; ////< The name of the node
	};

	//---------------------------------------------------------------------------
	// Node implimentation definitions
	//---------------------------------------------------------------------------
	/** \brief File parser attributes
	*/
	class LmlAttribImpl : public LmlImpl
	{
	public:
		LmlAttribImpl( LmlFileParser::Stream& fs, const String& name, LmlFileParser::StreamPos postNamePos );
	
		virtual Impl* next() const;	
		virtual Impl* next( const String& name ) const;
		virtual Impl* copy() const;

	};

	/** \brief File parser attributes
	*/
	class LmlElementImpl : public LmlImpl
	{
	public:
		/** \brief Constructor for reading an element from a file after the name tag with the name supplied
		*/
		LmlElementImpl( LmlFileParser::Stream& fs, const String& name, LmlFileParser::StreamPos postNamePos );
	
		virtual Impl* next() const;	
		virtual Impl* next( const String& name ) const;
		virtual Impl* copy() const;
		virtual const Char* getValue() const;

	protected:
		mutable String m_value; ////< The value of the node which is only filled when getValue() is called

	};

	//---------------------------------------------------------------------------
	// base node implimentation
	//---------------------------------------------------------------------------
	Bool LmlImpl::readBlock( String& tval ) const
	{
		bool comment;
		do
		{
			m_fs >> tval;
			if ( tval[0] == commentLineTag )
			{
				m_fs.ignore( 4048, newLineChar);
				comment = true;
			}	
			else
			if ( tval[0] == commentBlockTag )
			{
				m_fs.ignore( 4048, commentBlockTag);
				comment = true;
			}
			else
				comment = false;
		}
		while ( comment );

		//assert( m_fs.good() );
		return m_fs.good();
	}

	//---------------------------------------------------------------------------
	// Element node implimentation
	//---------------------------------------------------------------------------
	LmlElementImpl::LmlElementImpl( LmlFileParser::Stream& fs, const String& name, LmlFileParser::StreamPos posNamePos  ) :
		LmlImpl( fs, name, posNamePos ) 
	{
		assert( m_name[0] != elementEndTag ); ///make sure the ead position is valid!
		assert( m_name[0] != elementStartTag ); ///make sure the ead position is valid!
	}

	const Char* LmlImpl::getName() const
	{
		return (const Char*)m_name.c_str();
	}

	const Char* LmlImpl::getValue() const
	{
		assert( false );
		return 0;
	}

	io::ParserNode::Impl* LmlImpl::firstChild() const
	{		
		if ( m_fs.tellg() != m_pos )
			m_fs.seekg( m_pos ); //Make sure we are at the position of this node

		String tval;
		//Find the start of the next element that is not past the end tag of this node!
		while ( tval[0] != elementStartTag && tval[0] != elementEndTag && m_fs.good() )
			readBlock( tval );

		//check if we found an element
		if ( tval[0] == elementStartTag )
		{
			readBlock( tval ); //Read the name of the element
			return new LmlElementImpl( m_fs, tval, m_fs.tellg() ); 
		}
		else
			return 0;
	}

	io::ParserNode::Impl* LmlImpl::firstChild( const String& name ) const
	{
		if ( m_fs.tellg() != m_pos )
			m_fs.seekg( m_pos ); //Make sure we are at the position of this node

		String tval;
		UInt8 level = 0; //Stores the level of element indentation
		while( tval != name ) ///Loop until we find the name... or bail
		{
			while ( level && m_fs.good() )
			{
				readBlock( tval );	
				if ( tval[0] == elementStartTag )
					++level;
				else
				if ( tval[0] == elementEndTag )
					--level;
			}

			//Find the start for the next element
			while ( tval[0] != elementStartTag && tval[0] != elementEndTag && m_fs.good() )
				readBlock( tval );

			if ( tval[0] == elementStartTag )
			{
				readBlock( tval ); //Get the elements name tag
				++level; //Set that the read position is at a sub level of the node
			}
			else
			{
				return 0; //No element found!
			}
		}

		// we found an element
		assert( tval == name );
		return new LmlElementImpl( m_fs, tval, m_fs.tellg() ); 
	}	

	io::ParserNode::Impl* LmlImpl::firstAttribute() const 
	{		
		if ( m_fs.tellg() != m_pos )
			m_fs.seekg( m_pos ); //Make sure we are at the position of this node

		String tval;
		readBlock( tval ); //Read in the first tag which should be the first attribute

		//check if we found an element
		if ( tval[0] == elementStartTag || tval[0] == elementEndTag )
			return 0;
		else
			return new LmlAttribImpl( m_fs, tval, m_fs.tellg() ); 
	}
	
	io::ParserNode::Impl* LmlImpl::firstAttribute( const String& name ) const 
	{			
		if ( m_fs.tellg() != m_pos )
			m_fs.seekg( m_pos ); //Make sure we are at the position of this node

		String tval;
		while ( tval[0] != elementStartTag && tval[0] != elementEndTag && m_fs.good() )
		{
			readBlock( tval ); //Read in the next tag
					
			if ( tval == name ) //Found the atribute
				return new LmlAttribImpl( m_fs, tval, m_fs.tellg() ); 
		}

		//We failed!
		return 0;
	}	
	

	io::ParserNode::Impl* LmlElementImpl::next() const
	{		
		if ( m_fs.tellg() != m_pos )
			m_fs.seekg( m_pos ); //Make sure we are at the position of this node

		String tval;
		Int8 level = 0, outCount = 0; //Stores the level of element indentation

		//Loop until we get the end tag of this element (outCount == 1) then in the next (level == 0)
		while ( m_fs.good() && outCount <= 1 )
		{	
			if ( !readBlock( tval ) )
				return 0;

			if ( tval[0] == elementStartTag ) 
			{
				if ( ++level == 0 ) break;//If we have moved into this new node fromt he parent i.e. next node!
			}
			else if ( tval[0] == elementEndTag ) 
			{
				if ( --level < 0 ) 
				{
					if ( ++outCount > 1 ) break;
				}
			}

		}	

		if ( level == 0 ) //We found an element
		{
			readBlock( tval ); //Read in the next tag
			return new LmlElementImpl( m_fs, tval, m_fs.tellg() ); 
		}
		else//No next element!			
			return 0;
	}

	const Char* LmlElementImpl::getValue() const
	{
		if ( m_value.length() == 0 )
		{

			if ( m_fs.tellg() != m_pos )
				m_fs.seekg( m_pos ); //Make sure we are at the position of this node

			Char buffer[256];
			//Read upto the end tag of the element. The value will therefore include all the sub elements but we can ignore that for now
			m_fs.getline( buffer, 256, elementEndTag );

			m_value = buffer;
		}
		
		return m_value.c_str();
	}

	io::ParserNode::Impl* LmlElementImpl::next( const String& name ) const
	{
		if ( m_fs.tellg() != m_pos )
			m_fs.seekg( m_pos ); //Make sure we are at the position of this node

		String tval;
		Int8 level = 0, outCount = 0; //Stores the level of element indentation

		//Loop until we get the end tag of this element (outCount == 1) then in the next (level == 0)
		while ( m_fs.good() && outCount <= 1 && !(outCount == 1 && level == 0 && tval == name) )
		{
			readBlock( tval );	
			if ( tval[0] == elementStartTag ) 
			{
				++level;				
				if ( level == 0 ) 
				{
					outCount = 0; ///Reset the out counter!
					readBlock( tval ); //Get the name of the element
				}
			}
			else if ( tval[0] == elementEndTag ) 
			{
				--level;
				if ( level < 0 ) ++outCount;
			}
		}	

		if ( level == 0 && tval == name ) //We found an element
		{
			return new LmlElementImpl( m_fs, tval, m_fs.tellg() ); 
		}
		else//No next element!			
			return 0;
	}	
	
	io::ParserNode::Impl* LmlElementImpl::copy() const
	{
		return new LmlElementImpl( m_fs, m_name, m_pos );
	}

	////---------------------------------------------------------------------------
	//// Attribute node implimentation
	////---------------------------------------------------------------------------
	LmlAttribImpl::LmlAttribImpl( LmlFileParser::Stream& fs, const String& name, LmlFileParser::StreamPos posNamePos  ) :
		LmlImpl( fs, name, posNamePos ) 
	{
		assert( m_name[0] != elementEndTag ); ///make sure the ead position is valid!
		assert( m_name[0] != elementStartTag ); ///make sure the ead position is valid!
	}

	io::ParserNode::Impl* LmlAttribImpl::next() const
	{ return firstAttribute(); }

	io::ParserNode::Impl* LmlAttribImpl::next( const String& name ) const
	{ return firstAttribute( name ); }	

	io::ParserNode::Impl* LmlAttribImpl::copy() const
	{
		return new LmlAttribImpl( m_fs, m_name, m_pos );
	}

	
	//---------------------------------------------------------------------------
	// Lml file parser implimentations
	//---------------------------------------------------------------------------	
	LmlFileParser::LmlFileParser( const String& filename ) :
		FileParser( filename )
	{
	}

	LmlFileParser::~LmlFileParser()
	{
		close();
	}

	bool LmlFileParser::open()
	{
		if ( is_open() )
			return true; //already open!
		
		m_fs.open( m_filename.c_str() );
		if ( !is_open() ) 
		{
			logError( "Failed to open Lml file '" << m_filename << "'" );	
			return false;
		}
		else
		{
			return true;
		}
	}

	bool LmlFileParser::is_open()
	{
		return m_fs.is_open();
	}

	void LmlFileParser::close()
	{		
		m_fs.close();
	}

	io::ParserNode LmlFileParser::getRootNode()
	{
		if ( is_open() )
		{			
			if ( m_fs.tellg() != (StreamPos)0 )
				m_fs.seekg( (StreamPos)0 ); //Make sure we are at the start of the file

			String tval;
			while ( m_fs.good() && tval[0] != elementStartTag )
				m_fs >> tval;

			m_fs >> tval; ///Read the root name
			return io::ParserNode( new LmlElementImpl( m_fs, tval, m_fs.tellg() ) );
		}
		else
			return io::ParserNode( 0 );
	}



} //END: frxai
