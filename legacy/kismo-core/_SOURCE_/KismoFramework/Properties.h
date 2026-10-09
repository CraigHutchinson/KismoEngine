/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\KismoCore\_SOURCE_\KismoFramework\Properties.h
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

#ifndef KISMOFRAMEWORK_PROPERTIES_H
#define KISMOFRAMEWORK_PROPERTIES_H

#include "KismoFramework\Common.h"
#include "KismoFramework/Variant.h"
#include "KismoFramework/Rtti.h"
#include <map>
	
namespace kismo
{		
	/** The proprty definition of an objects 'property'
	*/
	class PropertyDefinition
	{
	public:
		PropertyDefinition( const Char* name, Count nameLength, const Char* description, Count descriptionLength, const Variant& defaultValue ) :
			m_name( name, nameLength ),
			m_description( description, descriptionLength ),
			m_defaultValue( defaultValue )
		{}

		const String& name() const
		{ return m_name; }

		/** PropertyDefinition description
			\remarks Should describe what the property affects and what values may be expected
		*/
		const String& description() const
		{ return m_description; }

		const Variant& defaultValue() const
		{ return m_defaultValue; }

	private:
		const String m_name;
		const String m_description;
		const Variant m_defaultValue;

		struct
		{
		  bool m_isHidden	: 1; ///< Whether the property is hidden from user interface
		  bool m_isSave		: 1; ///< Whether the property is saved/wread to file
		  bool m_isReadOnly	: 1; ///< Whether the property cannot be modified
		};
	};
	
	/** List of properties
		\remarks Does not pefrom property-value get/set functionality. To do this you should obtain the PropertyDefinition of interest vie
		the PorpertyList::getPropertyDefinition() methods
	*/
	class PropertyDefinitionList
	{
	public:
		PropertyDefinitionList();
		~PropertyDefinitionList();

		/**	Reserves a number of properties which are indexed into for fast lookup 
			\note Once allocated the number of properties cannot be modified 
		*/
		bool setPropertyCount( Count count );

		inline Count getPropertyCount() const
		{ return m_propertyCount; }

		/** Adds a custom property type to the list
			\returns	Pointer to the property stored at the specified index 
			\remarks	This function does not error if a property is already defined at the existing index but will Assert in debug if error cases occur for release performance.
						Call getPropertyDefinition(index) and getPropertyIndex(prop->name()) == Index_Max to handle the error cases cleanly if needed.
		*/
		PropertyDefinition* setPropertyDefinition( Index index, PropertyDefinition* prop );


		/** Adds a property to the list
			\returns The new property if the index is valid
			\warning The number of properties must be defined by setPropertyCount() prior to calling setPropertyDefinition().
			\remarks Calls setPropertyDefinition(PropertyDefinition*) internally
			\TODO: Add an Allocator class instead of calling 'new' which may cause fragmentation
		*/
		inline PropertyDefinition* setPropertyDefinition( Index index, const Char* name, Count nameLength, const Char* description, Count descriptionLength, const Variant& defaultValue )
		{
			//TODO: use a memory allocator of some kind to maintain a pool of properties and reduce fragmentation
			return setPropertyDefinition( index, new PropertyDefinition( name, nameLength, description, descriptionLength, defaultValue ) );
		}

		/** \copydoc setPropertyDefinition(Index,constChar*,Count,constChar*,Count,const Variant&)
		*/
		inline PropertyDefinition* setPropertyDefinition( Index index, const Char* name, const Char* description, const Variant& defaultValue )
		{ return setPropertyDefinition( index, name, 0, description, 0, defaultValue ); }

		/** \copydoc setPropertyDefinition(Index,constChar*,Count,constChar*,Count,const Variant&)
		*/
		inline PropertyDefinition* setPropertyDefinition( Index index, const String& name, const String& description, const Variant& defaultValue )
		{ return setPropertyDefinition( index, name.c_str(), (Count)name.length(), description.c_str(), (Count)description.length(), defaultValue ); }

		/** Get a property by index
			\remark The index is used for fast lookup/iteration over the properties. get the index of a named property by calling getPropertyIndex()
		*/
		PropertyDefinition* getPropertyDefinition( Index index ) const
		{
			Assert(index < m_propertyCount);
			return m_properties[index]; 
		}

		/** Get a property by name. 
			\remarks If property is looked up many times storing the property or 
			using getPropertyIndex() to cache the position is recommended
			\returns The proeprty with the given name or 0 if the name was not found
			\param[in] name Case sensitive search name
		*/
		inline PropertyDefinition* getPropertyDefinition( const Char* name, Count nameLength = 0 ) const
		{
			Index index = getPropertyIndex( name, nameLength );
			return ( index!=Index_Max ? getPropertyDefinition(index) : 0);
		}
		
		inline PropertyDefinition* getPropertyDefinition( const String& name ) const
		{ return getPropertyDefinition( name.c_str(), (Count)name.length() ); }
		
