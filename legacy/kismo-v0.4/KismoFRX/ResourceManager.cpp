/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoFRX\ResourceManager.cpp
 * Status: current (newest copy of this file name in the archive)
 */
#include "ResourceManager.h"
#include "KismoFRX/Log.h"

namespace kismo
{		
	bool ResourceManager::registerTranslator( ResourceTranslator* resource )
	{
		if ( m_resourceTranslators.find( resource ) != m_resourceTranslators.end() )
		{
			assert( false );
			return false;
		}

		m_resourceTranslators.insert( resource );
		return true;
	}

	
	bool ResourceManager::removeTranslator( ResourceTranslator* resource )
	{
		ResourceTranslatorSet::iterator i = m_resourceTranslators.find( resource );
		
		if ( i != m_resourceTranslators.end() )
		{
			m_resourceTranslators.erase( i );
			return true;
		}
		//TODO: get any resources that use this translator and clear the m_creator to 0
		return false;
	}
	
	ResourceTranslator* ResourceManager::getTranslator( const String& filename ) const
	{
		ResourceTranslator* bestTranslator = 0;

		InFStream istream( filename.c_str() );
		if ( !istream.is_open() )
		{
			logError( _T("Resource file '") << filename << _T("' could not be opened for reading") );
			return 0; //Failure!
		}

		const Size BUFFER_SIZE = 48;
		Char buff[ BUFFER_SIZE ];
		istream.read( buff, BUFFER_SIZE );

		for ( ResourceTranslatorSet::const_iterator i = m_resourceTranslators.begin(); i != m_resourceTranslators.end(); ++i )
		{
			//get how well the file matches this translator
			ResourceTranslator::eFileMatch match = (*i)->identifyFile( filename, buff, BUFFER_SIZE );

			switch ( match )
			{
			case ResourceTranslator::FileMatch_NotMyFileType:
				break; ///do nothign!
			case ResourceTranslator::FileMatch_CouldBeMyFileType:
				bestTranslator = *i;
				break; ///do nothign!
			case ResourceTranslator::FileMatch_IsMyFileType:
				return *i; ///No point carrying on as we have the best match
			}
		}

		return bestTranslator;
	}

	Bool ResourceManager::addImpl( Resource* resource )
	{
		if ( !resource )
		{
			assert( false );
			logRecoverable( "Resource load failed, 'null' resource supplied" );
			return false;
		}

		if ( exists( resource->getUID() )  )
		{
			logRecoverable( "Resource load failed as a resource called '" << resource->getName() << "' already exists" );
			return false;
		}

		m_resources[ resource->getUID() ] = resource;
		
		//Store the resource pointer in the 
		m_resourceUID[ resource->getName() ] = resource->getUID();

		logSuccess(  "Resource load Succeded '" << resource->getName() << "'" );

		return true;
	}


	//-----------------------------------------------------
	// Unload resources - does not remove but saves memory
	//-----------------------------------------------------
	void ResourceManager::unload( const String& name )
	{
	}
	void ResourceManager::unload( const Resource::UID handle )
	{
	}
	void ResourceManager::unloadUnreferenced( bool reloadableOnly )
	{
	}
	void ResourceManager::unloadAll( bool reloadableOnly )
	{
		for ( ResourceUIDMap::iterator iResource = m_resources.begin(); iResource != m_resources.end(); ++iResource )
		{
			iResource->second->unload();
		}
	}

	//-----------------------------------------------------
	// Reload resources - reloads a resource making sure it upto date with any file changes
	//-----------------------------------------------------
	bool ResourceManager::reload( const String& name )
	{
		return false;
	}

	bool ResourceManager::reload( const Resource::UID handle )
	{
		return false;
	}

	bool ResourceManager::reloadAll( bool reloadableOnly )
	{
		return false;
	}

	//-----------------------------------------------------
	// Remove resources - removes a resource permanately
	//-----------------------------------------------------
	Bool ResourceManager::remove( const String& name )
	{
		return false;

	}
	Bool ResourceManager::remove( const Resource::UID uid )
	{	
		ResourceUIDMap::iterator iUID = m_resources.find( uid );
		if ( iUID == m_resources.end() )
		{
			return false;
		}

		ResourceNameMap::iterator iName = m_resourceUID.find( iUID->second->getName() );

		m_resources.erase( iUID );

		if ( iName == m_resourceUID.end() )
		{
			assert( false );
			return false;
		}

		m_resourceUID.erase( iName );

		return true;
	}
	Count ResourceManager::removeUnreferenced()
	{
		return 0;
	}
	void ResourceManager::removeAll()	
	{		
		unloadAll( false );

		m_resources.clear();
		m_resourceUID.clear();
	}

	//-----------------------------------------------------
	// Gets
	//-----------------------------------------------------
	Resource::UID ResourceManager::getUID( const String& name )
	{
		ResourceNameMap::const_iterator it = m_resourceUID.find( name );
		if ( it != m_resourceUID.end() )
			return it->second;
		else
			return 0;
	}

	Resource::Ptr ResourceManager::getResource( const Resource::UID uid )
	{ 
		ResourceUIDMap::const_iterator it = m_resources.find( uid );
		if ( it != m_resources.end() )
			return it->second;
		else
			return 0;
	}

} //END: kismo
