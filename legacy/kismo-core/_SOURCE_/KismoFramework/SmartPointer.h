/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\KismoCore\_SOURCE_\KismoFramework\SmartPointer.h
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

#ifndef SMARTPOINTER_H
#define	SMARTPOINTER_H

#include "KismoFramework/Types.h"
#include "KismoFramework\Common.h"

namespace kismo
{		
	/** \brief Reference counting implimentation
		\remarks To use derive the class :
		 clas CMyClass : public SmartReferenced {...}
	*/
	class SmartReferenced
	{
	public:

		/** \brief Add a manual reference to this object
		*/
		void addReference() 
		{ 
			Assert(m_referenceCount != UInt16_Max);
			++m_referenceCount;
		}

		/** \brief  manual remove a reference to this object
		*/
		void removeReference()
		{
			Assert( m_referenceCount != 0 );
			--m_referenceCount;
			if ( m_referenceCount == 0 )
				free();
		}

		/** \brief Get the number of references that this object has
		*/
		UInt16 getReferenceCount() const 
		{ return m_referenceCount; }

	protected:

		/** \brief Protected constructor enforces derived class
		*/
		SmartReferenced() : 
			 m_referenceCount( 0 ) 
		{}

		virtual ~SmartReferenced()
		{
			Assert(m_referenceCount == 0);
		}

		/** \brief Virtual free function allows derived class to perform pre-destuct actions by verloading this
		*/
		virtual void free()
		{
			delete this;
		}

	private:
		UInt16 m_referenceCount;
	};


	/** \brief Smart pointer class takes a  SmartReferenced object and wraps referencing
		\remarks The template paramater is the derived type that the SmartPointer will form
	*/
	template < class T > 
	class SmartPointer
	{
	public:

		/** \brief Default constructor for initialising a smart pointer to null
		*/
		SmartPointer( int i = 0 ) : 
			 m_referencee( 0 ) 
		{}

		 /** \brief Smart pointer takes a pointer to the type
		 */
		explicit SmartPointer( T* ptr ) : 
			m_referencee( (SmartReferenced*)ptr ) 
		{ if (m_referencee ) m_referencee->addReference(); }

		/** \brief COnstructor takes another smart pointer and adds a reference accodringly
		*/
		SmartPointer( const SmartPointer& smartPointer ) : 
			m_referencee( smartPointer.m_referencee )
		{ if ( m_referencee ) m_referencee->addReference(); }

		/** \brief Destructor removes a reference to the object
			\remarks This is nor virtual for speed
		*/
		~SmartPointer() 
		{
			if (m_referencee ) 
			{
				Assert( referenceCount() );
				m_referencee->removeReference();
			}
		}

		/** \brief Get a pointer to the referenced object
		*/
		inline T* getPtr() const 
		{ return (T*)m_referencee; }

		/** \brief Returns whether the smart pointer is valid
		*/
		inline bool isNull() const 
		{ return (m_referencee == 0); }

		/** \brief Returns whether the smart pointer is valid
		*/
		inline bool isValid() const 
		{ return !(m_referencee == 0); }

		/** \brief Assign the referencee of another smart pointer to this
		*/
		SmartPointer& operator = ( const SmartPointer& rhs ) 
		{ 
			assign( rhs.m_referencee ); 
			return *this; 
		}

		/** \brief Assign a pointer to the referenceable type
		*/
		SmartPointer& operator = ( T* rhs ) 
		{ 
			assign( rhs ); 
			return *this; 
		}

		/** \brief Pointer access operator
			\remarks Will asssert if the object is invalid
		*/
		T* operator->() const 
		{ 
			Assert( m_referencee ); 
			return (T*)m_referencee; 
		}

		/** \brief Inbuilt Type cast operator
			\remarks Will asssert if the object is invalid
		*/
		operator T* () const 
		{ 
			Assert( m_referencee ); 
			return (T*)m_referencee;
		}

		/** \brief Type cast operator
		*/
		template< class TCast >
		operator TCast* () const 
		{ 
			return (TCast*)m_referencee;
		}

		/** \brief Boolean not operator
		*/
		bool operator !() const 
		{ return isNull(); }

		/** \brief Boolean cast for 'if' usage
		*/
		operator bool() const 
		{ return isValid(); }

		/** \brief Equality operator with another smart pointer
		*/
		bool operator ==( const SmartPointer& rhs ) const 
		{ return getPtr() == rhs.getPtr(); }

		/** \brief Equality operator with another smart pointer
		*/
		bool operator !=( const SmartPointer& rhs ) const 
		{ return getPtr() != rhs.getPtr(); }

		/** \brief Equality operator with another smart pointer
		*/
		bool operator >( const SmartPointer& rhs ) const 
		{ return getPtr() > rhs.getPtr(); }

		/** \brief Equality operator with another smart pointer
		*/
		bool operator <( const SmartPointer& rhs ) const 
		{ return getPtr() < rhs.getPtr(); }

		/** \brief Get the number of references 
		*/
		UInt16 referenceCount() const
		{ return ( isValidPointer( m_referencee ) ? m_referencee->getReferenceCount() : 0 ); }			
	
	protected:

		SmartReferenced* m_referencee;

		/** \brief Assigns a new object to the smart pointer
		*/
		void assign( SmartReferenced* referencee )
		{
			//Add a reference to the new object
			if ( isValidPointer( referencee ) )
				referencee->addReference();

			//Store the old object
			SmartReferenced* oldReferencee = m_referencee;

			//Save the new one
			m_referencee = referencee;

			//Dereference the old object
			if ( isValidPointer( oldReferencee ) )
				oldReferencee->removeReference();
		}

	};


} //END: kismo

#endif
