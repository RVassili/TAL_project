#ifndef LINKEDMAILLON_H_INCLUDED
#define LINKEDMAILLON_H_INCLUDED

#include "baseObject.h"

class TLinkedMaillon
{
protected:
	CustomStd::STDBaseObject *m_pObject;
	TLinkedMaillon *m_pNext;

public:
	TLinkedMaillon()
	{
		m_pObject = NULL;
		m_pNext = NULL;
	}
	~TLinkedMaillon()
	{

	}

	void SetObject(CustomStd::STDBaseObject *pObj)
	{
		m_pObject = pObj;
	}
	void SetNext(TLinkedMaillon *pNext)
	{
		this->m_pNext = pNext;
	}

	CustomStd::STDBaseObject *GetObjectM()const
	{
		return m_pObject;
	}
	TLinkedMaillon *GetNext()const
	{
		return this->m_pNext;
	}
	int GetSize()const
	{
		if (m_pObject == NULL)
			return 0;

		if (this->GetNext() == NULL)
			return 1;
		return 1 + this->GetNext()->GetSize();
	}

	bool operator== (const TLinkedMaillon &tMaillon)
	{
		return *m_pObject == *(tMaillon.GetObjectM());
	}
	bool operator<= (const TLinkedMaillon &tMaillon)
	{
		TLinkedMaillon *pMaillon = this;
		while (pMaillon != NULL)
		{
			if (*pMaillon == tMaillon)
				return true;
			pMaillon = pMaillon->GetNext();
		}
		return false;
	}

	bool Parcours(bool(*pF)(TLinkedMaillon *, TLinkedMaillon *, TLinkedMaillon *), TLinkedMaillon *pNewMaillon, TLinkedMaillon *pOldMaillon)
	{
		TLinkedMaillon *pPrevious = this;
		TLinkedMaillon *pActual = this->m_pNext;

		while (pActual != NULL){
			if (pOldMaillon != NULL){
				if (*pOldMaillon == *pActual)
					return pF(pPrevious, pActual, pNewMaillon);
			}

			pPrevious = pActual;
			pActual = pActual->m_pNext;
		}

		if (pNewMaillon != NULL)
			return pF(pPrevious, pActual, pNewMaillon);

		return false;
	}
};

//ajoute pNewMaillon apres pPrevious si pActual est nul
//sinon insere pNewMaillon apres pActual
static bool AddMaillon(TLinkedMaillon *pPrevious, TLinkedMaillon * pActual, TLinkedMaillon * pNewMaillon){
	if (pActual == NULL){
		pPrevious->SetNext(pNewMaillon);
		return true;
	}

	TLinkedMaillon *pTemp = pActual->GetNext();
	pActual->SetNext(pNewMaillon);
	return pPrevious->Parcours(AddMaillon, pTemp, NULL);
}
//remplace pActual par pNewMaillon si il est non nul
//sinon on supprime pActual
static bool RemoveMaillon(TLinkedMaillon *pPrevious, TLinkedMaillon * pActual, TLinkedMaillon * pNewMaillon){
	if (pNewMaillon != NULL){
		pPrevious->SetNext(pNewMaillon);
		return pPrevious->Parcours(AddMaillon, pActual->GetNext(), NULL);
	}
	else
		pPrevious->SetNext(pActual->GetNext());

	return true;
}


#endif
