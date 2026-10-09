/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoAI\OpsaiLmlLoader.h
 * Status: unique (no other copy of this file name in the archive)
 */
/** \author Craig Hutchinson \file 
	\brief 
*/
#ifndef OPSAI_LMLLOADER_H
#define OPSAI_LMLLOADER_H

#include "OpsaiOperatorStore.h"
#include "OpsaiWorldState.h"

namespace opsai
{
	/** \brief Opsai Loader for operators store
	*/
	class LmlOperatorStoreLoader : public OperatorStoreLoader
	{
	public:
		LmlOperatorStoreLoader(  Dictionary& dictionary, const String& filename );
		virtual Bool load( OperatorStore& operatorStore );
	};

	/** \brief Opsai Loader for the worldState
	*/
	class LmlWorldStateLoader : public WorldStateLoader
	{
	public:
		LmlWorldStateLoader( Dictionary& dictionary, const String& filename );
		virtual Bool load( WorldState& worldState );
		virtual Bool load( State::Array& states, const String& nodeName = String() );
	};

} //END: opsai

#endif