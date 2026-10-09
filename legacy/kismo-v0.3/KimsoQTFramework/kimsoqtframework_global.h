/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\KimsoQTFramework\kimsoqtframework_global.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef KIMSOQTFRAMEWORK_GLOBAL_H
#define KIMSOQTFRAMEWORK_GLOBAL_H

#include <Qt/qglobal.h>

#ifdef KIMSOQTFRAMEWORK_LIB
# define KIMSOQTFRAMEWORK_EXPORT Q_DECL_EXPORT
#else
# define KIMSOQTFRAMEWORK_EXPORT Q_DECL_IMPORT
#endif

#endif // KIMSOQTFRAMEWORK_GLOBAL_H
