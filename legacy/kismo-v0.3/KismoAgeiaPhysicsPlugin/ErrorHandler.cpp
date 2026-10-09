/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\KismoAgeiaPhysicsPlugin\ErrorHandler.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
#include "ErrorHandler.h"
#include "Framework/Log.h"


namespace phys
{
	void ErrorHandler::reportError( NxErrorCode errorCode, const char* message, const char* file, int line)
	{
		switch ( errorCode )
		{
		case NXE_INVALID_PARAMETER:
			{
				logRecoverable( "[PhysX] File: '" 
					<< file << "' Line: [" 
					<< line << "] - "  
					<< message );
				break;
			}

		case NXE_INVALID_OPERATION:
			{
				logRecoverable( "[PhysX] File: '" 
					<< file << "' Line: [" 
					<< line << "] - "  
					<< message );
				break;
			}
		case NXE_OUT_OF_MEMORY:
			{
				logError( "[PhysX] OUT_OF_MEMORY - File: '" 
					<< file << "' Line: [" 
					<< line << "] - "  
					<< message );
				break;
			}
		case NXE_DB_INFO:
			{
				logComment( "[PhysX] File: '" 
					<< file << "' Line: [" 
					<< line << "] - "  
					<< message );
				break;
			}
		case NXE_DB_WARNING:
			{
				logWarning( "[PhysX] File: '" 
					<< file << "' Line: [" 
					<< line << "] - "  
					<< message );
				break;
			}
		default:
			{
				logRecoverable( "[PhysX] UNKNOWN ERROR! - File: '"
					<< file << "' Line: [" 
					<< line << "] - "  
					<< message );
				break;
			}
		}
	}

	NxAssertResponse ErrorHandler::reportAssertViolation(const char* message, const char* file, int line)
	{
		frx::Log::eHandleAlert ret = logCritical( "[PhysX] ACCESS VIOLATION - File: '" 
					<< file << "' Line: [" 
					<< line << "] - "  
					<< message );

		switch ( ret )
		{

		case frx::Log::HandleAlert_Retry:
			return NX_AR_CONTINUE;
		case frx::Log::HandleAlert_Ignore:
			return NX_AR_IGNORE;
		case frx::Log::HandleAlert_Break:
		default:
			return NX_AR_BREAKPOINT;
		}
	}

	void ErrorHandler::print( const char* message )
	{
		logMessage( message );
	}

} //END: phys