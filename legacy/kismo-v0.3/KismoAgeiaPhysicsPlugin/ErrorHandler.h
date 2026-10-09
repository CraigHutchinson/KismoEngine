/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\KismoAgeiaPhysicsPlugin\ErrorHandler.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef PHYSX_ERRORHANDLER_H
#define PHYSX_ERRORHANDLER_H

#include <NxUserOutputStream.h>

namespace phys
{
	class ErrorHandler : public NxUserOutputStream
	{
	public:
		void reportError(NxErrorCode e, const char* message, const char* file, int line);

		NxAssertResponse reportAssertViolation(const char* message, const char* file, int line);

		void print(const char* message);
	};

} //END: phys


#endif
