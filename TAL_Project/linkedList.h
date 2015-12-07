#ifndef LINKED_LIST_H_INCLUDED
#define LINKED_LIST_H_INCLUDED

#include "baseObject.h"

class TLinkedMaillon;

namespace CustomStd
{
	class STDLinkedList : public STDBaseObject
	{
	protected:
		TLinkedMaillon *m_pHead;
		int m_iSize;

	public:
		STDLinkedList();
		~STDLinkedList();

		int GetSize()const;

		bool Add(STDBaseObject *);
		bool Remove(STDBaseObject *);
		bool RemoveHead();

		class STDIterator{
		public:
			TLinkedMaillon *m_pMaillon;

			bool operator<= (const STDIterator &);
			STDBaseObject *operator*();
			STDIterator operator++ (int);
		};
		STDIterator Begin();
		STDIterator End();
	};
}

#endif
