/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\RAGE_Demo\Framework\LockFree.cpp
 * Status: superseded by kismo-core/_SOURCE_/KismoFramework/LockFree.cpp (same name, content differs)
 */
#include "LockFree.h"

//
//#include <iostream>
//#include <new>
//#include <cassert>
//#include <vector>
//#include <algorithm>

#include <windows.h>
#include <process.h>


#ifdef _MSC_VER
#pragma warning(disable: 4127) // conditional expression is constant
#pragma warning(disable: 4311) // pointer truncation
#endif

//
// uint32_t is defined in standard C, but not yet in standard C++.
//
typedef unsigned int uint32_t;

//------------------------------------------------------------------------------
//
// Definitions of CAS.
//
//------------------------------------------------------------------------------

//
// Define what code we will be using for CAS.
// Intrinsic only available on Visual C++.
//
#ifndef CAS
#define CAS CAS_assembly
//#define CAS CAS_intrinsic
//#define CAS CAS_windows
#endif

// this function is atomic
// bool CAS(uint32_t *ptr, uint32_t oldVal, uint32_t newVal) {
//     if(*ptr == oldVal) {
//         *ptr = newVal;
//         return true;
//     }
//     return false;
// }

//
// All of the CAS functions will operate on a Node.  So we define Node first.
//


// CAS will assume a multi-processor machine (versus multithread on a single processor).
// On a single processor machine, it might not make sense to spin on a CAS because
// if it fails, there is no way to succeed until the another thread runs (which will be
// on the same processor).  In these cases, if CAS is required to succeed, it might make
// more sense to yield the processor to the other thread via Sleep(1).

// Since we are assuming a multi-processor machine, we will need to use 'lock cmpxchg'
// instead of just cmpxchg, as the bus needs to be locked to synchronize the processors
// access to memory.

//
// Define a version of CAS which uses x86 assembly primitives.
//

bool CAS_assembly(void * volatile * _ptr, void * oldVal, void * newVal)
{
    register bool f;

#ifdef __GNUC__
    __asm__ __volatile__(
        "lock; cmpxchgl %%ebx, %1;"
        "setz %0;"
            : "=r"(f), "=m"(*(_ptr))
            : "a"(oldVal), "b" (newVal)
            : "memory");
#else
    _asm
    {
        mov ecx,_ptr
        mov eax,oldVal
        mov ebx,newVal
        lock cmpxchg [ecx],ebx
        setz f
    }
#endif // __GNUC__

    return f;
}

//
// Define a version of CAS which uses the Visual C++ InterlockedCompareExchange intrinsic.
//
#ifdef _MSC_VER

// Define intrinsic for InterlockedCompareExchange
extern "C" long __cdecl _InterlockedCompareExchange(long volatile * Dest, long Exchange, long Comp);

#pragma intrinsic (_InterlockedCompareExchange)


bool CAS_intrinsic(void * volatile * _ptr, void * oldVal, void * newVal)
{
    return _InterlockedCompareExchange((long *)_ptr, (long)newVal, (long)oldVal) == (long)oldVal;
}

#endif  // _MSC_VER

//
// Define a version of CAS which uses the Windows API InterlockedCompareExchange.
//

bool CAS_windows(void * volatile * _ptr, void * oldVal, void * newVal)
{
    return InterlockedCompareExchange((long *)_ptr, (long)newVal, (long)oldVal) == (long)oldVal;
}

//------------------------------------------------------------------------------
//
// Definitions of CAS2.
//
//------------------------------------------------------------------------------

//
// Define what code we will be using for CAS.
// Intrinsic only available on Visual C++.
// Windows version only available on Windows Vista.
//
#ifndef CAS2
#define CAS2 CAS2_assembly
//#define CAS2 CAS2_intrinsic
//#define CAS2 CAS2_windows
#endif

//
// Define a version of CAS2 which uses x86 assembly primitives.
//
bool CAS2_assembly(void * volatile * _ptr, void * old1, uint32_t old2, void * new1, uint32_t new2)
{
    register bool f;
#ifdef __GNUC__
    __asm__ __volatile__(
        "lock; cmpxchg8b %1;"
        "setz %0;"
            : "=r"(f), "=m"(*(_ptr))
            : "a"(old1), "b" (new1), "c" (new2), "d" (old2)
            : "memory");
#else
    _asm
    {
        mov esi,_ptr
        mov eax,old1
        mov edx,old2
        mov ebx,new1
        mov ecx,new2
        lock cmpxchg8b [esi]
        setz f
    }
#endif
    return f;
}

//
// Define a version of CAS2 which uses the Visual C++ InterlockedCompareExchange64 intrinsic.
//
#ifdef _MSC_VER

// Define intrinsic for InterlockedCompareExchange64
extern "C" __int64 __cdecl _InterlockedCompareExchange64(__int64 volatile * Destination, __int64 Exchange, __int64 Comperand);

#pragma intrinsic (_InterlockedCompareExchange64)


bool CAS2_intrinsic(void * volatile * _ptr, void * old1, uint32_t old2, void * new1, uint32_t new2)
{
    LONGLONG Comperand = reinterpret_cast<LONG>(old1) | (static_cast<LONGLONG>(old2) << 32);
    LONGLONG Exchange  = reinterpret_cast<LONG>(new1) | (static_cast<LONGLONG>(new2) << 32);

    return _InterlockedCompareExchange64(reinterpret_cast<LONGLONG volatile *>(_ptr), Exchange, Comperand) == Comperand;
}

#endif  // _MSC_VER

//
// Define a version of CAS2 which uses the Windows API InterlockedCompareExchange64.
// InterlockedCompareExchange64 requires Windows Vista
//
#if WINVER >= 0x0600

// LONGLONG InterlockedCompareExchange64(LONGLONG volatile * Destination, LONGLONG Exchange, LONGLONG Comperand);


bool CAS2_windows(void * volatile * _ptr, void * old1, uint32_t old2, void * new1, uint32_t new2)
{
    LONGLONG Comperand = reinterpret_cast<long>(old1) | (static_cast<LONGLONG>(old2) << 32);
    LONGLONG Exchange  = reinterpret_cast<long>(new1) | (static_cast<LONGLONG>(new2) << 32);

    return InterlockedCompareExchange64(reinterpret_cast<LONGLONG volatile *>(_ptr), Exchange, Comperand) == Comperand;
}
#endif  // WINVER >= 0x0600
