/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\KismoCore\_SOURCE_\KismoFramework\MacroManager.h
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

#ifndef KISMOFRAMEWORK_MACROMANAGER_H
#define KISMOFRAMEWORK_MACROMANAGER_H

#include "KismoFramework\Common.h"
#include "KismoFramework/Types.h"
#include "KismoFramework/String.h"
#include <map>

namespace kismo
{	
	//Forward declarations
	class MacroNamespace;

	//TODO: do we really need the macro manager. Namespaces could create nere namespaces etc
	//TODO: possible the manager is just for convenience in programatically creating namespaces when parsing files!?

	/** The Macro manager performs macro expansion of string values;
		\brief macros are expanded using the same format as Visual Studio. e.g.
			macros: "Weather" = "sunny", "feeling" = "nice"
			input: "The day is $(Weather) today which is $(feeling)"
			output "The weather is sunny today which is nice"

		Macros are defined in 'namespaces' and a child namespace can override/hide the value fo a parent namespace.
		This is performed in a sinilar value to variable names in most programming languages.
	*/
	class MacroManager
	{
	public:
		MacroManager();

		MacroNamespace* defaultNamespace() 
		{ return m_defaultNamespace; }
		
		MacroNamespace* findNamespace( const String& name )  
		{ 
			NamespaceLookup::const_iterator iFind = m_namespaces.find( name );
			return iFind != m_namespaces.end() ? iFind->second : 0;
		}

		MacroNamespace* findNamespace( const Char* name, Count nameLength )
		{ return findNamespace( String(name,nameLength) ); }

		/** Creates a new namespace which has the default-namespace as its parent namespace
		*/
		inline MacroNamespace* createNamespace( const String& name );
		inline MacroNamespace* createNamespace( const Char* name );
		inline MacroNamespace* createNamespace( const Char* name, Count nameLength );

		/** Creates a new namespace which has a parent namespace looked up by name
			\returns The new namespace or 0 if the parentNamespace does not exist
		*/
		MacroNamespace* createNamespace( const String& name, const String& parentNamespace );
		inline MacroNamespace* createNamespace( const Char* name, const Char* parentNamespace );
		inline MacroNamespace* createNamespace( const Char* name, Count nameLength, const Char* parentNamespace, Count parentNamespaceLength  ) ;

		MacroNamespace* createNamespace( const String& name, MacroNamespace* parentNamespace );
		inline MacroNamespace* createNamespace( const Char* name, MacroNamespace* parentNamespace );
		inline MacroNamespace* createNamespace( const Char* name, Count nameLength, MacroNamespace* parentNamespace  ) ;

	protected:
		typedef std::map<String,MacroNamespace*> NamespaceLookup;
#pragma warning(disable:4251)
		NamespaceLookup m_namespaces;
#pragma warning(default:4251)
		MacroNamespace* m_defaultNamespace;
	};
		
	MacroNamespace* MacroManager::createNamespace( const String& name ) 
	{ return createNamespace( name, m_defaultNamespace ); }

	MacroNamespace* MacroManager::createNamespace( const Char* name )
	{ return createNamespace( String(name)); }

	MacroNamespace* MacroManager::createNamespace( const Char* name, Count nameLength ) 
	{ return createNamespace( String(name, nameLength )); }

	MacroNamespace* MacroManager::createNamespace( const Char* name, const Char* parentNamespace )
	{ return createNamespace( String(name), String(parentNamespace)); }

	MacroNamespace* MacroManager::createNamespace( const Char* name, MacroNamespace* parentNamespace )
	{ return createNamespace( String(name), parentNamespace ); }
	
	MacroNamespace* MacroManager::createNamespace( const Char* name, Count nameLength, MacroNamespace* parentNamespace )
	{ return createNamespace( String(name,nameLength), parentNamespace ); }

} //END: kismo

#endif