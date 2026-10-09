/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\gfx\KismoTextureManager.cpp
 * Status: unique (no other copy of this file name in the archive)
 */
#include "KismoTextureManager.h"

using namespace kismo;
using namespace kismo::gfx;

template<> TextureManager* Singleton< TextureManager >::ms_instance = 0;
TextureManager& TextureManager::getSingleton( void ) { assert( isValidPointer( ms_instance ) ); return ( *ms_instance ); }
TextureManager* TextureManager::getSingletonPtr( void ) { return ms_instance;  }