/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\Phys\Physics.h
 * Status: duplicate of kismo-v0.3/Phys/Physics.h
 */
#ifndef PHYS_PHYSICS_H
#define	PHYS_PHYSICS_H

#include "Framework/Named.h"
#include "Framework/Rtti.h"
#include "Framework/Singleton.h"
#include "Framework/SmallSet.h"
#include "Framework/SmartPointer.h"
#include "Gfx/Scene.h"
#include "Gfx/Mesh.h"
#include "Phys/Scene.h"

#define _Physics phys::Physics::getSingleton()
#define _PhysicsPtr phys::Physics::getSingletonPtr()

//Forward delcaration
namespace gfx { class Scene; }

/** \brief Physics namespace contains classes for physics interaction with a ascene
*/
namespace phys
{
	//Forward declaration
	class Scene;
	class Actor;
	class Body;
	class Shape;

	/** \brief Physics base class
	\remarks 
		Base physics interface which can be specialised in a plugin to use a specific engine
	*/
	class _DllExport Physics : public frx::Named, public frx::Singleton< Physics >
	{		
	public:
		SINGLETON( Physics );

	protected:

		/** \brief Physics constructor
		\param[in] name The name of the renderer used to identify its type
		*/
		Physics( const String& name );

		/** \brief Virtual destructor
		*/
		virtual ~Physics();

	public:

		//Gets a list of all available options and their current values for the Physics
		//TODO: Option list
		//virtual void getConfigOptions() = 0;

		//Sets a configurable option for this Physics
		//virtual void setConfigOption(const String& name, const String& value) = 0;

		/** \brief Initialises & creates the physics scene
			\remarks Scene options should normally be set before hand but can eb set afterwards
		*/
		virtual Bool initialise() = 0;
		
		/** \brief Reinitialise the Physics if the config has changed. This will reset the scene
		*/
		virtual Bool reinitialise() = 0;
		
		/** \brief Deletes the physics scene and free's any memory used
		*/
		virtual void destroy() = 0;
		
		/** \brief Check if the Physics is initialised and is worth updating
			\returns True if initialised
		*/
		Bool initialised()
		{ return m_initialised; }

		/** \brief Create a new physics scene
		*/
		Scene::Ptr createScene( const String& name, const gfx::Scene::Ptr& gfxScene );

		/** \brief Find the ophysics scene that is in use in a graphics scene
		*/
		Scene::Ptr getScene( gfx::Scene* gfxScene );
		
		/** \brief Destroy a physics scene
		*/
		void releaseScene( Scene* scene );

		/** \brief Advance the simulation by a delta time step
		*/
		void update( Real tDelta, Real tApp );
		

		/** \brief Sets whether the physics are paused
		*/
		void setPaused( Bool paused )
		{ m_paused = paused; }

		/** \brief Gets whether the physics have been paused
		*/
		Bool getPaused() const
		{ return m_paused; }

		/** \brief Set whether the wireframe should be displayed
		*/
		void setWireframe( bool wire )
		{ m_wireframe = wire; }

		/** \brief get whether the collision shapes should be rendered in wireframe
		*/
		bool getWireframe() const
		{ return m_wireframe; }

	protected:

		/** \brief Call the scene implementation create for the specific physics plugin
		*/
		virtual Scene::Ptr createSceneImpl( const String& name ) = 0;

		/** \brief Destroy a physics scene
		*/
		virtual void releaseSceneImpl( Scene* scene ) = 0;

	protected:
		
		typedef frx::SmallSet< frx::SmartPointer< Scene > > SceneLookup;
		SceneLookup m_scenes; ///< vector set of all the scenes

		typedef Map< gfx::Scene*, frx::SmartPointer< Scene >  > SceneMap;
		SceneMap m_sceneGfxLookup; ///< Stores a map which relates the graphics scene to the physuics scene used within it.

	private:
		Bool m_initialised; ///< Whether the renderer is initialised
		Bool m_paused; //< Whether the physics has been paused
		Bool m_wireframe; ///< Whether the collison wireframe is to be shown
	};

} // END: phys

#endif