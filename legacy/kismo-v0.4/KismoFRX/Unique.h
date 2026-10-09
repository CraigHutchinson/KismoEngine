/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoFRX\Unique.h
 * Status: superseded by kismo-core/_SOURCE_/KismoFramework/Unique.h (same name, content differs)
 */
#ifndef UNIQUE_H
#define UNIQUE_H

#include "KismoFRX/Types.h"
//#include "Platform/Platform.h"

#define IMPLEMENT_UNIQUE( _className ) \
	template<> kismo::Unique< _className >::UID kismo::Unique< _className >::ms_nextLowerID = 0;

namespace kismo
{
	/** \brief Base unique is used to dynamically create a new unique uppper ID for unique objects
	*/
	class KISMOFRX_EXPORT UniqueUpperBase
	{
	public:
		UniqueUpperBase() { ms_nextUpperID++; }
		virtual ~UniqueUpperBase() {}
	protected:
		static UInt8 ms_nextUpperID; ///< Defines the upper identifier for new types
	};

	/** \brief Defines an interface for a uniquely identifiable object (UID)  
	*/
	template< class T >
	class Unique  : private UniqueUpperBase
	{
	public:

		typedef UInt32 UID;

	public:

		/** \brief Constructor initialises the unique object ID
		*/
		Unique() : 
			m_uid( (ms_nextUpperID << 24) | ms_nextLowerID++ ) 
		{}

		virtual ~Unique() {}
		
		/** \brief Returns the unique ID (UID) of the object 
		*/
		UInt32 getUID() const	
		{ return m_uid; }	
	

	private:
		UID m_uid; ///< The unique identifier for each instance
		static UID ms_nextLowerID; ///< Defines the lower identifier for new instances
	};


} //END: kismo

#endif