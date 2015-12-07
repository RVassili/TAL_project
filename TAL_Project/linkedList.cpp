#include "linkedList.h"
#include "linkedMaillon.h"

namespace CustomStd
{
	STDLinkedList::STDLinkedList(){
		m_iSize = 0;
		m_pHead = NULL;
	}
	STDLinkedList::~STDLinkedList(){
		if (m_pHead != NULL)
			delete m_pHead;
	}

	int STDLinkedList::GetSize()const{
		return this->m_iSize;
	}
	bool STDLinkedList::Add(STDBaseObject *pObject){
		TLinkedMaillon *pMaillon = new TLinkedMaillon();
		pMaillon->SetObject(pObject);

		if (m_pHead == NULL){
			m_pHead = pMaillon;
			m_iSize = pMaillon->GetSize();
			return true;
		}

		if (m_pHead->Parcours(AddMaillon, pMaillon, NULL)){
			m_iSize += pMaillon->GetSize();
			return true;
		}

		return false;
	}
	bool STDLinkedList::Remove(STDBaseObject *pObject){
		TLinkedMaillon *pMaillon = new TLinkedMaillon();
		pMaillon->SetObject(pObject);

		if (pMaillon == NULL)
			return false;

		if (m_pHead == NULL)
			return false;

		return m_pHead->Parcours(RemoveMaillon, pMaillon, NULL);
	}
	bool STDLinkedList::RemoveHead(){
		if (m_pHead == NULL)
			return false;

		m_pHead = m_pHead->GetNext();
		m_iSize--;
		return true;
	}

	STDLinkedList::STDIterator STDLinkedList::Begin(){
		STDLinkedList::STDIterator tIt;
		tIt.m_pMaillon = this->m_pHead;
		return tIt;
	}
	STDLinkedList::STDIterator STDLinkedList::End(){
		STDLinkedList::STDIterator tIt;
		TLinkedMaillon *pMaillon = m_pHead;

		if (pMaillon == NULL)
		{
			tIt.m_pMaillon = pMaillon;
			return tIt;
		}

		while (pMaillon->GetNext() != NULL)
			pMaillon = pMaillon->GetNext();

		tIt.m_pMaillon = pMaillon;
		return tIt;
	}
	bool STDLinkedList::STDIterator::operator<= (const STDIterator &tA){
		TLinkedMaillon *pMaillon = this->m_pMaillon;
		while (pMaillon != NULL)
		{
			if (*pMaillon == *tA.m_pMaillon)
				return true;
			pMaillon = pMaillon->GetNext();
		}

		return false;
	}
	STDBaseObject *STDLinkedList::STDIterator::operator*(){
		return this->m_pMaillon->GetObjectM();
	}
	STDLinkedList::STDIterator STDLinkedList::STDIterator::operator++ (int){
		m_pMaillon = m_pMaillon->GetNext();
		return *this;
	}
}
