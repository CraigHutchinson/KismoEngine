/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoPlatform\GLFont.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef GLFONT_H
#define GLFONT_H

#include "KismoPlatform/Common.h"
extern GLvoid BuildFont( HDC hDC  );
extern GLvoid KillFont(GLvoid)	;
extern GLvoid glPrint(const char *fmt, ...);

#endif
