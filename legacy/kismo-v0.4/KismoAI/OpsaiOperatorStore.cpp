/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoAI\OpsaiOperatorStore.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
/** \author Craig Hutchinson \file 
	\brief 
*/
#include "OpsaiOperatorStore.h"

namespace opsai
{
	IMPLEMENT_RTTI_BASE( opsai, OperatorStore );

	OperatorStore::OperatorStore( OperatorStoreLoader* loader ) :
		m_loader( loader )
	{
	}

	OperatorStore::~OperatorStore()
	{
		clear();
	}

	void OperatorStore::clear()
	{
		for ( Operator::Array::const_iterator iOperator = m_operators.begin();
			iOperator != m_operators.end();
			++iOperator )
		{
			delete (*iOperator);
		}
		m_operators.clear();
		m_operatorNames.clear();
	}

	Bool OperatorStore::load()
	{
		if ( !m_loader )
		{
			logRecoverable( "No OperatorStore loader has been specified - operators cannot be (re)loaded" );
			return false;
		}
		
		//Clear the operators
		clear();

		if ( m_loader->load( *this ) )
		{
			logSuccess( "The OperatorStore loaded from '" << m_loader->getFilename() << "' successfully" );
			return true;
		}
		else
		{
			logRecoverable( "The OperatorStore failed to load from '" << m_loader->getFilename() << "'" );
			return false;
		}
	}

	Operator* OperatorStore::createOperator( const String& name, const Sentence& txt, const State::Array& pre, const State::Array& del, const State::Array& add )
	{
		OperatorMap::const_iterator iOperator = m_operatorNames.find( name );
		if ( iOperator != m_operatorNames.end() )
		{
			logError( "An operator of the name '" << name << "' already exists wihtin the operator store! "
					"The duplicate is ignored" );
			return 0;
		}

		Operator* newOp = new Operator( name, txt, pre, del, add );
		assert( newOp );

		//Store the operator in the operator vector
		m_operators.push_back( newOp );

		//Store a lookup for the operator by name cto the handle wihtin the operator array
		m_operatorNames[ name ] = (Handle)m_operators.size();

		return newOp;
	}	

} //END: opsai