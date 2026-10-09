/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoAI\LexaiParseNode.h
 * Status: unique (no other copy of this file name in the archive)
 */
/** \author Craig Hutchinson \file 
	\brief 
*/
#ifndef LEXAI_PARSENODE_H
#define LEXAI_PARSENODE_H

#include "FrxaiWord.h"
#include "LexaiSlots.h"
#include "GraphNode.h"

namespace lexai
{
	class ParseNode : public frx::GraphNode< ParseNode >, public Slots
	{
	public:		
		typedef Vector< ParseNode* > Array;
	public:

		explicit ParseNode( const Word& value, const Word& ruleID = Word(0) ) : 
			m_value( value ),
			m_ruleID( ruleID )
		{}

		//Comparison operators				
		Bool operator == ( const Word& rhs ) const 
		{ return m_value == rhs; }

		Bool operator != ( const Word& rhs ) const 
		{ return m_value != rhs; }

		Bool operator == ( const ParseNode& rhs ) const 
		{ return m_value == rhs.m_value ; }

		Bool operator != ( const ParseNode& rhs ) const 
		{ return m_value != rhs.m_value ; }

		Bool operator < ( const ParseNode& rhs ) const 
		{ return m_value < rhs.m_value ; }

		Bool operator > ( const ParseNode& rhs ) const 
		{ return m_value > rhs.m_value ; }
		friend std::ostream& operator << ( std::ostream& os, const ParseNode& node )
		{
			os << '(' << (String)node.m_value;
			node.outputSlots( os );
			os << ')';
			return os;
		}

		/** \brief Finds a child whose value matches the word supplied
		*/
		ParseNode* findChild( const Word& value ) const
		{			
			const_iterator iChild = m_children.begin();
			for (; iChild != m_children.end() && (**iChild) != value; ++iChild ) {}
			if ( iChild != m_children.end() )
				return *iChild;
			else
				return 0;			
		}

		/** \brief Get the category/value of the parse node
		*/
		const Word& getValue() const
		{ return m_value; }

		/** \brief Set the ID for the node used in debugging
		*/
		void setID( const Word& id )
		{ m_ruleID = id; }

		/** \brief Get the ID used for debugging
		*/
		const Word& getID() const
		{ return m_ruleID; }

	private:

		Word m_value; ////< The category ID of the rule for this node, the word, or word category
		Word m_ruleID; ////< The rule ID whihc will identify the creating rule fromt the grammar file
	};
	

} //END: lexai

#endif
