/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\framework\KismoException.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef _KISMO_EXCEPTION_H
#define	_KISMO_EXCEPTION_H

#include "KismoTypes.h"

namespace kismo
{
	namespace frx
	{		
		//-----------------------------------------------------
		// Exception type
		//-----------------------------------------------------
		class Exception
		{
		public:
			enum ke_Exception {
				kc_Exception_FILE_NOT_FOUND,
				kc_Exception_CANNOT_WRITE_TO_FILE,			
				kc_Exception_INVALID_PARAM,
				kc_Exception_ASSERTION_FAILED, 
				kc_Exception_NOT_IMPLEMENTED,
				kc_Exception_UNKNOWN,
			};

		public:
			Exception( ke_Exception code, const k_String& description, const k_String& source );
			Exception( ke_Exception code, const k_String& description, const k_String& source, k_Char* file, k_Int32 line );


		protected:
			k_Int32 m_line;
			k_Int32 m_number;
			k_String m_description;
			k_String m_source;
			k_String m_file;
			static Exception* m_last;
		};
	} //END: frx
} //END: kismo

#endif