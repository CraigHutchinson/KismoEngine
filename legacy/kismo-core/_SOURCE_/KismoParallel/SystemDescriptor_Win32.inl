/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\KismoCore\_SOURCE_\KismoParallel\SystemDescriptor_Win32.inl
 * Status: unique (no other copy of this file name in the archive)
 */
#include "SystemDescriptor.h"

#define WIN32_LEAN_AND_MEAN  //Thin windows headers a bit
#include <windows.h>

namespace kismo 
{
	SystemDescriptor::SystemDescriptor() 
		: m_processorCount(0)
		, m_physicalMemoryBytes(0)
		, m_hyperThreadAvailability(false)
	{
		MEMORYSTATUS memInfo;
		SYSTEM_INFO SystemInfo;
		memInfo.dwLength=sizeof(MEMORYSTATUS);
		GlobalMemoryStatus(&memInfo); 
		GetSystemInfo(&SystemInfo);
		
		m_physicalMemoryBytes = memInfo.dwTotalPhys;
	}
	
	Size SystemDescriptor::getSystemMemoryLoadBytes() const
	{
		MEMORYSTATUS memInfo;
		memInfo.dwLength=sizeof(MEMORYSTATUS);
		GlobalMemoryStatus(&memInfo); 
		return (Size)memInfo.dwMemoryLoad;
	}

}
 