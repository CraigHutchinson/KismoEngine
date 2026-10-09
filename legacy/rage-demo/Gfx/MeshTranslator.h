/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\Gfx\MeshTranslator.h
 * Status: duplicate of kismo-v0.3/Gfx/MeshTranslator.h
 */
#ifndef GFX_MESHTRANSLATOR_H
#define	GFX_MESHTRANSLATOR_H

#include "Framework/Types.h"
#include "Framework/Log.h"
#include "Framework/Resource.h"
#include "Gfx/Mesh.h"

namespace gfx
{

	/** \brief Mesh import/exporter
	*/
	class _DllExport MeshTranslator : public frx::ResourceTranslator
	{
		DECLARE_RTTI;
	protected:

		MeshTranslator( Bool canRead, Bool canWrite, const String& defaultExtension );

	public:

		virtual ~MeshTranslator();

		/** \brief \copydoc frx::ResourceTranslator::writer() 
		\remarks
			Verifies that the resource is of the correct type and if so calls the Mesh specific writer
		*/
		virtual bool writer( const frx::Resource* resource, OutStream& stream )
		{ 
			if ( resource->isDerived( Mesh::TYPE ) )
				return writer( (Mesh*)resource, stream );
			else
				return false;
		}

		/** \brief \copydoc frx::ResourceTranslator::reader()
		\remarks
			Verifies that the resource is of the correct type and if so calls the Mesh specific reader
		*/
		virtual bool reader( frx::Resource* resource, InStream& stream )
		{ 
			if ( resource->isDerived( Mesh::TYPE ) )
				return reader( (Mesh*)resource, stream );
			else
			{
				logRecoverable( "Invalid resource type passed to reader!" );
				return false;
			}
		}

		/** \brief Writes the supplied Mesh to the output stream 
		*/
		virtual bool writer( const Mesh* mesh, OutStream& stream )
		{ return false; }

		/** \brief Reads a Mesh from the supplied input stream 
		*/
		virtual bool reader( Mesh* mesh, InStream& stream )
		 { return false; }
	};

} // END: gfx


#endif