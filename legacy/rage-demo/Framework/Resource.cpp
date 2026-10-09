/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\Framework\Resource.cpp
 * Status: superseded by kismo-v0.4/KismoFRX/Resource.cpp (same name, content differs)
 */
#include "Resource.h"
#include "Log.h"

IMPLEMENT_UNIQUE( frx::Resource );


namespace frx
{
	IMPLEMENT_RTTI_BASE( frx, Resource );
	IMPLEMENT_RTTI_BASE( frx, ResourceTranslator );

	//void* Resource::operator new ( size_t  size )
	//{
	//	void * p = ::malloc(size);
	//	if (p == 0)
	//		logError( "AllocationFailed!" );  //instead of std::bad_alloc
	//	return p; 
	//}

	//void Resource::operator delete ( void* p )
	//{	
	//	::free( static_cast<Resource*>(p) );	
	//}
	
	ResourceTranslator::ResourceTranslator( Bool canRead, Bool canWrite, const String& defaultExtension ) :
		m_hasReadMethod( canRead ),
		m_hasWriteMethod( canWrite ),
		m_defaultExtension( defaultExtension )
	{
	}
	
	ResourceTranslator::~ResourceTranslator()
	{
	}

	Resource::Resource( ResourceManager* owner, const String& resourceID, ResourceTranslator* loader, const String& origin ) :
		Named( resourceID ),
		m_loader( loader ),
		m_owner( owner ),
		m_origin( origin ),
		m_size( 0 ),
		m_isLoaded( false ),
		m_canReload( true )
	{
		//assert( creator );
	}

} //END: frx
