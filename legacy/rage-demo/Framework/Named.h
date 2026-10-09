/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\Framework\Named.h
 * Status: superseded by kismo-core/_SOURCE_/KismoFramework/Named.h (same name, content differs)
 */
#ifndef NAMED_H
#define NAMED_H

#include "Framework/Types.h"
#include "Platform/Platform.h"

namespace frx
{
	/** \brief Defines an interface for a 'named' object
	\remarks The name can only be set in the constructor, derive to a custom type to access setName() */
	class _DllExport Named
	{
	public:

		/** \brief Constructor
		\param[in] name Specifies the name of the object 
		*/
		Named( const String& name ) : 
			m_name( name ) 
		{}

		virtual ~Named() {}
		
		/** \brief Returns the name of the object 
		*/
		const String& getName() const	
		{ return m_name; }	
	
	protected:

		/** \brief Protected setName() so derived classes can change the name if required 
			\param[in] the new name of the object 
		*/
		void setName( const String& name )
		{ m_name = name; }	

	private:
		String m_name; ///< Holds the name of the object
	};


} //END: frx

#endif