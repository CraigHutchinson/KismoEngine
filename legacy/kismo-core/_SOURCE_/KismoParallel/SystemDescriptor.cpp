/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\KismoCore\_SOURCE_\KismoParallel\SystemDescriptor.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
#include "SystemDescriptor.h"

#if KISMO_PLATFORM == KISMO_PLATFORM_WINDOWS
#	include "KismoParallel\SystemDescriptor_Win32.inl"
#else
#error Unsupported platform for Threading::SystemDescriptor
#endif
