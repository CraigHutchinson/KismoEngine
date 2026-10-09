/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\IO\FileParser.cpp
 * Status: duplicate of kismo-v0.3/IO/FileParser.cpp
 */
#include "FileParser.h"
#include "FileParserImpl.h"
#include "Framework/Common.h"

//const Char xTag[] = "x";
//const Char yTag[] = "y";
//const Char widthTag[] = "w";
//const Char heightTag[] = "h";

namespace io
{
	ParserNode::ParserNode( Impl* impl ) : 
		m_impl( impl )
	{
#ifdef _DEBUG
		m_debugName = (m_impl? m_impl->getName():0);
#endif
	}
	
	ParserNode::ParserNode( const ParserNode& r )
	{
		if ( r.isValid() ) 
			m_impl = r.m_impl->copy();
		else
			m_impl = 0;

#ifdef _DEBUG
		m_debugName = (m_impl? m_impl->getName():0);
#endif
	}

	ParserNode::~ParserNode()
	{ frx::safeDelete( m_impl ); }

	ParserNode& ParserNode::operator = ( const ParserNode& r )
	{ 
		frx::safeDelete( m_impl ); 
		if ( r.isValid() ) 
			m_impl = r.m_impl->copy(); 
		
#ifdef _DEBUG
		m_debugName = (m_impl? m_impl->getName():0);
#endif
		return *this;
	} 

	const Char* ParserNode::getName() const 
	{ return (m_impl? m_impl->getName() : 0 ); }

	const Char* ParserNode::getValue() const 
	{ return (m_impl? m_impl->getValue() : 0 ); }
	
	InStream* ParserNode::getValueStream() const 
	{ return (m_impl? m_impl->getValueStream(): 0 ); }

	ParserNode ParserNode::firstChild() const 
	{ return ParserNode( m_impl->firstChild() ); }

	ParserNode ParserNode::firstChild( const String& name) const 
	{ return ParserNode( m_impl->firstChild( name ) ); }

	ParserNode ParserNode::firstAttribute() const 
	{ return ParserNode( m_impl->firstAttribute() ); }

	ParserNode ParserNode::firstAttribute( const String& name ) const 
	{ return ParserNode( m_impl->firstAttribute( name ) ); }

	ParserNode ParserNode::next() const 
	{ return ParserNode( m_impl->next() ); }

	ParserNode ParserNode::next( const String& name ) const 
	{ return ParserNode( m_impl->next( name ) ); }

	ParserNode::operator Int16() const
	{ return _tstoi( getValue() ); }

	ParserNode::operator UInt16() const
	{ return _tstoi( getValue() ); }

	ParserNode::operator Float32() const  
	{ return (Float32)_tstof( getValue() ); }

	Bool ParserNode::operator ==( const String& rhs ) const 
	{ return rhs == getName(); }

	Bool ParserNode::operator !=( const String& rhs ) const 
	{ return rhs != getName(); }

	Bool ParserNode::operator ==( const Char* rhs ) const 
	{ 
		const Char* chr = getName();
		if ( chr && rhs )
			return _tcscmp( chr, rhs) == 0;
		else
			return false;
	}

	Bool ParserNode::operator !=( const Char* rhs ) const 
	{ return !(*this == rhs); }

	//frx::Rect2i ParserNode::parseRect2i() const
	//{
	//	frx::Rect2i rect;
	//	for ( io::FileParser::Node attr = firstAttribute(); attr.isValid(); attr = attr.next() )
	//	{
	//		if ( attr == xTag )		rect.pos.x = attr;
	//		else if ( attr == yTag )	rect.pos.y = attr;
	//		else if ( attr == widthTag )	rect.size.w = attr;
	//		else if ( attr == heightTag )	rect.size.h = attr;
	//	}		
	//	return rect;
	//}

	//frx::Point2i ParserNode::parsePoint2i() const
	//{
	//	frx::Point2i point;
	//	for ( io::FileParser::Node attr = firstAttribute(); attr.isValid(); attr = attr.next() )
	//	{
	//		if ( attr == xTag )
	//			point.x = attr;
	//		else
	//		if ( attr == yTag )
	//			point.y = attr;
	//	}		
	//	return point;
	//}

	//------------------------------------------------------
	// File parser base implementation
	//------------------------------------------------------	
	FileParser::FileParser( const String& filename ) :
		m_filename( filename ),
		m_inStream( 0 ),
		m_outStream( 0 )
	{
	}

	FileParser::FileParser( InStream& inputStream ) :
		m_inStream( &inputStream ),
		m_outStream( 0 )
	{ }
		
	FileParser::FileParser( OutStream& outputStream ) :
		m_inStream( 0 ),
		m_outStream( &outputStream )
	{ }
		
	FileParser::FileParser( Stream& ioStream ) :
		m_inStream( &ioStream ),
		m_outStream( &ioStream )
	{ }

	FileParser::~FileParser()
	{
	}

	bool FileParser::open( const String& filename )
	{
		m_filename = filename;
		return open();
	}

	bool FileParser::open( InStream& inputStream )
	{
		m_inStream = &inputStream;
		m_outStream = 0;
		return open();
	}

	bool FileParser::open( OutStream& outputStream )
	{
		m_inStream = 0;
		m_outStream = &outputStream;
		return open();
	}

	bool FileParser::open( Stream& ioStream )
	{
		m_inStream = &ioStream;
		m_outStream = &ioStream;
		return open();
	}

	ParserNode FileParser::getRootNodeByName( const String& name )
	{
		ParserNode root = getRootNode();
		while ( root.isValid()  && root.getName() != name  )
		{ 
			root = root.next();
		}
		
		if ( root.getName() == name )
			return root;
		else
			return ParserNode( 0 );
	}
} //END: io

