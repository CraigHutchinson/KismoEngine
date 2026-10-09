/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\KismoCore\_SOURCE_\KismoParallel\SystemDescriptor.h
 * Status: current (newest copy of this file name in the archive)
 */
#ifndef KISMOPARALLEL_SYSTEMDESCRIPTOR_H
#define KISMOPARALLEL_SYSTEMDESCRIPTOR_H

#include "KismoParallel\Common.h"

namespace kismo
{
	class SystemDescriptor
	{
	public:
		SystemDescriptor();

		/** Get number fo physical processors
		*/
		inline Count getProcessorCount() const;

		/** Get whether Hyper-Threading is supported
			\remarks This enabled upto 2 concurrent threads to execute on a single physical processor
		*/
		inline bool getHyperThreadAvailability() const;
		
		/* Get the physical memory size in bytes
		*/
		inline Size getPhysicalMemoryBytes() const;

		/** Get the system memory load in bytes
		*/
		Size getSystemMemoryLoadBytes() const;

	private:
		Count m_processorCount;
		Size m_physicalMemoryBytes;
		bool m_hyperThreadAvailability;
	};
	
	Count SystemDescriptor::getProcessorCount() const
	{ return m_processorCount; }

	bool SystemDescriptor::getHyperThreadAvailability() const
	{ return m_hyperThreadAvailability; }

	Size SystemDescriptor::getPhysicalMemoryBytes() const
	{ return m_physicalMemoryBytes; }

} //END: kismo

/*
#if KISMO_PLATFORM == KISMO_PLATFORM_WINDOWS
#	include "KismoParallel\SystemDescriptor_Win32.inl"
#else
#error Unsupported platform for Threading::SystemDescriptor
#endif
*/

#endif