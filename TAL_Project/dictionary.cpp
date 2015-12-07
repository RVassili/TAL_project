#include "dictionary.h"
#include "node.h"



namespace CustomStd
{
	STDDictionary::STDDictionary(){
		m_pTree = new AVL_tree<TDictionaryData<STDBaseObject *> *>();
		m_pDeleteCallback = NULL;
	}
	STDDictionary::~STDDictionary(){
		if (m_pDeleteCallback != NULL)
			m_pTree->suffix(m_pDeleteCallback);
		delete m_pTree;
	}

	void STDDictionary::SetCallback(void(*pCallback)(node<TDictionaryData<STDBaseObject *> *> *)){
		m_pDeleteCallback = pCallback;
	}

	void STDDictionary::AddElementForKey(STDBaseObject *pElement, const char *pKey){
		GfxDbgAssert(pKey != NULL, "can not add element in dictionary with NULL key");
		TDictionaryData<STDBaseObject *> *pNewData = new TDictionaryData<STDBaseObject *>(pKey, pElement);
		if (!m_pTree->add(pNewData))
			delete pNewData;
	}
	STDBaseObject *STDDictionary::GetElementForKey(char *pKey)const{
		if (m_pTree == NULL)
			return NULL;

		TDictionaryData<STDBaseObject *> *tTempData = new TDictionaryData<STDBaseObject *>(pKey);
		TDictionaryData<STDBaseObject *> *tData = m_pTree->findElement(tTempData);
		delete tTempData;

		if (tData == NULL)
			return NULL;

		return tData->m_tElement;
	}
}
