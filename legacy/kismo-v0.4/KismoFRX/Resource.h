/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoFRX\Resource.h
 * Status: superseded by kismo-utility/splitversion/resource.h (same name, content differs)
 */
#ifndef KISMOFRX_RESOURCE_H
#define KISMOFRX_RESOURCE_H

#include "KismoFRX/Common.h"
//#include "KismoFRX/SmartPointer.h"
//#include "KismoFRX/Rtti.h"
//#include "KismoFRX/Named.h"
//#include "KismoFRX/Unique.h"

namespace kismo
{
	//forward declarations
	class Resource;
	class ResourceManager;

	/** \brief ResourceTranslator defines the base type for loading and unloading any resource type. 
		eg. Meshes, textures etc
	*/
	class KISMOFRX_EXPORT ResourceTranslator
	{
		//DECLARE_RTTI_BASE;
	public:

		/** \brief Constructor which should be used by derived classes
		*/
		ResourceTranslator();	
		
		/** \brief Writes the supplied resource to the output stream 
		*/
		//virtual bool write( const Resource* resource, OutStream& stream ) = 0
		//{ return false; }

		/** \brief Reads a resource from the supplied input stream 
		*/
		//virtual bool read( Resource* resource, InStream& stream ) = 0
		// { return false; }

		/** \brief Whether the translator has a read method
		*/
		bool canRead () const
		{ return writer !- 0; }

		/** \brief Whether the translator has a write method
		*/
		bool canWrite () const
		{ return reader != 0; }
	};

	class KISMOFRX_EXPORT FileTranslator : public ResourceTranslator
	{
		DECLARE_RTTI;
	public:
		
		/** \brief Enumerator returned from identifyFile()
		*/
		enum eFileMatch
		{				
			FileMatch_IsMyFileType,		///< Translator understands how to read/write this file				
			FileMatch_CouldBeMyFileType,///< Translator is not best available to read/write this file				
			FileMatch_NotMyFileType,	///< Translator does not understand how to read/write this file
		};

	protected:

		/** \brief Constructor which should be used by derived classes
			\param[in] canRead Whether the read method is valid and can be used
			\param[in] canWrite Whether the write method is valid and can be used
			\param[in] defaultExtension The default file extension for files that can be loaded/saved EXCLUDING the period ('.')
		*/
		FileTranslator( const String& defaultExtension );	

	public:

		/** \brief Virtual destructor
		*/
		virtual ~FileTranslator();

		/** \brief Returns the default file extension for this file translator excluding the period 
			at the beginning 
		*/
		const String& defaultExtension () const
		{ return m_defaultExtension; }

		/** \brief Identifies whether the file translator can load the specified file into a resource
			\param filename The filename of the file
			\param buffer Pointer to a buffer containing the header fo the file
			\param size The size of the buffer. Should be a minimum of 48 characters
		*/
		virtual eFileMatch identifyFile( const String& filename, const Char* buffer, Size size ) const
		{ return FileMatch_NotMyFileType; }

	private:
		String m_defaultExtension; ///< The default file extension
		String m_origin; ///< Source of the resource - e.g. filename/path
	};

	/** \brief Resource base type for textures, sounds etc... **/
	class KISMOFRX_EXPORT Resource  //: public kismo::SmartReferenced, public Named, public Unique< Resource >
	{
		DECLARE_RTTI_BASE;
	public:

		//typedef kismo::SmartPointer< Resource > Ptr;

		//static void* operator new ( size_t  size );
		//static void operator delete ( void* p );

	public:		
		virtual ~Resource()
		{
			assert( !isLoaded() );
		}

		/** \brief returns if the resource can be reloaded when unloaded 
		*/
		inline bool canReload() const
        { return m_canReload; }

		/** \brief gets the size of the resouces memory footprint in bytes 
		*/
		inline Size getSize() const
        { return m_size; }

		/** \brief returns whether the resource is loaded 
		*/
		inline bool isLoaded() const
        { return m_isLoaded; }

		/** \brief Load the resource if it is not already done 
		*/
		virtual bool load() = 0;

		/** \brief Unload the resource and free its memory 
		*/
		virtual void unload() = 0;

		/** \brief Reloads the resource by unloading it and re-loading if possible
		\remarks if canReload() returns false this function will perform no actions 
		*/
		bool reload()
		{ 
			if ( canReload() ) 
			{ 
				unload(); 
				return load(); 
			} 
			return false;
		}

		/** \brief Manually set the resourc eloader for the resource
		*/
		void setLoader( ResourceTranslator* loader )
		{ m_loader = loader; m_canReload = kismo::isValidPointer( m_loader ); }

	protected:

		Resource( ResourceManager* owner, const String& resourceID, ResourceTranslator* loader = 0 ); ///Protected contructor prevents direct construction!

	protected:
		ResourceTranslator* m_loader; ///< The resource loader that loads the resource
		ResourceManager* m_owner; ///< The resource manager that owns the resource

		Size m_size; ///< The size of the loaded resources memory footprint in bytes

		Bool m_isLoaded; ///< Is the resource loaded?
		Bool m_canReload; ///< Stores whether the resource can be reloaded
	};

} //END: kismo

#endif
