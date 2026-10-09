/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\gui\KismoConsole.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef _KISMO_CONSOLE_H
#define	_KISMO_CONSOLE_H

#include "framework/KismoPlatform.h"
#include "framework/KismoCommon.h"
#include "framework/KismoVector.h"
#include "framework/KismoLog.h"

#define MAX_CONSOLE_LINES 1024

namespace kismo
{
	namespace gui
	{
		class _KismoExport Console : 
			public k_OStream//,
			//private Log::LogListener
		{
		public:
			enum ke_ConsoleType
			{
				kc_default, //Default ( Windows normal 'cmd' type console )
				kc_overlay, //Overlay on primary output window
				kc_auto, //Automatically picks the best option
			};		

		public:
			Console( k_Bool showTheDefault = kc_TRUE );
			virtual ~Console();

			//Show a console
			void open( ke_ConsoleType type = kc_default );
			void close( ke_ConsoleType type = kc_auto );

			inline friend k_OStream& operator << ( k_OStream& outStream, const Console& v )
			{
				outStream << (k_OStream)v.rdbuf();
				return outStream;
			}
		protected:

			virtual void write( const k_String& message, frx::Log::ke_MessageType type )
			{
				//(*(std::ostream*)this) << DateTime() << " | "  ;					
				(*(k_OStream*)this) << _T("LOG: ") << type << _T(" | ") << message.c_str() << std::endl;
			}

			//------------------------------------
			// Internal specific console type functions
			//------------------------------------			
			void openDefault(); //Show the default console ( Windows normal 'cmd' type console )
			void closeDefault(); //Hide the default console ( Windows normal 'cmd' type console )

			//void showOverlay();//Show the overlay console on the primary output window
			//void closeOverlay();

		private:
			void* m_consoleHandle; //Handle to the Win32 console

			std::basic_filebuf< k_Char >* m_fileBuffer; //Pointer to the output file buffer 
			std::basic_filebuf< k_Char > m_Nul; //Null buffer for when console is closed 

			FILE* m_fNul;                     
			FILE* m_consoleFile;
		};

	} // END: UI

} // END: kismo

#endif
