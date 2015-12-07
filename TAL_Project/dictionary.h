#ifndef DICTIONARY_H_INCLUDED
#define DICTIONARY_H_INCLUDED

#include "debug.h"
#include "baseObject.h"
#include <vector>

template <class T>
class AVL_tree;
template <class T>
class node;

template <class T>
struct TDictionaryData{
	char *m_pKey;
	T m_tElement;

	TDictionaryData(const char *pKey = NULL, const T &tElement = NULL){
		if (pKey == NULL)
			m_pKey = NULL;
		else
		{
			m_pKey = (char *)malloc(strlen(pKey) + 1);
			strncpy(m_pKey, pKey, strlen(pKey));
			m_pKey[strlen(pKey)] = 0;
			m_tElement = tElement;
		}
	}
	~TDictionaryData(){
		if (m_pKey != NULL)
			free(m_pKey);
		if (m_tElement != NULL)
			free(m_tElement);
	}

	TDictionaryData& operator=(const TDictionaryData& tOther){
		if (this != &tOther){
			this->m_pKey = tOther.m_pKey;
			this->m_tElement = tOther.m_tElement;
		}
		return *this;
	}
	inline bool operator==(const TDictionaryData& tOther)const{
		if (this->m_pKey == NULL && tOther.m_pKey == NULL)
			return true;
		else if (this->m_pKey == NULL || tOther.m_pKey == NULL)
			return false;
		else
			return strcmp(this->m_pKey, tOther.m_pKey) == 0;
	}
	bool operator<(const TDictionaryData& tOther)const{
		if (this->m_pKey == NULL || tOther.m_pKey == NULL)
			return false;
		else
			return strcmp(this->m_pKey, tOther.m_pKey) < 0;
	}
};

namespace CustomStd
{
	class STDDictionary : public STDBaseObject
	{
		AVL_tree<TDictionaryData<STDBaseObject *> *> *m_pTree;
		void(*m_pDeleteCallback)(node<TDictionaryData<STDBaseObject *> *> *);

	public:
		STDDictionary();
		~STDDictionary();

		void SetCallback(void(*pCallback)(node<TDictionaryData<STDBaseObject *> *> *));

		void AddElementForKey(STDBaseObject *pElement, const char *pKey);
		STDBaseObject *GetElementForKey(char *pKey)const;
	};
}

#endif
