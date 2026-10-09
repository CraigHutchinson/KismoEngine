/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\framework\KismoSmartPtr.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef _KISMO_SMARTPOIntER_H
#define	_KISMO_SMARTPOIntER_H

#include "framework/KismoTypes.h"

//@ game coding complete (first edition) by Mike McShaffry, p54

namespace kismo
{
	namespace frx
	{		
		//forward declaration
		template < class T > class SmartPtr;

		//-----------------------------------------------------
		// Reference counting interface
		// Classes can impliment this themselves or use the Smartptr
		// internal implimentation
		//-----------------------------------------------------
		template < class T > class IRefCount
		{
			friend class SmartPtr< T >;
		protected:
			virtual void addRef() = 0;
			virtual void release() = 0;
			virtual T* getPtr() const = 0;
			virtual k_UInt16 getCount() const = 0;
		};

		//-----------------------------------------------------
		// Reference counting Default implimentation
		// To use derive the class :
		// clas CMyClass : public IRefCountImpl<CMyClass> {...}
		//-----------------------------------------------------
		template <class T> class IRefCountImpl : public IRefCount< T >
		{
		protected:

			IRefCountImpl() : m_count( 0 ) {}

			virtual void addRef() 
			{ 
				assert(m_count != kc_UInt16_MAX);
				++m_count;
			}
			virtual void release()
			{
				assert( m_count != 0 );
				--m_count;
				if ( m_count == 0 )
					destroy();
			}
			virtual T* getPtr() const { return ((T*)this); }
			virtual void destroy() { if( isValidPointer( getPtr() ) ) delete getPtr(); }
			virtual k_UInt16 getCount() const { return m_count; }

		private:
			k_UInt16 m_count;
		};


		//-----------------------------------------------------
		// Smart pointer
		//-----------------------------------------------------
		template < class T > class SmartPtr
		{
		public:

			SmartPtr() : m_refCount( 0 ) {}
			SmartPtr( T* ptr ) : m_refCount( 0 ) { assign( ptr ); }
			SmartPtr( const SmartPtr& sp ) : m_refCount( 0 ) { assign( sp.m_refCount ); }
			virtual ~SmartPtr() { assign( (IRefCount<T>*)0 ); }

			inline T* getPtr() const { return ( isValidPointer( m_refCount ) ? m_refCount->getPtr() : 0 ); }

			//Assignment operators
			SmartPtr& operator = ( const SmartPtr& sp ) { assign( sp.m_refCount ); return *this; }
			SmartPtr& operator = ( T* ptr ) { assign( ptr ); return *this; }

			//T access and const conversion
			T* operator->() const { assert( isValidPointer( getPtr() ) ); return getPtr(); }
			operator T* () const { return getPtr(); }

			//utilities
			bool operator !() { return !isValidPointer( getPtr() ); }
			bool operator ==( const SmartPtr& sp ) { return getPtr() == sp.getPtr(); }
			bool operator !=( const SmartPtr& sp ) { return getPtr() != sp.getPtr(); }

			/** Get the number of references */
			k_UInt16 refCount() { return ( isValidPointer( m_refCount ) ? m_refCount->getCount() : 0 ); }			
		private:

			IRefCount< T >* m_refCount;

			//-----------------------------------------------------
			// Ref counter
			// For classes that don't impliment IRefCount
			//-----------------------------------------------------
			class  RefCounter : public IRefCountImpl< T >
			{
			public:
				RefCounter( T* ptr ) : m_ptr( ptr ) {}
				virtual ~RefCounter() { IRefCountImpl<T>::destroy(); }
			protected:
				virtual T* getPtr() const { return m_ptr; }
				virtual void destroy() { delete this; }
			private:
				T* m_ptr;
			};

			//This is called if T does not impliment IRefCount
			void assign( void* ptr )
			{
				if ( !isValidPointer( ptr ) )
					assign( (IRefCount<T>*)0 );
				else
					assign( new RefCounter( static_cast<T*>(ptr) ) );
			}

			//...if T impliments IRefCount
			void assign( IRefCount<T>* refCount )
			{
				if ( isValidPointer( refCount ) )
					refCount->addRef();
				IRefCount<T>* oldRef = m_refCount;
				m_refCount = refCount;
				if ( isValidPointer( oldRef ) )
					oldRef->release();
			}

		};


	} //END: frx

} //END: kismo

#endif