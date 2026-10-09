/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\gui\KismoConsole.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
#include "KismoConsole.h"

#include <windows.h>
#include <fcntl.h>
#include <io.h>
using namespace std;

namespace kismo
{
	namespace gui
	{
		Console::Console( k_Bool showTheDefault ) :
			m_fNul( fopen( "nul", "w" ) ),
			m_Nul( m_fNul ),
			k_OStream( &m_Nul ),
			//Log::LogListener( Log::kc_normal ),
			m_fileBuffer( 0 ),
			m_consoleHandle( INVALID_HANDLE_VALUE )
		{
			//show the default console if specified that we should do
			if ( showTheDefault )
				open( kc_default );
		}

		Console::~Console()
		{
			if ( m_fileBuffer )
				delete m_fileBuffer; //Free the file buffer

			if ( m_consoleHandle != INVALID_HANDLE_VALUE ) 
			{
				FreeConsole(); //Close the console window
				fclose( m_consoleFile );
			}

			fclose( m_fNul );
		}

		void Console::open( ke_ConsoleType type )
		{
			if ( type == kc_auto )
				type = kc_default;

			switch( type )
			{
			case kc_overlay:
				{
					//TODO:
					break;
				}
			case kc_default:
			default:
				{
					openDefault();
					break;
				}
			}
		}

		void Console::close( ke_ConsoleType type )
		{
			if ( type == kc_auto )
				type = kc_default;

			switch( type )
			{
			case kc_overlay:
				{
					//TODO:
					break;
				}
			case kc_default:
			default:
				{
					closeDefault();
					break;
				}
			}
		}

		void Console::openDefault()
		{
			if ( m_consoleHandle != INVALID_HANDLE_VALUE ) 
				return; //Bail as the console is already initialised!

			AllocConsole();// allocate the console so it becomes visible

			m_consoleHandle = GetStdHandle( STD_OUTPUT_HANDLE ); //Get the handle to the console

			int handle = _open_osfhandle( (k_Size)m_consoleHandle, _O_TEXT ); //Get a handle to the console stream
			
			m_consoleFile = _fdopen( handle, "w" ); //Get a file pointer for the consoles stream

			m_fileBuffer = new basic_filebuf< k_Char >( m_consoleFile ); //Create a filebuffer which uses the consoles file object

			init( m_fileBuffer ); //init the filebuffer with the new fileBuffer objects to 

			setf( ios::unitbuf );

			SetConsoleTextAttribute( m_consoleHandle, 
					FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE |
					BACKGROUND_BLUE
				);

			//clar the console
			system( "cls" );

		}

		void Console::closeDefault()
		{
			if ( m_consoleHandle != INVALID_HANDLE_VALUE ) 
			{
				init( &m_Nul ); //Set the null file buffer as the target for output
		        
				FreeConsole(); //Close the console

				fclose( m_consoleFile ); //Close the output file

				m_consoleHandle = INVALID_HANDLE_VALUE;

				if ( m_fileBuffer)
					delete m_fileBuffer;
			}

		}


			
			
			//*stdout = *(ostream::rdbuf());

			//ostream::_Tiestr = fp;
			
			//setvbuf( stdout, NULL, _IONBF, 0 );

			/*
			// redirect unbuffered STDIN to the console

			lStdHandle = (long)GetStdHandle(STD_INPUT_HANDLE);

			hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);

			fp = _fdopen( hConHandle, "r" );

			*stdin = *fp;

			setvbuf( stdin, NULL, _IONBF, 0 );
			*/

			// redirect unbuffered STDERR to the console

			/*
			lStdHandle = (long)GetStdHandle(STD_ERROR_HANDLE);

			hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);

			fp = _fdopen( hConHandle, "w" );

			*stderr = *fp;

			setvbuf( stderr, NULL, _IONBF, 0 );
			*/

			// make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog

			// point to console as well

			//ios::sync_with_stdio();
	}
}