/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\KismoCore\KismoFramework\Factory.h
 * Status: duplicate of kismo-core/_SOURCE_/KismoFramework/Factory.h
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

#ifndef KISMOFRAMEWORK_FACTORY_H
#define	KISMOFRAMEWORK_FACTORY_H

#include "KismoFramework/Types.h"
#include "KismoFramework/Rtti.h"
#include <functional>
#include <set>

namespace kismo
{	
	//foward declarations
	class Factory;

	/** \brief Defines the base implementation of a factory create functor
		\TODO: The constructor could be supplied an allocator
	*/
	class IFactoryProduct
	{
	public:
		IFactoryProduct( const kismo::Rtti* type );

		/** Destructor automatically removes this product from the attached factory
			\remarks This functionality currently limits the product instance to only exist within a single factory at any one time
			\TODO: we could remove the factory reference if this becomes an issue
		*/
		virtual ~IFactoryProduct();

		const Rtti* type() const
		{ return m_type; }

		Factory* factory() const
		{ return m_factory; }

		/** The derived implementation should create a specific create function */
	private:
		const Rtti* m_type; ///< The type the functor will create
		Factory* m_factory;
	};

	/** \brief Base factory definition
		\remarks The factory isn't templated to allow for factories that can create many different object types as required. 
		TODO: Could implement a template design that will enforce type safety but in the long run may reduce flexibility?
		\remarks See KismoTest_Factory for usage/examples
	*/	
	class Factory
	{
		friend class IFactoryProduct; ///< Function factories need to register automatically via the base interface
	protected:
		
		struct RTTIDeref_Name_SortLess : public std::binary_function< Rtti*, Rtti*, bool>
		{ bool operator() (const Rtti* _Left, const Rtti* _Right) const { return _tcscmp( _Left->name(), _Right->name() ) < 0; } };

		struct IFactoryProductDeref_Type_SortLess : public std::binary_function< IFactoryProduct*, IFactoryProduct*, bool>
		{ bool operator() (const IFactoryProduct* _Left, const IFactoryProduct* _Right) const { return _Left->type() < _Right->type(); } };

		//note: We don't store the IFactoryProduct type in the map to reduce memory fragmentation at the cost of some more interesting sort/search function
		typedef std::set< IFactoryProduct*, IFactoryProductDeref_Type_SortLess > ProductMap;

		//note: We don't store the Rtti name as a String in the map to reduce memory fragmentation at the cost of some more interesting sort/search function
		typedef std::set< const Rtti*, RTTIDeref_Name_SortLess > TypeLookup; ///< Type lookup map for use in matching entity names loaded from file

	public:

		Factory();
		virtual ~Factory();
	

		/** \brief Takes a type name and returns the type defintion
			\remarks This is usefull when loading from file with an entity type defined by string
			\par the types are automatically populated with existing entity types at compile time
			\param[in] name The Rtti typename of the entity to create
		*/
		const Rtti* getType( const Char* name, Count nameLength = 0 ) const;

		inline const Rtti* getType( const String& name ) const
		{ return getType( name.c_str(), (Count)name.length() ); }
	
		/** Returns whether the factory can construct an instance of the specified type
		*/			
		bool hasType( const kismo::Rtti* type ) const;

		inline bool hasType( const Char* name, Count nameLength = 0 ) const
		{ return hasType( getType(name,nameLength) ); }

		inline bool hasType( const String& name ) const
		{ return hasType( getType(name) ); }
		
		/** Obtain the product for a type
			\returns The base factory interface which should be support the interface of the derived factory
		*/
		IFactoryProduct* getProduct( const Rtti* type ) const;

		inline IFactoryProduct* getProduct( const String& name ) const
		{ return getProduct( getType(name) ); }

		inline IFactoryProduct* getProduct( const Char* name, Count nameLength = 0 ) const
		{ return getProduct( getType(name,nameLength) ); }

		/** Registration of factory functors
			\returns Whether the functor added successfully
			\note Should be implemented with type safe public function in derived impelmentation
			\remarks See KismoTest_Factory for usage/examples
			\param[in] type The type of the object defined by its runtime type
			\param[in] instance Pointer to the factory functor instance
		*/
		virtual bool addProduct( IFactoryProduct* instance );

		/** Unregisters a functor
			prevent possible errors
			\note Should be implemented with type safe public function in derived impelmentation
			\remarks See KismoTest_Factory for usage/examples
			\returns Whether the functor was found and removed successfully
		*/
		virtual bool removeProduct( IFactoryProduct* instance );

	private:
		//TODO: we copuld keep a direct lookup from type name to constructor for performance?
#pragma warning(disable:4251)
		TypeLookup m_types; ///< Lookup of the type names that exist in the factory (These match the class names)
		ProductMap m_constructors; ///< Map of factory methods for entity types
#pragma warning(default:4251)
	};

	inline IFactoryProduct::IFactoryProduct( const kismo::Rtti* type ) :  m_type(type), m_factory(0) { }
	inline IFactoryProduct::~IFactoryProduct()
	{ 
		if ( m_factory )  //, ensure factory is no longer referencing us
			m_factory->removeProduct(this);
	}

} //END: kismo

#endif
