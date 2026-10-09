/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\Gfx\SceneTranslator.h
 * Status: duplicate of kismo-v0.3/Gfx/SceneTranslator.h
 */
#ifndef GFX_SCENETRANSLATOR_H
#define	GFX_SCENETRANSLATOR_H

#include "Framework/Types.h"
#include "Framework/Log.h"
#include "Framework/Resource.h"
#include "Gfx/Scene.h"

namespace gfx
{

	/** \brief Scene import/exporter
	*/
	class _DllExport SceneTranslator : public frx::ResourceTranslator
	{
		DECLARE_RTTI;
	protected:

		SceneTranslator( Bool canRead, Bool canWrite, const String& defaultExtension );

	public:

		virtual ~SceneTranslator();

		/** \brief \copydoc frx::ResourceTranslator::writer() 
		\remarks
			Verifies that the resource is of the correct type and if so calls the Scene specific writer
		*/
		virtual bool writer( const frx::Resource* resource, OutStream& stream )
		{ 
			if ( resource->isDerived( SceneResource::TYPE ) )
				return writer( (Scene::Ptr)((SceneResource*)resource)->getScene(), stream );
			else
				return false;
		}

		/** \brief \copydoc frx::ResourceTranslator::reader()
		\remarks
			Verifies that the resource is of the correct type and if so calls the Scene specific reader
		*/
		virtual bool reader( frx::Resource* resource, InStream& stream )
		{ 
			if ( resource->isDerived( SceneResource::TYPE ) )
				return reader( (Scene::Ptr)((SceneResource*)resource)->getScene(), stream );
			else
			{
				logRecoverable( "Invalid resource type passed to reader!" );
				return false;
			}
		}

		/** \brief Writes the supplied Scene to the output stream 
		*/
		virtual bool writer( const Scene::Ptr scene, OutStream& stream )
		{ return false; }

		/** \brief Reads a Scene from the supplied input stream 
		*/
		virtual bool reader( Scene::Ptr scene, InStream& stream )
		 { return false; }
	};

} // END: gfx


#endif