/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\framework\KismoLockFree.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef _KISMO_LOCKFREE_H
#define _KISMO_LOCKFREE_H

//
// Game Programming Gems 6
// Lock free multithreaded algorithms
// By Toby Jones (thjones@microsoft.com)
// Supports Microsoft Visual C++ and GCC.
//
#include "framework/KismoTypes.h"

#ifdef _MSC_VER
#pragma warning(push)
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

extern bool CAS_assembly(void * volatile * _ptr, void * oldVal, void * newVal);

//
// Define a version of CAS which uses the Visual C++ InterlockedCompareExchange intrinsic.
//
#ifdef _MSC_VER
// Define intrinsic for InterlockedCompareExchange
extern "C" long __cdecl _InterlockedCompareExchange(long volatile * Dest, long Exchange, long Comp);
#pragma intrinsic (_InterlockedCompareExchange)
extern bool CAS_intrinsic(void * volatile * _ptr, void * oldVal, void * newVal);
#endif  // _MSC_VER

//
// Define a version of CAS which uses the Windows API InterlockedCompareExchange.
//

extern bool CAS_windows(void * volatile * _ptr, void * oldVal, void * newVal);

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
extern bool CAS2_assembly(void * volatile * _ptr, void * old1, uint32_t old2, void * new1, uint32_t new2);

//
// Define a version of CAS2 which uses the Visual C++ InterlockedCompareExchange64 intrinsic.
//
#ifdef _MSC_VER
// Define intrinsic for InterlockedCompareExchange64
extern "C" __int64 __cdecl _InterlockedCompareExchange64(__int64 volatile * Destination, __int64 Exchange, __int64 Comperand);
#pragma intrinsic (_InterlockedCompareExchange64)
extern bool CAS2_intrinsic(void * volatile * _ptr, void * old1, uint32_t old2, void * new1, uint32_t new2);
#endif  // _MSC_VER

//
// Define a version of CAS2 which uses the Windows API InterlockedCompareExchange64.
// InterlockedCompareExchange64 requires Windows Vista
//
#if WINVER >= 0x0600
// LONGLONG InterlockedCompareExchange64(LONGLONG volatile * Destination, LONGLONG Exchange, LONGLONG Comperand);
extern bool CAS2_windows(void * volatile * _ptr, void * old1, uint32_t old2, void * new1, uint32_t new2);
#endif  // WINVER >= 0x0600




//
////------------------------------------------------------------------------------
////
//// Parameterized Lock-free Freelist
////
////------------------------------------------------------------------------------
//template<typename T> class LockFreeFreeList {
//    //
//    // Memory reclaimation is generally difficult with lock-free algorithms,
//    // so we bypass the situation by making the object own all of the memory,
//    // and creating and destroying the nodes on the thread that controls the
//    // object's lifetime.  Any thread synchronization should be done at that
//    // point.
//    //
//    LockFreeStack<T> _Freelist;
//    Node * _pObjects;
//    const uint32_t _cObjects;
//
//public:
//    //
//    // cObjects is passed to the constructor instead of a template parameter
//    // to minimize the code bloat of multiple freelists with varying sizes,
//    // but each using the same underlying type.
//    //
//    LockFreeFreeList(uint32_t cObjects) : _cObjects(cObjects)
//    {
//        //
//        // The Freelist may live on the stack, so we allocate the
//        // actual nodes on the heap to minimize the space hit.
//        //
//        _pObjects = new Node[cObjects];
//        FreeAll();
//    }
//    ~LockFreeFreeList()
//    {
//#ifdef _DEBUG
//        for(uint32_t ix = 0; ix < _cObjects; ++ix)
//        {
//            assert(_Freelist.Pop() != NULL);
//        }
//#endif
//
//        delete[] _pObjects;
//    }
//    void FreeAll()
//    {
//        for(uint32_t ix = 0; ix < _cObjects; ++ix)
//        {
//            _Freelist.Push(&_pObjects[ix]);
//        }
//    }
//    T * NewInstance()
//    {
//        Node * pInstance = _Freelist.Pop();
//        return new(&pInstance->value) T;
//    }
//    void FreeInstance(T * pInstance)
//    {
//        pInstance->~T();
//        _Freelist.Push(reinterpret_cast<Node *>(pInstance));
//    }
//};

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif