/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\KismoFRX\LockFreeQueue.h
 * Status: superseded by kismo-core/_SOURCE_/KismoFramework/LockFreeQueue.h (same name, content differs)
 */
#ifndef KISMOFRX_LOCKFREEQUEUE_H
#define KISMOFRX_LOCKFREEQUEUE_H

#include "KismoFRX/LockFree.h"
//#include "Platform/Platform.h"

//
// Game Programming Gems 6
// Lock free multithreaded algorithms
// By Toby Jones (thjones@microsoft.com)
// Supports Microsoft Visual C++ and GCC.
//

namespace kismo
{		
	/** \brief Parameterized Lock-free Queue
	*/
	template< typename T>
	class KISMOFRX_EXPORT LockFreeQueue 
	{
	public:

		/** \brief Templated node for lock free queue
		*/
		struct Node 
		{
			T value;
			Node* volatile pNext;

			Node() : value(), pNext(0) {}
			Node( const T& v) : pNext(0), value(v) {}
		};

	public:

		LockFreeQueue( ) : 
			_cPops(0), 
			_cPushes(0)
		{
			_pHead = _pTail =  new Node();
		}

		~LockFreeQueue()
		{
			//clear the gueue incase there are still nodes. If the nodes point to data then memory leaks may occur
			clear();

			assert( _pHead == _pTail );//Queue not empty

			delete _pHead; //Remove the head
		}

		void push( const T& val )
		{
			push( new Node( val ) ); 		
		}

		void push( Node* pNode )
		{
			pNode->pNext = NULL;

			uint32_t cPushes;
			Node* pTail;

			while(true)
			{
				cPushes = _cPushes;
				pTail = _pTail;

				// NOTE: The Queue has the same consideration as the Stack.  If _pTail is
				// freed on a different thread, then this code can cause an access violation.

				// If the Node that the tail points to is the last Node
				// then update the last Node to point at the new Node.
				if(CAS((void *volatile *)&(_pTail->pNext), reinterpret_cast<Node *>(NULL), pNode))
				{
					break;
				}
				else
				{
					// Since the tail does not point at the last Node,
					// need to keep updating the tail until it does.
					CAS2((void *volatile *)&_pTail, pTail, cPushes, _pTail->pNext, cPushes + 1);
				}
			}

			// If the tail points to what we thought was the last Node
			// then update the tail to point to the new Node.
			CAS2((void *volatile *)&_pTail, pTail, cPushes, pNode, cPushes + 1);
		}

		Node* pop() 
		{
			T value = T();
			Node * pHead;

			while(true)
			{
				uint32_t cPops = _cPops;
				uint32_t cPushes = _cPushes;
				pHead = _pHead;
				Node* pNext = pHead->pNext;

				// Verify that we did not get the pointers in the middle
				// of another update.
				if(cPops != _cPops)
				{
					continue;
				}
				// Check if the queue is empty.
				if(pHead == _pTail)
				{
					if(NULL == pNext)
					{
						pHead = NULL; // queue is empty
						break;
					}
					// Special case if the queue has nodes but the tail
					// is just behind. Move the tail off of the head.
					CAS2((void *volatile *)&_pTail, pHead, cPushes, pNext, cPushes + 1);
				}
				else if(NULL != pNext)
				{
					value = pNext->value;
					// Move the head pointer, effectively removing the Node
					if(CAS2((void *volatile *)&_pHead, pHead, cPops, pNext, cPops + 1))
					{
						break;
					}
				}
			}
			if(NULL != pHead)
			{
				pHead->value = value;
			}
			return pHead;
		}

		void clear()
		{
			while ( !empty() )
			{
				delete pop();
			}
		}


		//--- Possibly not safe bits? ----
		bool empty() { assert( _cPushes >= _cPops ); return _cPushes == _cPops; }

	private:

		// NOTE: the order of these members is assumed by CAS2.
		Node* volatile _pHead;
		volatile uint32_t  _cPops;
		Node* volatile _pTail;
		volatile uint32_t  _cPushes;
	};

} // END: frx


#endif