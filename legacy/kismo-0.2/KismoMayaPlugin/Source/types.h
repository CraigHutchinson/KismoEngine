/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\KismoMayaPlugin\Source\types.h
 * Status: superseded by kismo-core/_SOURCE_/KismoFramework/Types.h (same name, content differs)
 */
//defines global types
#ifndef TYPES_H
#define	TYPES_H


typedef unsigned int UINT;
typedef unsigned long ULONG;
typedef unsigned short USHORT;

typedef float FLOAT;
typedef double LFLOAT;

typedef UINT INDICE; //Mesh indices
typedef UINT COUNT; //Count value
typedef UINT INDEX; //Index - shoudl be the same as COUNT
typedef UINT FVFVALUE; //The Type for the Flexible Vertex flag

typedef unsigned char BYTE; //8bit byte
typedef unsigned long COLOUR; //RGBA colour value

//Define from Microsoft Direct3D
#define COLOUR_ARGB(a,r,g,b) ((COLOUR)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))
#define COLOUR_RGB(r,g,b) ((COLOUR)((((1)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))

#endif
