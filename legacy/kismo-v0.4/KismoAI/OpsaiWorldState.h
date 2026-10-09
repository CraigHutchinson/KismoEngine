/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoAI\OpsaiWorldState.h
 * Status: unique (no other copy of this file name in the archive)
 */
/** \author Craig Hutchinson \file 
	\brief 
*/
#ifndef OPSAI_WORLDSTATE_H
#define OPSAI_WORLDSTATE_H

#include "OpsaiState.h"
#include "FastQueue.h"
#include "Rtti.h"

namespace opsai
{
	//forward declarations
	class WorldStateLoader;
	class Operator;

	/** \brief World state contains the dynamic states of the world and a pointer ot the static 
		states to remove duplication.
		\remarks The static world state is manaaged within a worldState when the loader has been specified
		\par For speed purposes deletion of a state occurs by moving the last state to the 
		new free state space instead of shifting the entire array of states after the erased state
		\par For insertion of a new dynamic state at the end of the WorldState there is an initial
		reserved size which is the dynamic state size times two which should be sufficient
		in most cases.
		\par For fast deletion and addition removeAdd() should be used as it will simply places
		new states in the space of the deleted states which is efficient especially if the number 
		of states to be deleted is equal to the number to add
	*/
	class WorldState
	{
		DECLARE_RTTI_BASE;
	public:

		/** \brief Custom iterator will manage passage from the static to dynamic 
			states wihtin the world as necessary
			\remarks This makes usage of the states appear as a single contiguous buffer.
			This is only however a const version as dynamic array can be iterated
			using the normal iterator which should not be used on the statis i.e. const static data
		*/
		class const_iterator : public State::Array::const_iterator
		{
		public:
			/** \brief Construcotr for an iterator which starts within the static world state
			*/
			const_iterator( const State::Array::const_iterator& currentStatic, 
							const State::Array::const_iterator& endStatic,
							const State::Array::const_iterator& beginDynamic ) :
				State::Array::const_iterator( currentStatic ),
				m_endStatic( endStatic ),
				m_beginDynamic( beginDynamic ),
				m_static( true )
			{}
			/** \brief Construcotr for an iterator which starts within the dyynamic world state
			*/
			const_iterator( const State::Array::const_iterator& currentDynamic ) :
				State::Array::const_iterator( currentDynamic ),
				m_static( false )
			{}

			/** \brief We redefine the pre-increment function to move from the statis array onto
				the dynamic array seamlessl
				*/
			void operator++()
			{	// preincrement the base implementation
				++(*(State::Array::const_iterator*)this);

				//If we are int eh static array then we need to check whether we must switch to the dynamic array
				if ( m_static && (*(State::Array::const_iterator*)this) == m_endStatic )
				{
					m_static = false;
					(*(State::Array::const_iterator*)this) = m_beginDynamic; ////< Goto the beginning of the dynamic buffer
				}
			}

			/** \brief Non equality operator.
			*/
			Bool operator!=( const const_iterator& rhs ) const
			{
				if ( m_static != rhs.m_static )
					return true;
				else
					return (*(State::Array::const_iterator*)this) != (*(State::Array::const_iterator*)&rhs);	
			}

		protected:
			Bool m_static; ////< Whether we are currently iterating the static array
			State::Array::const_iterator m_endStatic; ////< End() of the static array
			State::Array::const_iterator m_beginDynamic; ////< Begin() of the static array
		};


		typedef Int16 StateIndex; ////< For indexing states where a negative state relates to the index in the static state array
		
		/** \brief For storing the indexes to states within the world we don't use a 
			map to we can use the default stl Map equality operator and ignore the state Index
			within the comparison
		*/
		struct StateHashLookup
		{
			State::HashValue first; ////< The states Hash value
			Index second; ////< The index within the state array

			StateHashLookup( State::HashValue hash ) : first( hash )  {} ////< Construct with no index for speed
			StateHashLookup( State::HashValue hash, StateIndex index ) : first( hash ), second( index ) {}
			Bool operator == ( const StateHashLookup& rhs ) const { return first == rhs.first; }
			Bool operator != ( const StateHashLookup& rhs ) const { return first != rhs.first; }
			Bool operator < ( const StateHashLookup& rhs ) const { return first < rhs.first; }
			Bool operator > ( const StateHashLookup& rhs ) const { return first > rhs.first; }

		};
		typedef Set< StateHashLookup > StateMap; ////< Map for finding states wihtin the array quickly