		/** Finds the index of a property from its name
			\remarks Names are case-sensitive
			\TODO: case-sensitivity incurs some flexibility costs but saves a very small amount of processing time that may add up
		*/
		Index getPropertyIndex( const Char* name, Count nameLength = 0 ) const;

		inline Index getPropertyIndex( const String& name ) const
		{ return getPropertyIndex(name.c_str(), (Count)name.length() ); }

	private:
		Count m_propertyCount; //, Stores the number of elements in m_properties
		PropertyDefinition** m_properties; ///< Array of properties \note Size of the proeprties array must be defined by a call to setPropertyCount()
		
		struct PropertyDefinition_Name_SortLess
		{
			bool operator() ( const PropertyDefinition* _Left, const PropertyDefinition* _Right) const { return _tcscmp( _Left->name().c_str(), _Right->name().c_str() ) < 0; } 
		};

		typedef std::map<PropertyDefinition*, Index, PropertyDefinition_Name_SortLess> PropertyDefinitionNameLookup;
#pragma warning(disable:4251)
		PropertyDefinitionNameLookup m_propertyNameLookup; //, Allows for lookup of properties by name
#pragma warning(default:4251)
	};

	class PropertyInterface : public PropertyDefinitionList
	{
	public:
		//DECLARE_ROOT_RTTI;
		//TODO: we could just use 'PropertyInterface' instead, this might not be safe all together though. using a virtual function to redirect is the only other viable approach though!?
		class Dummy{}; //< Dummy class to allow member function pointer use without type issues

		/** Function deifnition of the 'defineProperties()' method for a class which desires to have Property data
		*/
		typedef bool (*Fn_defineProperties)( PropertyDefinitionList& );
		
		/** Function definition which must be impelmented for setting properties
			\note setProperty()
		*/
		typedef bool (Dummy::*Fn_setProperty)( Index prop, Variant& value );
		
		/** Function definition which must be impelmented for getting properties
			\note getProperty()
		*/
		typedef bool (Dummy::*Fn_getProperty)( Index prop, Variant& value ) const;

	public:
		/*template < class _Type >
		PropertyInterface( Fn_defineProperties fnDefineProperties ) :
			m_fnSetProperty( (Fn_setProperty)_Type::setProperty ),
			m_fnGetProperty( (Fn_getProperty)_Type::getProperty )
		{ if(fnDefineProperties) (*fnDefineProperties)( m_properties ); }*/

		/** Type Meta constructor taking 3 function pointers
			\param[in] fnDefineProperties Static/C-style function pointer called to populate the types property list
			\param[in] fnSetProperty Class-member-function called to SET a property values for a class instance
			\param[in] fnGetProperty Class-member-function called to GET a property values for a class instance
			\remarks Cast member function pointers to PropertyInterface::Fn_setProperty, PropertyInterface::Fn_getProperty, etc.
		*/
		PropertyInterface( Fn_defineProperties fnDefineProperties, Fn_setProperty fnSetProperty = 0, Fn_getProperty fnGetProperty = 0 );
	
		/** Virtual destructor to allow for specialised type Meta implementations
		*/
		virtual ~PropertyInterface(void);

		inline bool setPropertyValue( void* objectInstance, const String& propertyName, Variant& value ) const
		{ return setPropertyValue( objectInstance, propertyName.c_str(), (Count)propertyName.length(), value ); }

		inline bool setPropertyValue( void* objectInstance, const Char* propertyName, Variant& value ) const
		{ return setPropertyValue( objectInstance ,propertyName, 0, value); }

		inline bool setPropertyValue( void* objectInstance, const Char* propertyName, Count propertyNameLength, Variant& value ) const
		{ return setPropertyValue( objectInstance, getPropertyIndex(propertyName,propertyNameLength), value ); }
		
		inline bool setPropertyValue( void* objectInstance, Index propertyIndex, Variant& value ) const
		{ 
			Assert( propertyIndex != Index_Max );
			Assert( m_fnSetProperty );
			return (((Dummy*)objectInstance)->*m_fnSetProperty)( propertyIndex, value );
		}
		
		inline bool getPropertyValue( void* objectInstance, const String& propertyName, Variant& value ) const
		{ return getPropertyValue(objectInstance,propertyName.c_str(),(Count)propertyName.length(),value); }

		inline bool getPropertyValue( void* objectInstance, const Char* propertyName, Variant& value ) const
		{ return getPropertyValue(objectInstance,propertyName,0,value); }

		inline bool getPropertyValue( void* objectInstance, const Char* propertyName, Count propertyNameLength, Variant& value ) const
		{ return getPropertyValue(objectInstance, getPropertyIndex(propertyName,propertyNameLength), value ); }
		
		inline bool getPropertyValue( void* objectInstance, Index propertyIndex, Variant& value ) const
		{ 
			Assert( propertyIndex != Index_Max );
			Assert( m_fnGetProperty );
			return (((Dummy*)objectInstance)->*m_fnGetProperty)( propertyIndex, value );
		}

