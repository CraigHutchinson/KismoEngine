/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\Phys\Constraint.cpp
 * Status: current (newest copy of this file name in the archive)
 */
#include "Constraint.h"
#include "phys/Actor.h"
#include "Framework/Log.h"

namespace phys
{
	IMPLEMENT_RTTI( phys, Constraint, gfx::Spatial );

	Constraint::Constraint( const String& name, const Scene::Ptr& owner ) :
		gfx::Spatial( name ),
		m_owner( owner ),
		m_constraintType( Constraint_Invalid ),
		m_firstUpdate( true )
	{
		m_actor[0] = m_actor[1] = 0;
	}

	Constraint::~Constraint()
	{
	}

	void Constraint::setActorLookup( const String& name, Index index )
	{
		assert( index < 2 );
		m_actorLookup[index] = name;
	}
	
	bool Constraint::lookupActors()
	{
		for ( Index i = 0; i < 2; ++i )
		{
			if ( m_actorLookup[i].isNull() )
				continue; //No name set so skip this

			frx::Object* found = frx::Object::findObject( m_actorLookup[i] );
			if ( !found )
			{
				logError( "An actor with the name '" << m_actorLookup[i] << "' couold not be found when trying to join it"
					" to the joint '" << getName() << "'" );
				continue; ///Not foudn at the moment so we just return for now
			}
			else
			if ( found->isDerived( phys::Actor::TYPE ) )
			{
				setActor( (phys::Actor*)found, i );
			}
			else
			{
				logError( "The constraint '" << getName() << "' cannot be joined to '" 
					<< m_actorLookup[i] << "' as the object of that name is not a phys::Actor" );
				return false;
			}
		}

		return updateConstraintActors();
	}

	void Constraint::setActor( Actor* actor, Index index )
	{
		assert( index < 2 );
		m_actor[ index ] = actor;
	}

	void Constraint::setConstraint( eConstraint constraintType )
	{
		if ( m_constraintType == constraintType )
			return;

		m_constraintType = constraintType;
		setConstraintImpl();
	}

	bool Constraint::setAttributes( const AttributeMap& attributes )
	{
		//Call the base spatial set attributes
		if ( !Spatial::setAttributes( attributes ) )
			return false;

		AttributeMap::const_iterator iFind;
				
		//Check for the shape type attribute
		if ( (iFind = attributes.find( _T("type") )) != attributes.end() )
		{
			//Set the type of shape volume
			if ( iFind->second == _T("hinge") )
				setConstraint( Constraint_Hinge );
			else
			if ( iFind->second == _T("pin") || iFind->second == _T("nail") )
				setConstraint( Constraint_Pin );
			else
				setConstraint( Constraint_Invalid );
		}

		if ( (iFind = attributes.find( _T("body1") )) != attributes.end() )
			setActorLookup( iFind->second, 0 );

		if ( (iFind = attributes.find( _T("body2") )) != attributes.end() )
			setActorLookup( iFind->second, 1 );

		//Call the shape specific setAttrributes
		switch( getConstraintType() )
		{
		case Constraint_Hinge:
			return setHingeAttributes( attributes );
		case Constraint_Pin:
			return setPinAttributes( attributes );
		default:
			assert( false );
			return false;
		}
	}

	bool Constraint::setHingeAttributes( const AttributeMap& attributes )
	{
		assert( getConstraintType() == Constraint_Hinge );
		return true;
	}

	bool Constraint::setPinAttributes( const AttributeMap& attributes )
	{
		assert( getConstraintType() == Constraint_Pin );
		return true;
	}


	void Constraint::updateWorldData( Real tDelta, Real tApp )
	{
		if ( m_firstUpdate )
		{
			Spatial::updateWorldData( tDelta, tApp ); //Update the world pose to get the position etc
			if ( !lookupActors() )//Make sure we have looked up the actors the joint belongs to
				return; //< Don't initialise yet as we need the actors!
			initialise(); //< initialise the constraint
			m_firstUpdate = false;
		}
		else
		{
			//do nothing until we need to visualise the joint
		}
	}

} // END: phys
