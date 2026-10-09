/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\Gfx\Scene.h
 * Status: duplicate of kismo-v0.3/Gfx/Scene.h
 */
#ifndef GFX_SCENE_H
#define	GFX_SCENE_H

#include "Gfx/Node.h"
#include "Framework/Resource.h"

namespace gfx
{	
	//forward declarations
	class SceneTranslator;
	class Renderer;

	/** \brief Scene node defines the base node foe any scene graph.
		\remarks Derives from Node which can be used as root but adds frx::Resource to add
		serialisation while using a SceneTranslator implementation
	*/
	class _DllExport Scene : public Node
	{
		DECLARE_RTTI;
	public:

		typedef frx::SmartPointer< Scene > Ptr;

	public:

		Scene( const String& sceneID, Renderer* renderer );

		virtual ~Scene();


		/** \brief Get the renderer used for the scene
		*/
		Renderer* getRenderer()
		{ return m_renderer; }

		/** \brief Create a new child node to another of a specified type and attribute
			\param[in] parentNode The node which will be parent. Default to 0 = this
			\param[in] id The id given to the scene node. Must be unique
			\param[in] typeName The type of Node to create e.g. Node, Geometry, Phys:Actor
			\param[in] attributes The New nodes attributes
		*/
		Spatial* createChild( const String& id, const String& typeName, const AttributeMap& attributes, Spatial* parentNode = 0 );

		/** \brief Create a new child node to another of a specified type
			\param[in] parentNode The node which will be parent. Default to 0 = this
			\param[in] id The id given to the scene node. Must be unique
			\param[in] typeName The type of Node to create e.g. Node, Geometry, Phys:Actor
		*/
		Spatial* createChild( const String& id, const String& typeName, Spatial* parentNode = 0 );
		
		/** \brief Create a new child node to another
			\param[in] parentNode The node which will be parent. Default to 0 = this
			\param[in] id The id given to the scene node. Must be unique
		*/
		Node* createChildNode( const String& id, Node* parentNode = 0 );

		/** \brief release a node and destroy it
		*/
		void releaseNode( Spatial* node );


	protected:
		Renderer* m_renderer; //< The renderer used to create the scene
	};

	
	/** \brief Scene resource class holds the root scene node and allows loading and saving of the scene state
	*/
	class _DllExport SceneResource : public frx::Resource
	{
		DECLARE_RTTI;
	public:

		SceneResource( frx::ResourceManager* owner, const String& sceneID, SceneTranslator* loader, const String& filename, Renderer* renderer );
		
		/** \brief \copydoc frx::Resource::load()
		*/
		virtual bool load();

		/** \brief \copydoc frx::Resource::destroy()
		*/
		virtual void unload();

		const Scene::Ptr& getScene() const
		{ return m_scene; }

	private:
		Scene::Ptr m_scene;
	};

} //END: gfx

#endif