		/** abrief STL compliant sorting class for searching wihtin a set of WorldState pointers
		*/
		template < class _T >
		struct DerefLess : public std::binary_function< _T, _T, bool>
		{	// functor for operator<
			bool operator()(const _T _Left, const _T _Right) const
			{	// apply operator< to operands
				return (*_Left < *_Right);
			}
		};

		typedef Set< WorldState*, DerefLess<WorldState*>  > PtrLookup; ////< Sort for fast checking if a worldState already exists	
		typedef Set< StateMap > Lookup; ////< Sert for fast checking if a worldState already exists
		typedef Vector< WorldState > Array; ////< For the queue we use an array for now
		typedef Queue< WorldState > Buffer; ////< For the queue we use an array for now
		typedef frx::FastQueue< const WorldState* > PtrBuffer; ////< For the queue we use an array for now

		typedef State::Array::iterator iterator; ////< Writeable iterator for dynamic states

	public:

		/** \brief constructor takes a loader which can be sued to load the state from file
		*/
		WorldState( WorldStateLoader* loader );

		/** \brief constructor takes the static and dynamic states of the world
			\remarks The internal buffer is sized to (dynamicStates.size() * 2)
		*/
		//WorldState( const State::Array* staticStates, const State::Array& dynamicStates );

		/** \brief Copy constructor
			\remarks Performs a normal copy but does not copy the m_loader pointer
		*/
		WorldState( const WorldState& rhs );

		/** \brief Destructor calls clear()
		*/
		~WorldState();

		/** \brief If the loader has been specified calling load() will erase the current grammar and then
			call the loader to load the config file	
			\remarks If no loader has been set then False will always be returned
		*/
		Bool load();

		/** \brief Sets the static states and the dynamic states
		*/
		Bool setStates( const State::Array* staticStates, const State::Array& dynamicStates );

		/** \brief Adds a single new state to the worldStates dynamic vector and stores a 
			lookup for fast finding
		*/
		Bool addState( const State& state );

		/** \brief Removes an array of states from the world state
			\remarks As erase() on an array will move a large chunk of memory we instead move states 
			\par If removing and adding simultaneously removeAdd() is more efficient
			at the end into the newly available space
			\par The states should be complete and not contain wildcards
			\param[in] states Array of sttaes which will be located and removed
		*/
		Bool remove( const State::Array& states ){return false;}

		/** \brief Adds an array of dynamic states to the world state
			\remarks If removing and adding simultaneously removeAdd() is more efficient
			\par TODO: Calling add may cause duplicate states, is this a problem to remove?!?
			\par The states should be complete and not contain wildcards
			\param[in] states Array of states which will be added
		*/
		Bool add( const State::Array& states ){return false;}

		/** \brief Adds an array of dynamic states to the world state
			\remarks If removing and adding simultaneously removeAdd() is more efficient
			\par The states should be complete and not contain wildcards
			\param[in] removeStates Array of sttaes which will be located and removed
			\param[in] addStates Array of states which will be located and removed
		*/
		Bool removeAdd( const State::Array& removeStates, const State::Array& addStates );

		/** \brief Assignment overload
		*/
		WorldState&  operator = ( const WorldState& rhs );

		/** \brief Lookup an array of states that do not contain wildcards
			\param[in] goals The states to check for
			\param[in] includeStatic Whether the static world state should be checked aswell
			\param[in] mustExist Specifies whether all of the goals must exist. If a goal is missing the function will quit and return 0
			\returns The number of goals that were matched. If mustExist is true then theis will be either 0 or goals.size()
		*/
		Count lookup( const State::Array& goals, Bool includeStatic = true, Bool mustExist = true ) const;

		//-------------------------------
		// Iterators
		//-------------------------------

		/** \brief Get static start State iterator
		*/
		const_iterator staticBegin() const
		{ return const_iterator( m_staticStates->begin(), m_staticStates->end(), m_dynamicStates.begin() ); }

		/** \brief Get static end State iterator
			\remarks This is one past the end of the array so is the same as dynamicStart()
		*/
		const_iterator staticEnd() const
		{ return const_iterator( m_staticStates->end(), m_staticStates->end(), m_dynamicStates.begin() ); }

