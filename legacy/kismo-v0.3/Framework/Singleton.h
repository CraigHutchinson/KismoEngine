/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\Framework\Singleton.h
 * Status: superseded by kismo-core/_SOURCE_/KismoFramework/Singleton.h (same name, content differs)
 */
#ifndef SINGLETON_H
#define	SINGLETON_H

#include "Framework/Types.h"

/** \brief Defines the singleton functions in a derived class so they work across DLL boundaries
*/
#define SINGLETON( _className )	\
	static _className& getSingleton( void );\
	static _className* getSingletonPtr( void );

/** \brief macros for implimenting the singleton functions so that they work across DLL boundaries
*/
#define IMPL_SINGLETON_INSTANCE( _className ) \
	template<> _className* frx::Singleton< _className, false >::ms_instance = 0;
#define IMPL_SINGLETON( _className ) IMPL_SINGLETON_EX( _className, false )
#define IMPL_SINGLETON_EX( _className, _multiInstance ) \
	template<> _className* frx::Singleton< _className, _multiInstance >::ms_instance = 0; \
	_className& _className::getSingleton( void ) \
	{ assert( ms_instance != 0 ); return ( *ms_instance ); } \
	_className* _className::getSingletonPtr( void ) \
	{ return ms_instance;  }

namespace frx
{		
	/*
		Example to create a singleton from this simply do:
			class CLASSNAME : public Singleton< CLASSNAME >
			{...

		NOTE derived class should have this in their cpp:
			template<> CLASSNAME* Singleton< CLASSNAME >::ms_Singleton = 0;
	*/

	/** \brief Manual instanciated Singleton class
		\remarks By manual instanciation the lifetime of the instance can be managed like any normal variable
	*/
	template < class T, bool multiInstance = false > 
	class /*_DllExport*/ Singleton
	{
	public:
		Singleton()
		{
			if ( !multiInstance )
				assert( !ms_instance );  //Make sure the singleton is not created twice anywhere!          
			ms_instance = static_cast< T* >( this );
		}

		virtual ~Singleton( void )
		{  
			if ( !multiInstance )
			{
				assert( ms_instance ); //Make sure the singletone was created.... wierd if not
			}
			else
			if ( ms_instance == ( const T* )( this ) ) //Only set to 0 if the instance is current
				ms_instance = 0; 
		}

			
		static T& getSingleton( void ) 
		{ 
			assert( ms_instance != 0 ); 
			return ( *ms_instance ); 
		}
		
		static T* getSingletonPtr( void ) 
		{ 
			assert( ms_instance != 0 ); 
			return ms_instance;  
		}

	protected:
		static T* ms_instance; //Static instance pointer
	};
	
} // END: frx

#endif
