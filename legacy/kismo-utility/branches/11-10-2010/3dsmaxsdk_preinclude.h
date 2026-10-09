/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo Utility\branches\11-10-2010\3dsmaxsdk_preinclude.h
 * Status: superseded by kismo-utility/splitversion/3dsmaxsdk_preinclude.h (same name, content differs)
 */
//**************************************************************************/
// Copyright (c) 1998-2005 Autodesk, Inc.
// All rights reserved.
// 
// These coded instructions, statements, and computer programs contain
// unpublished proprietary information written by Autodesk, Inc., and are
// protected by Federal copyright law. They may not be disclosed to third
// parties or copied or duplicated in any form, in whole or in part, without
// the prior written consent of Autodesk, Inc.
//**************************************************************************/
// DESCRIPTION: Include this file before including any 3ds Max SDK files. It 
//              define the macros required to add linkable todo compile-time 
//              messages and lower the warning level to 3.
// AUTHOR: Jean-Francois Yelle - created Mar.20.2007
//***************************************************************************/

// useful for #pragma message
#define STRING2(x) #x
#define STRING(x) STRING2(x)
#define TODO(x) __FILE__ "(" STRING(__LINE__) "): TODO: "x