		/** \brief Get dynamic start State iterator (Const)
		*/
		const_iterator dynamicStart() const
		{ return const_iterator( m_dynamicStates.begin() ); }
		
		/** \brief Get dynamic start State iterator
		*/
		iterator dynamicStart()
		{ return m_dynamicStates.begin(); }

		/** \brief Get dynamic end State iterator (Const)
		*/
		const_iterator dynamicEnd() const
		{ return const_iterator( m_dynamicStates.end() ); }
		
		/** \brief Get dynamic end State iterator
		*/
		iterator dynamicEnd()
		{ return m_dynamicStates.end(); }
	
		//-------------------------------
		// STL compliant iterators for entire world state
		//-------------------------------
		
		/** \brief Get static start State iterator
		*/
		const_iterator begin() const
		{ return staticBegin(); }

		/** \brief Get static start State iterator
		*/
		const_iterator end() const
		{ return dynamicEnd(); }
		
		//Equality operators
		Bool operator == ( const WorldState& rhs ) const { return m_dynamicStateLookup == rhs.m_dynamicStateLookup; }
		Bool operator != ( const WorldState& rhs ) const { return m_dynamicStateLookup != rhs.m_dynamicStateLookup; }
		Bool operator < ( const WorldState& rhs ) const { return m_dynamicStateLookup < rhs.m_dynamicStateLookup; }
		Bool operator > ( const WorldState& rhs ) const { return m_dynamicStateLookup > rhs.m_dynamicStateLookup; }

		/** \brief Get a rteference to the states state map
		*/
		const StateMap& getDynamicStateMap() const
		{ return m_dynamicStateLookup; }

		/** \brief Set the operator stored by theis state
			\remarks This is used to store where the state came from
		*/
		void setOperator( const Operator* op, const State::Match& match )
		{ m_operator = op; m_operatorMatch = match; }

		/** \brief Get the parent state if one exists
		*/
		const WorldState* getParentState() const
		{ return m_parentState; }

		/** \brief Get the matcher data which was used to create this state
		*/
		const State::Match& getMatchData() const
		{ return m_operatorMatch; }

		/** \brief Get the operator which was used to create this state
		*/
		const Operator* getOperator() const
		{ return m_operator; }

		/** \brief Causes the state to loose its dynamic state array but maintain the lookup table.
			This reduces memory consuumtion as required.
		*/
		void compact()
		{ m_dynamicStates.resize( (StateMap::size_type)0 ); }

		/** \brief Removes parentState reference and operator data
		*/
		void cleanup()
		{ m_operator = 0; m_parentState = 0; m_operatorMatch.clear(); }

	protected:

		/** \brief Clears all the world state and removes the reference to the staticStates
			\remarks If this WorldState has a loader the static state array will be deleted.
			This should therefore not be called when other states have been created from this as will cause an invalid pointer
		*/
		void clear();

	protected:
		const State::Array* m_staticStates; ////< Pointer to the static state array
		State::Array m_dynamicStates; ////< The dynamic state array

		StateMap* m_staticStateLookup; ////< State lookup for finding the index of a static state quickly
		StateMap m_dynamicStateLookup; ////< State lookup for finding the index of a static state quickly

		WorldStateLoader* m_loader; ////< The loade rwhich parses the worldState file is supplied

		const WorldState* m_parentState; ////< The parent state that this state was created from
		const Operator* m_operator; ////< The operator that was sued to create this state
		State::Match m_operatorMatch; ////< The operator matcher data for wildcards
	};


	/** \brief Base interface for a worldState loader
	*/
	class WorldStateLoader
	{
	public:
		WorldStateLoader( Dictionary& dictionary, const String& filename ) : 
			m_filename( filename ),
			m_dictionary( dictionary )
		{}

		/** \brief Virtual load function must be implimented
		*/
		virtual Bool load( WorldState& worldState ) = 0;
	
		/** \brief Virtual load function must be implimented for loading an array of states from the file
			\remarks Loads from a file containing only states wihtin the root node and if a name is supplied
			will load states within a sub-node of the root with that name. Without supplying the nodeName it 
			is expected the states are child to the root node
		*/
		virtual Bool load( State::Array& states, const String& nodeName = String() ) = 0;

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
		Dictionary& m_dictionary; ////< A reference to the dictionary to convert word string to hashed word values
	};


} //END: opsai


#endif


