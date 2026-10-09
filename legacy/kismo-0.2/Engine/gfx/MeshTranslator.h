/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\gfx\MeshTranslator.h
 * Status: superseded by kismo-v0.3/Gfx/MeshTranslator.h (same name, content differs)
 */
#ifndef _KISMO_MESHTRANSLATOR_H
#define	_KISMO_MESHTRANSLATOR_H

#include "fileIO\KismoResource.h"

namespace kismo
{
	namespace gfx
	{
		//forward declaration
		class Mesh;

		//-----------------------------------------------------
		// Mesh import/exporter
		//-----------------------------------------------------
		class MeshTranslator
		{
		public:
			
			enum ke_FileMatch
			{				
				kc_FileMatch_IsMyFileType,		///< Translator understands how to read/write this file				
				kc_FileMatch_CouldBeMyFileType, ///< Translator is not best available to read/write this file				
				kc_FileMatch_NotMyFileType,		///< Translator does not understand how to read/write this file
			};

		public:
			/** Constructor which should be used by derived classes
			\param canRead Whether the read method should be allowed by the translator
			\param canWrite Whether the write method should be allowed by the translator 
			\param defaultExtension The default file extension */
  			MeshTranslator( k_Bool canRead, k_Bool canWrite, const k_String& defaultExtension );
			virtual ~MeshTranslator();
			/** Writes the supplied mesh to the output stream */
			virtual bool writer( const Mesh* mesh, DataStream& stream );
			/** Reads a mesh from the supplied input stream */
			virtual bool reader( Mesh* mesh, DataStream& stream );
			/** Whether the translator has a read method
			@remarks Returns false by default */
			bool hasReadMethod () const;
			/** Whether the translator has a write method
			@remarks Returns false by default */
			bool hasWriteMethod () const;			
			/** Returns the default file extension for this file translator excluding the period 
			at the beginning */
			const k_String& defaultExtension () const;
			/** Identifies whether the file translator can load the
			\param filename The filename of the file
			\param buffer Pointer to a buffer containing the header fo the file
			\param size The size of the buffer. Should be a minimum of 32bytes */
			virtual ke_FileMatch identifyFile( const k_String& filename, 
				const k_Byte* buffer, k_UShort size ) const;
		private:
			k_Bool m_hasReadMethod; ///< Whether a read method can be used			
			k_Bool m_hasWriteMethod; ///< Whether a write method can be used
			k_String m_defaultExtension; ///< The default file extension
		};

	} // END: gfx

} // END: kismo


#endif