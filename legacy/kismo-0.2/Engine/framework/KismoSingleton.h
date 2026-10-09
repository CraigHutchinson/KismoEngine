/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\framework\KismoSingleton.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef _KISMO_SINGLETON_H
#define	_KISMO_SINGLETON_H

#include "KismoTypes.h"
#include <cassert>

namespace kismo
{
	namespace frx
	{		
		/*
			Example to create a singleton from this simply do:
				class CLASSNAME : public Singleton< CLASSNAME >
				{...

			NOTE derived class should have this in their cpp:
				template<> CLASSNAME* Singleton< CLASSNAME >::ms_Singleton = 0;
		*/

		template < class T > 
		class Singleton
		{
		public:
			Singleton()
			{
				assert( !ms_instance );  //Make sure the singleton is not created twice anywhere!          
				ms_instance = static_cast< T* >( this );
			}

			virtual ~Singleton( void )
			{  
				assert( ms_instance ); //Make sure the singletone was created.... wierd if not
				ms_instance = 0;  
			}

				
			static T& getSingleton( void ) 
			{ assert( isValidPointer( ms_instance ) ); return ( *ms_instance ); }
			
			static T* getSingletonPtr( void ) 
			{ assert( isValidPointer( ms_instance ) ); return ms_instance;  }


		protected:
			static T* ms_instance; //Static instance pointer
		};
	} // END: frx
} // END: kismo

#endif
