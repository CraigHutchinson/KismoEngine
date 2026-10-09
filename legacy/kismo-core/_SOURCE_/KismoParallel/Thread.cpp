/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\KismoCore\_SOURCE_\KismoParallel\Thread.cpp
 * Status: current (newest copy of this file name in the archive)
 */
#include "Thread.h"
#include "KismoFramework/String.h"

namespace kismo
{
	Count Thread::kDestructorTimeoutMs = 10000;
	
	eThreadPriority getThreadPriorityFromName( const Char* value )
	{
		if( measure(value) == 0 )
			return ThreadPriority_Normal; //Default 'other control type'
		if ( compare<false>( value, _T("Idle")) == 0 ) 
			return ThreadPriority_Idle;
		if ( compare<false>( value, _T("Low")) == 0 ) 
			return ThreadPriority_Low;
		if ( compare<false>( value, _T("BelowNormal")) == 0 ) 
			return ThreadPriority_BelowNormal;
		if ( compare<false>( value, _T("Normal")) == 0 ) 
			return ThreadPriority_Normal;
		if ( compare<false>( value, _T("AboveNormal")) == 0 ) 
			return ThreadPriority_AboveNormal;
		if ( compare<false>( value, _T("High")) == 0 ) 
			return ThreadPriority_High;
		if ( compare<false>( value, _T("TimeCritical")) == 0 ) 
			return ThreadPriority_TimeCritical;
		return ThreadPriority_Normal;

	}
} //END: kismo