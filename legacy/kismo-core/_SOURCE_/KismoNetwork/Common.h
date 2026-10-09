/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\KismoCore\_SOURCE_\KismoNetwork\Common.h
 * Status: superseded by kismo-core/_SOURCE_/KismoGL/Common.h (same name, content differs)
 */
#ifndef KISMONETWORK_COMMON_H
#define KISMONETWORK_COMMON_H

#include "KismoFramework/Types.h"

// Dll export interface
#ifdef _DLL
#ifdef KISMONETWORK_EXPORTS	
#	define KISMONETWORK_EXPORT __declspec(dllexport)
#else	
#	define KISMONETWORK_EXPORT __declspec(dllimport)
#endif
#else
#	define KISMONETWORK_EXPORT
#endif

namespace kismo 
{
	/**
		\brief	Namespace containing tcp/udp INternet/network resources for netowkr play and
				remote collaboration
	*/
	namespace network
	{	
	} /* END: new */
} /*END: kismo*/

#endif