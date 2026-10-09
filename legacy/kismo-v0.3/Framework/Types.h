/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.3\Framework\Types.h
 * Status: superseded by kismo-core/_SOURCE_/KismoFramework/Types.h (same name, content differs)
 */
#ifndef TYPES_H
#define TYPES_H

#include "Framework/BaseTypes.h"
#include <cassert> //Include here so everywhere can get access to assert

//STL classes until faster more memory efficient versions are created
//#include <vector>
//#define Vector std::vector
#include <map>
#define Map std::map
#define MultiMap std::multimap
#include <list>
#define List std::list
#include <set>
#define Set std::set
#include <bitset>
#define BitSet std::bitset
#include <stack>
#define Stack std::stack
#include <queue>
#define Queue std::queue

#include "Framework/String.h"
typedef frx::StringT String;
typedef frx::String8 String8;
typedef frx::String16 String16;

/** \brief typedef for a data structure used to pass data for unkniwn types into a constructor
*/
typedef Map< String, String > AttributeMap;

#endif