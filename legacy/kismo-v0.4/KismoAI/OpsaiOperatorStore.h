/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoAI\OpsaiOperatorStore.h
 * Status: unique (no other copy of this file name in the archive)
 */
/** \author Craig Hutchinson \file 
	\brief 
*/
#ifndef OPSAI_OPERATORSTORE_H
#define OPSAI_OPERATORSTORE_H

#include "OpsaiOperator.h"
#include "Rtti.h"

namespace opsai
{
	//forward declaration
	class OperatorStoreLoader;

	/** \brief Storeage class for GPS operators
	*/
	class OperatorStore
	{
		DECLARE_RTTI_BASE;
	public:

		typedef Operator::Array::const_iterator const_iterator; ////< Iterating the operators is always const

	public:

		/** \brief Constructor
			\param[in] loader The loader sued for reading the operators from file and possible 
			saving at a later time
		*/
		OperatorStore( OperatorStoreLoader* loader = 0 );

		/** \brief Destructor calls clear()
		*/
		~OperatorStore();

		/** \brief If the loader has been specified calling load() will erase the current grammar and then
			call the loader to load the config file	
			\remarks If no loader has been set then False will always be returned
		*/
		Bool load();

		/** \brief Adds a new operator to the operator store
			\param[in] name Name used for identifying the operastor for debugging
			\param[in] txt A state which is used to match values for debug output of the operators action
			\param[in] pre Array of precondition states including wildcards that must be matched
			\param[in] del If the rpeconditions match the del states will be removed form the world state
			\param[in] add If the rpeconditions match the add states will be added to the world
		*/
		Operator* createOperator( const String& name, const Sentence& txt, const State::Array& pre, const State::Array& del, const State::Array& add );

		/** \brief get an iterator for the first rule
		*/
		const_iterator begin() const
		{ return m_operators.begin(); }

		/** \brief get an iterator for the last rule
		*/
		const_iterator end() const
		{ return m_operators.end(); }

	protected:

		/** \brief Clears all the operators that are loaded releasing any memory used
			\remarks IF for some reason the operator is referenced elsewhere the pointer will
			be invalid
			\par This is called within load()
		*/		
		void clear();

	protected:
		Operator::Array m_operators; ////< Vector array of all the operators

		typedef Map< String, Handle > OperatorMap; //?< Map for name to handle lookups
		OperatorMap m_operatorNames; ////< Stores all operator names to their index wihtin the op vectorm_operatorNames

		OperatorStoreLoader* m_loader; ////< The loade rwhich parses the grammar file	
	};

	
	/** \brief Base interface for a operator store loading
	*/
	class OperatorStoreLoader
	{
	public:
		OperatorStoreLoader( Dictionary& dictionary, const String& filename ) : 
		  m_filename( filename ), m_dictionary( dictionary ) {}

		/** \brief Virtual load function must be implimented
		*/
		virtual Bool load( OperatorStore& operatorStore ) = 0;
	
		/** \brief get the filename the loade rwill load from
		*/
		const String& getFilename() const
		{ return m_filename ; }

		/** \brief get the dictionary used for word string hashing
		*/
		Dictionary& getDictionary()
		{ return m_dictionary ; }

	private:
		String m_filename; ///< The filename the loader will load from
		Dictionary& m_dictionary; ////< the dictionary instance for converting string words to word hash instances
	};

} //END: opsai

#endif