	private:
		Fn_setProperty m_fnSetProperty;
		Fn_getProperty m_fnGetProperty;
	};

	/** property adapter for an object
	*/
	class Property
	{
	public:
		Property( void* instance, const PropertyInterface* propertyInterface, Index propertyIndex ) :
			m_instance(instance), 
			m_propertyInterface( propertyInterface ), 
			m_propertyIndex( propertyIndex )
		{ Assert(instance); Assert(m_propertyInterface); Assert( propertyIndex != Index_Max ); }

		bool setValue( Float32 f )
		{  
			Variant t(f);
			m_propertyInterface->setPropertyValue( m_instance, m_propertyIndex, t );
		} 

		bool setValue( UInt32 i )
		{  
			Variant t(i);
			m_propertyInterface->setPropertyValue( m_instance, m_propertyIndex, t ); 
		}

		bool operator !() const
		{ return m_propertyInterface == 0 || m_propertyIndex == Index_Max; }


		PropertyDefinition* getDefinition() const
		{ Assert(m_propertyIndex!=Index_Max); return m_propertyInterface->getPropertyDefinition(m_propertyIndex); }

		const String& name() const
		{ return getDefinition()->name(); }

		const String& description() const
		{ return getDefinition()->description(); }
		
		const Variant& defaultValue() const
		{ return getDefinition()->defaultValue(); }

		//TODO: We need this to circumvent an issue with Iterator!?!?!
		const Property* operator -> () const
		{ return this; }

		//TODO: We need this to circumvent an issue with Iterator!?!?!
		Property* operator -> ()
		{ return this; }

	public:
		const PropertyInterface* m_propertyInterface;
		const Index m_propertyIndex; //, The property whcih is the iterators position
		void* m_instance; //, The object instance the property belongs to
	};

	class Properties
	{
	public:

		class Iterator
		{
		public:
			Iterator( void* instance, const Rtti* type, Index propertyIndex ) :
				m_instance(instance), 
				m_type( type ), 
				m_index( propertyIndex )
			{}

			Iterator& operator ++ ()
			{ 
				Assert( m_type != 0); 
				Assert( m_index != Index_Max ); 

				const PropertyInterface* properties = m_type->propertyInterface();
				if ( ++m_index < properties->getPropertyCount() )
				{
					//We have no more properties for this class so get the first property of one of the base classes
					// - We deal with the case that a parent class does not need to define Meta data nor does it have to have properties if it does
					 while ( m_type && (m_type->propertyInterface() == 0 || m_type->propertyInterface()->getPropertyCount() == 0) );
						 m_type = m_type->getBaseType();
				}
				
				m_index = 0;
				return *this; 
			}

			bool operator !() const
			{ return m_type == 0; }

			const Property operator -> () const
			{ Assert(m_type); return Property( m_instance, m_type->propertyInterface(), m_index); }

			const Property operator * () const
			{ Assert(m_type); return Property( m_instance, m_type->propertyInterface(), m_index); }

			Property operator -> ()
			{ Assert(m_type); return Property( m_instance, m_type->propertyInterface(), m_index); }
			Property operator * ()
			{ Assert(m_type); return Property( m_instance, m_type->propertyInterface(), m_index); }

			//bool operator > ( const Iterator& rhs ) const
			//{/* Assert( dbg_owner == rhs.dbg_owner ); */return m_property > rhs.m_property; }
			//bool operator < ( const Iterator& rhs ) const
			//{/* Assert( dbg_owner == rhs.dbg_owner ); */return  m_propertyList < rhs.m_propertyList && m_index == rhs.m_index; }
			bool operator == ( const Iterator& rhs ) const
			{/* Assert( dbg_owner == rhs.dbg_owner );*/ return m_type == rhs.m_type && m_index == rhs.m_index; }
			bool operator != ( const Iterator& rhs ) const
			{/* Assert( dbg_owner == rhs.dbg_owner );*/ return m_type != rhs.m_type || m_index != rhs.m_index; }

		public:
			const Rtti* m_type;
			Index m_index; //, index of the property we are on
			void* m_instance; //, The object instance the property belongs to
		};

	public:
		template< class _Type >
		explicit Properties( _Type* instance ) :
			m_instance( (void*)instance), 
			m_type( instance->getType() )
		{}

		Properties( void* instance, const Rtti& type ) :
			m_instance(instance), 
			m_type( type )
		{}

		Iterator begin()
		{ return Iterator(m_instance, &m_type, 0u ); }

		Iterator end()
		{ return Iterator(m_instance, 0, 0 ); }

		bool isNull() const
		{ return m_instance == 0; }

		bool isValid() const
		{ return m_instance != 0; }

		void* instance() const
		{ return m_instance; }

		const Rtti& type() const
		{ return m_type; }

	private:
		void* m_instance;
		const Rtti& m_type;
	};


} //END: kismo

#endif