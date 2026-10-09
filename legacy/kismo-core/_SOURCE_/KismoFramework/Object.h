/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\KismoCore\_SOURCE_\KismoFramework\Object.h
 * Status: current (newest copy of this file name in the archive)
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

#ifndef KISMOFRAMEWORK_OBJECT_H
#define KISMOFRAMEWORK_OBJECT_H

//#include "Platform/Platform.h"
#include "KismoFramework/SmartPointer.h"
#include "KismoFramework/Named.h"
#include "KismoFramework/Unique.h"
#include <map>

namespace kismo
{	
	/** \brief Obvject defines the base type for an object
	*/
	class Object :  /*public kismo::SmartReferenced,*/ public Named, public Unique< Object >
	{
	public:

		/*typedef kismo::SmartPointer< Object > Ptr;*/

		/** \brief Finds an object of the supplied name
		*/
		static Object* findObject( const String& name );

	public:

		virtual ~Object();

	protected:

		Object( const Char* name );

	private:
		typedef std::map< String, Object* > ObjectNameMap;
#pragma warning(disable:4251)
		static ObjectNameMap ms_objects;
#pragma warning(default:4251)
	};
} //END: kismo

#endif