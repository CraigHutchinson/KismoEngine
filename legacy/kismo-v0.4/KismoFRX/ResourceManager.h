/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoFRX\ResourceManager.h
 * Status: current (newest copy of this file name in the archive)
 */
#ifndef KISMOFRX_RESOURCEMANAGER_H
#define	KISMOFRX_RESOURCEMANAGER_H

#include "KismoFRX/Common.h"
#include "KismoFRX/Resource.h"

namespace kismo
{
	/** \brief ResourceManager base class 
	*/
	class KISMOFRX_EXPORT ResourceManager
	{
	public:

		virtual ~ResourceManager()
		{
			removeAll();
		}

		/** \brief Register a resource translator for the manager to use when creating a resource
		\returns Whether the translator was added
		*/
		bool registerTranslator( ResourceTranslator* resource );  

		/** \brief Removes a translator from the manager and from all resources that reference it
		*/
		bool removeTranslator( ResourceTranslator* resource ); 

		/** \brief specifies the maximum memory usage of the resources in the manager
		\remarks 
			Setting this too low could cause issues 
		*/
		virtual void setMemoryBudget( Size bytes ) {}

		/** \brief Unload a resource by name
		\param[in] name The name of the resource
		*/
		void unload( const String& name );

		/** \brief Unload a resource by uid
		\param[in] uid The unique identifier for the resource
		*/
		void unload( const Resource::UID uid );
		
		/** \brief Unload resources which haven't been 'touched' by a reference
		\param[in] reloadableOnly Sets that only resources that can be re-loaded will be unloaded
		*/
		void unloadUnreferenced( bool reloadableOnly = false );

		/** \brief Unload all resources
		\param[in] reloadableOnly Sets that only resources that can be re-loaded will be unloaded
		*/
		void unloadAll( bool reloadableOnly = false );

		/** \brief Reload (Unload+reload) a resource by name
		\param[in] name The name of the resource
		\remarks
			If the resource is not relodable then false will be returned
		*/
		bool reload( const String& name );
		
		/** \brief Reload (Unload+reload) a resource by uid
		\param[in] uid The unique identifier for the resource
		\remarks
			If the resource is not relodable then false will be returned
		*/
		bool reload( const Resource::UID uid );

		/** \brief Reload (Unload+reload) all resources
		\param[in] reloadableOnly Sets that only resources that can be re-loaded will be reloaded
		\returns Whether all resources could be reloaded
		*/
		bool reloadAll( bool reloadableOnly = true );

		/** \brief Remove a resource by name
		\param[in] name The name of the resource
		\returns Whether the reosurce exists and was removed
		*/
		bool remove( const String& name );
		
		/** \brief Remove a resource by uid
		\param[in] uid The unique identifier for the resource
		\returns Whether the reosurce exists and was removed
		*/
		bool remove( const Resource::UID uid );

		/** \brief Remove resources which haven't been 'touched' by a reference (SmartPointer)
		\returns The number of resources removed
		*/
		Count removeUnreferenced();

		/** \brief remove all resources
		*/
		void removeAll(); 

		/** \brief Get the uid of a resource form its name
		\param[in] name The name of the resource
		*/
		Resource::UID getUID( const String& name );
		
		/** \brief Get a resource from its uid
		\param[in] uid The unique identifier for the resource
		\returns A smart pointer to the resource
		*/
		Resource::Ptr getResource( const Resource::UID uid );

		/** \brief Get a resource from its name
		\param[in] name The name of the resource
		\returns A smart pointer to the resource
		*/
		Resource::Ptr getResource( const String& name )
		{ return getResource( getUID( name ) ); }

		/** \brief Checks whether a resource exists
		\param[in] name The name of the resource
		\returns Whether the resource exists
		*/
		bool exists( const String name ) 
		{ return isValidHandle( getUID( name ) ); }
		
		/** \brief Checks whether a resource exists
		\param[in] uid The unique identifier for the resource
		\returns Whether the resource exists
		*/
		bool exists( const Resource::UID uid ) 
		{ return getResource( uid ).isValid(); }

		/** \brief Get a translator for a specified resource file
		*/
		ResourceTranslator* getTranslator( const String& filename ) const; 

	protected:

		/** \brief Added a resource imstance to the manager
		*/
		Bool ResourceManager::addImpl( Resource* resource );

	protected:
		typedef Set< ResourceTranslator* > ResourceTranslatorSet;
		typedef std::map< Resource::UID, Resource::Ptr > ResourceUIDMap;
		typedef std::map< String, Resource::UID > ResourceNameMap;

		ResourceUIDMap m_resources; ///< Resources stored in a map index by uid
		ResourceNameMap m_resourceUID; ///< Map used to associate a Name to a resource uid
		ResourceTranslatorSet m_resourceTranslators; ///< vector of avilable resource translators

		Size m_memoryBudget; ///< The maximum amount of memory (in bytes) this resource manager can use
        Size m_memoryUsage; ///< The current memory usage for this resource manager

	};

} //END: kismo

#endif
