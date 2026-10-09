/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo-0.2\Engine\framework\KismoLockFreeStack.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef _KISMO_LOCKFREESTACK_H
#define _KISMO_LOCKFREESTACK_H

#include "framework/KismoLockFree.h"
#include "framework/KismoPlatform.h"

//
// Game Programming Gems 6
// Lock free multithreaded algorithms
// By Toby Jones (thjones@microsoft.com)
// Supports Microsoft Visual C++ and GCC.
//

namespace kismo
{
	namespace frx
	{
		
		//------------------------------------------------------------------------------
		//
		// Parameterized Lock-free Stack
		//
		//------------------------------------------------------------------------------
		template<typename T>
		class _KismoExport LockFreeStack 
		{
		public:
			
			struct Node
			{
				T value;
				Node* volatile pNext;

				Node() : value(), pNext(NULL) {}
				Node(T v) : pNext(NULL), value(v) {}
			};

		public:

			LockFreeStack() : 
				_pHead(NULL),
				_cPops(0)
			{}

			void Push( Node* pNode )
			{
				while(true)
				{
					pNode->pNext = _pHead;
					if(CAS(&_pHead, pNode->pNext, pNode))
					{
						break;
					}
				}
			}
			Node* Pop()
			{
				while(true)
				{
					Node* pHead = _pHead;
					uint32_t  cPops = _cPops;
					if(NULL == pHead)
					{
						return NULL;
					}

					// NOTE: Memory reclaimation is difficult in this context.  If another thread breaks in here
					// and pops the head, and then frees it, then pHead->pNext is an invalid operation.  One solution
					// would be to use hazard pointers (http://researchweb.watson.ibm.com/people/m/michael/ieeetpds-2004.pdf).

					Node* pNext = pHead->pNext;
					if(CAS2(&_pHead, pHead, cPops, pNext, cPops + 1))
					{
						return pHead;
					}
				}
			}

		private:

			// NOTE: the order of these members is assumed by CAS2.
			Node* volatile _pHead;
			volatile uint32_t  _cPops;

		};

	} // END: frx
} // END: kismo



#endif