/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\gfx\KismoMeshManager.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
#include "KismoMeshManager.h"

using namespace kismo;
using namespace kismo::gfx;

MeshManager::Translators meshTranslators();

template<> MeshManager* Singleton< MeshManager >::ms_instance = 0;
MeshManager& MeshManager::getSingleton( void ) { assert( isValidPointer( ms_instance ) ); return ( *ms_instance ); }
MeshManager* MeshManager::getSingletonPtr( void ) { return ms_instance;  }