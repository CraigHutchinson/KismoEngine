/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\KismoCore\KismoFramework\Named.h
 * Status: duplicate of kismo-core/_SOURCE_/KismoFramework/Named.h
 */
/*^**************************************************************************
*	
*	Kismo Engine 
*	© 2008-2013 Craig Hutchinson (craig@craighutchinson.co.uk)
*	All right reserved
*
*	This file is subject to the terms and conditions defined in
*	file 'license.txt', which is part of this source code package.
*
***************************************************************************^*/

#ifndef KISMOFRAMEWORK_NAMED_H
#define KISMOFRAMEWORK_NAMED_H

#include "KismoFramework\Common.h"
#include "KismoFramework/Types.h"
#include "KismoFramework/String.h"
//#include "Platform/Platform.h"

namespace kismo
{
	/** \brief Defines an interface for a 'named' object
	\remarks The name can only be set in the constructor, derive to a custom type to access setName() */
	class Named
	{
	public:

		/** \brief Constructor
			\param[in] name Specifies the name of the object 
		*/
		Named( const Char* name ) : 
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


} //END: kismo

#endif