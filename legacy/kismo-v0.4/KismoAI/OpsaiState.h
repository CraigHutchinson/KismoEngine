/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoAI\OpsaiState.h
 * Status: unique (no other copy of this file name in the archive)
 */
/** \author Craig Hutchinson \file 
	\brief 
*/
#ifndef OPSAI_STATE_H
#define OPSAI_STATE_H

#include "OpsaiCommon.h"
#include "SmallMap.h"

namespace opsai
{
	/** \brief A state is a tuple of words which contains information about one part of the world state
		\remarks In this instance the tuple is represented by a vector in the Sentence class
	*/
	class State : public Sentence
	{
	public:

		static const UInt16 MaxWords = 4; ////< The maximum number of words a state tuple can hold
		static const UInt16 TrueMask = 0x0; ////< True mask - Used by matcher to ignore words that are wildcards when comparing states
		static const UInt16 FalseMask = 0xffff; ////< False mask for a word used by the matcher for a bit compare
		static const UInt16 WordShift = sizeof(Word::WID) * 8; ////< The size of each word for bitshifting for the mask generation

		typedef frx::SmallMap< Word, Word > Match; ////< Map for storing a lookup from a wildcard to the matched values
		typedef Vector< Match > MatchArray; ////< Vector of matches found from match() contain all combinations of the match wildcards
		//typedef Set< State > Lookup; ////< Lookup set for storing a world state
		typedef Vector< State > Array; ////< Array of states

		typedef BitSet< MaxWords > BitFlag; ////< Bitset for flagging a value for each word - used to store whether a word in the State is a wildcard

		typedef UInt64 HashValue; ///< Typedef the hash value type

		/** \brief For fast state comparison we store a lookup for the word ID's
			\remarks This creates a continguous memory chunk for fast comparisons using an integer compare
			\par This is only valid as long as tuples are 4 words or less. When the number of words is
			exceeded a word by word comparison would be required with this process. This however should not
			occur within the scope of the problem and can be avoided
		*/
		union Hash
		{

			Word::WID wids[ MaxWords ]; ////< The stgate ID consists of the words that make up this state
			HashValue value; ////< Very simple hash which is based on the 4 word ID's packed into a large integer

			//Equality operators
			Bool operator == ( const Hash& rhs ) const { return value == rhs.value; }
			Bool operator != ( const Hash& rhs ) const { return value != rhs.value; }
			Bool operator < ( const Hash& rhs ) const { return value < rhs.value; }
			Bool operator > ( const Hash& rhs ) const { return value > rhs.value; }
		};

	public:

		/** \brief default constructor
		*/
		State();

		/** \brief Constructor supplies a dictionary so can be set by a string at a later time
		*/
		State( Dictionary& dictionary );

		/** \brief Constructor from a sentence
		*/
		State( Dictionary& dictionary, const String& sentence, Char delim = ' ' );

		/** \brief Constructor from an array of words e.g. 'in agent room'
		*/
		State( const Sentence& words  );

		/** \brief Sets the state to that contained in a vector of words
		*/
		Bool set( const Sentence& words );

		/** \brief Sets a word within the state
			\remarks This set method DOES NOT check if the new word is a wildcard
		*/
		void setWord( Index wordIndex, const Word& word )
		{
			at( wordIndex ) = word;
			m_hash.wids[ wordIndex ] = (Word::WID)word;
			m_wildCards[ wordIndex ] = false;
		}

		/** \brief Overload the assignment by a word array to prevent accidental alteration without 
			proper intention
			\return Whether the assignment succeeded
		*/
		Bool operator = ( const Sentence& words )
		{ return set( words ); }

		//Equality operators
		Bool operator == ( const State& rhs ) const { return m_hash == rhs.m_hash; }
		Bool operator != ( const State& rhs ) const { return m_hash != rhs.m_hash; }
		Bool operator < ( const State& rhs ) const { return m_hash < rhs.m_hash; }
		Bool operator > ( const State& rhs ) const { return m_hash > rhs.m_hash; }

		operator Bool () const
		{ return size() != 0; }

		/** \brief Matches this state to another
			\param[in] db The database state to match with
			\param[out] matches The lookup table for the matches that were found if any
			\remarks If the paramater count is equal the and there are wildCards the match( mask,...) 
			will be called. For matching a large number of states use getWildCardMask() and call the laternate method.
		*/
		Bool match( const State& db, Match& matches ) const;

		/** \brief Matches this state suing a wildCard mask which is used to ignore certain words which should correlate to 
			the wildcards wihtin this state obtained via getWildCardMask().
			\param[in] mask Bitmask whihc filters out Wildcard values for comparisons
			\param[in] db The state to compare against
			\param[out] matches Output map whihc contains a lookup of wildcard names to a vector of possible values
			\remarks The matched wildcards will be returned in the matches parameter
		*/
		Bool match( const HashValue& mask, const State& db, Match& matches ) const
		{
			//Ceck that the words which aren't wildcards match (These are markes by 0's int eh mask)
			if ( (db.m_hash.value & mask) != (m_hash.value & mask) )
				return false;

			//Match the wildcard wiords with the 'matches'
			return matchWildcards( db, matches );
		}


		/** \brief Gets the states hash vlaue
		*/
		const HashValue& getHashValue() const
		{ return m_hash.value; }		

		/** \brief Creates a bit mask which can be applied to a states hash value to omit wildcard values
			\remarks This is used by the matchers for bit fast matching
			\par For now the mask is not stored to save memory as generating the mask does nto require much processing
		*/
		HashValue getWildCardMask() const
		{ 
			//Use a hash union to create the mask without the need of bitshifts :P
			Hash temp = { 
				( m_wildCards[0]? TrueMask : FalseMask ),
				( m_wildCards[1]? TrueMask : FalseMask ),
				( m_wildCards[2]? TrueMask : FalseMask ),
				( m_wildCards[3]? TrueMask : FalseMask ) };
				return temp.value; }

		/** \brief Gets whether the state has wildcards
		*/
		Bool hasWildcards() const
		{ return m_wildCards != 0; }

		/** \brief Returns whether a specified word is a wildcard
		*/
		Bool isWildcard( Index wordIndex ) const
		{ return m_wildCards[ wordIndex ]; }

	protected:

		/** \brief Helper function process the wordsand sets up m_wildCards to indicate whether
			the state contains wildcards or not
		*/
		Bool processWildCards();

		/** \brief Performs a word by word match with wildcards
		*/
		Bool matchWildcards(  const State& db, Match& matches ) const;

	protected:
		Hash m_hash; ////< The state value whihc is a packed struct of the words
		BitFlag m_wildCards; ////< Bitset defining which words are wildcards to save searching each time
	};

} //END: opsai

#endif